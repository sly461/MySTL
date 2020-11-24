/**
 * 内存管理，以字节为单位进行内存分配与释放
 * 供allocator内部使用
**/
#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <cstdlib>

namespace MySTL
{
	class alloc
	{
	private:
		enum{__ALIGN = 8}; //小型区块的上调边界
		enum{__MAX_BYTES = 128}; //小型区块的上限,超过__MAX_BYTES则直接由malloc分配
		enum{__NFREELISTS = __MAX_BYTES / __ALIGN}; //free_lists个数
		enum{__NOBJS = 20}; //默认每次从内存池取20个区块

	private:
		//将bytes上调至8的倍数
		static size_t ROUND_UP(size_t bytes)
		{
			return (((bytes) + __ALIGN - 1) & ~(__ALIGN - 1));
			//算法很巧妙 例如21bytes
			//00011100 & 11111000 = 00011000 = 24
		}

	private:
		//free_lists的节点
		union obj
		{
			union obj *free_list_link;
			char client_data[1];
		};

		//16个free_list
		static obj *free_list[__NFREELISTS];

	private:
		//内存池
		static char *start_free; //内存池起始位置
		static char *end_free;	 //内存池结束位置
		static size_t heap_size;

	private:
		//根据区块大小，决定使用第n号free_list，n从0算起
		static size_t FREELIST_INDEX(size_t bytes)
		{
			return (((bytes) + __ALIGN - 1) / __ALIGN - 1);
		}
		//返回一个大小为n的对象，并可能加入大小为n的其他区块到free_list
		static void *refill(size_t n);
		//配置一大块空间，可容纳nobjs个大小为size的区块
		//如果配置nobjs个区块有所不便，nobjs可能会降低
		static char *chunk_alloc(size_t size, size_t &nobjs);

	public:
		static void *allocate(size_t bytes);
		static void deallocate(void *ptr, size_t bytes);
		static void *reallocate(void *ptr, size_t old_sz, size_t new_sz);

	public:
		alloc(){};
		~alloc(){};
	};

	//初始化
	char *alloc::start_free = nullptr;
	char *alloc::end_free = nullptr;
	size_t alloc::heap_size = 0;

	alloc::obj *alloc::free_list[alloc::__NFREELISTS] = {
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
	};

	void *alloc::allocate(size_t n)
	{
		//大于128就调用一级配置器
		if (n > (size_t)__MAX_BYTES)
		{
			return (malloc(n));
		}
		//寻找16个 free_lists 中适当大小的一个空间
		size_t index = FREELIST_INDEX(n);
		obj *my_free_list = free_list[index];
		//有可用空间，从free_list中取出适合大小的区块
		if (my_free_list)
		{
			free_list[index] = my_free_list->free_list_link;
			return my_free_list;
		}
		//没有适合大小的空间，需要从内存池中refill
		return refill(ROUND_UP(n));
	}

	void alloc::deallocate(void *ptr, size_t n)
	{
		//大于128
		if (n > size_t(__MAX_BYTES))
		{
			free(ptr);
		}
		//收回到free_lists 中
		else
		{
			size_t index = FREELIST_INDEX(n);
			obj *node = static_cast<obj *>(ptr);
			node->free_list_link = free_list[index];
			free_list[index] = node;
		}
	}
	void *alloc::reallocate(void *ptr, size_t old_sz, size_t new_sz)
	{
		deallocate(ptr, old_sz);
		ptr = allocate(new_sz);

		return ptr;
	}
	//n 已经被上调到8的倍数
	void *alloc::refill(size_t n)
	{
		//缺省补充20个区块
		size_t nobjs = __NOBJS;
		char *chunk = chunk_alloc(n, nobjs);
		obj *result;
		obj *cur, *next;
		if (1 == nobjs)
		{
			return chunk;
		}
		size_t index = FREELIST_INDEX(n);

		result = (obj *)(chunk);
		free_list[index] = next = (obj *)(chunk + n);
		//将取出的多余的空间加入到相应的free list里面去
		for (int i = 1; i < nobjs-1 ; ++i)
		{
			cur = next;
			next = (obj *)((char *)next + n);
			cur->free_list_link = next;
		}
		//注意最后接一个空指针
		next->free_list_link = nullptr;
		return result;
	}
	//内存池管理
	char *alloc::chunk_alloc(size_t bytes, size_t &nobjs)
	{
		char *result = 0;
		size_t need_bytes = bytes * nobjs;
		size_t bytes_left = end_free - start_free;

		if (bytes_left >= need_bytes)
		{ //内存池剩余空间完全满足需要
			result = start_free;
			start_free = start_free + need_bytes;
			return result;
		}
		else if (bytes_left >= bytes)
		{ //内存池剩余空间不能完全满足需要，但足够供应一个或以上的所需求大小区块
			nobjs = bytes_left / bytes;
			need_bytes = nobjs * bytes;
			result = start_free;
			start_free += need_bytes;
			return result;
		}
		else
		{ //内存池剩余空间连一个区块的大小都无法提供
			size_t bytes_to_get = 2 * need_bytes + ROUND_UP(heap_size >> 4);
			if (bytes_left > 0)
			{
				obj **my_free_list = free_list + FREELIST_INDEX(bytes_left);
				((obj *)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj *)start_free;
			}
			start_free = (char *)malloc(bytes_to_get);
			if (!start_free)
			{ //malloc都找不到空间，寻找大小相近的空间
				obj **my_free_list = 0, *p = 0;
				for (int i = 0; i <= __MAX_BYTES; i += __ALIGN)
				{
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (p != 0)
					{
						*my_free_list = p->free_list_link;
						start_free = (char *)p;
						end_free = start_free + i;
						return chunk_alloc(bytes, nobjs);
					}
				}
				end_free = 0;
			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return chunk_alloc(bytes, nobjs);
		}
	}

} // namespace MySTL

#endif