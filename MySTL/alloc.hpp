/**
 * 内存管理，以字节为单位进行内存分配与释放
 * 供allocator内部使用
**/
#ifndef _ALLOC_H_
#define _ALLOC_H_


#include<cstdlib>

namespace MySTL {
	class alloc {
	private:
		enum {__ALIGN = 8}; //小型区块的上调边界
		enum {__MAX_BYTES = 128}; //小型区块的上限,超过__MAX_BYTES则直接由malloc分配
		enum {__NFREELISTS = __MAX_BYTES / __ALIGN}; //free_lists个数
		enum {NOBJS = 20}; //默认每次从内存池取20个区块

	private:
        //将bytes上调至8的倍数
		static size_t ROUND_UP(size_t bytes) {
			return (((bytes)+__ALIGN - 1)&~(__ALIGN - 1));
            //算法巧妙 例如
		}

	private:
        //free_lists的节点
		union obj {
			union obj* free_list_link;
			char client_data[1];
		};

		//16个free_list
		static obj* free_list[__NFREELISTS];
	private:
		//内存池
		static char *start_free; //内存池起始位置
		static char *end_free; //内存池结束位置
		static size_t heap_size;

	private:
		//根据区块大小，决定使用第n号free_list，n从0算起 
		static size_t FREELIST_INDEX(size_t bytes) {
			return (((bytes)+__ALIGN - 1)/__ALIGN - 1);
		}
		//返回一个大小为n的对象，并可能加入大小为n的其他区块到free_list
		static void *refill(size_t n);
		//配置一大块空间，可容纳nobjs个大小为size的区块
		//如果配置nobjs个区块有所不便，nobjs可能会降低
		static char *chunk_alloc(size_t size, size_t& nobjs);

	public:
		static void *allocate(size_t bytes);
		static void deallocate(void *ptr, size_t bytes);
		static void *reallocate(void *ptr, size_t old_sz, size_t new_sz);
	public:
		alloc() {};
		~alloc() {};
	};

}


#endif