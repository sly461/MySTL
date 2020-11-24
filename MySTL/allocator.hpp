/**
 * 1、内存的分配与释放（以对象为单位） allocate deallocate
 * 2、对象的构造与析构 construct destroy
**/
#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include "construct.hpp"

namespace MySTL
{
	template <class T>
	class allocator
	{
	public:
		using value_type = T;
		using pointer = T *;
		using const_pointer = const T *;
		using reference = T &;
		using const_reference = const T &;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

	public:
		//1、内存分配与释放：分配未构造的内存空间，使用alloc
		static T *allocate();
		static T *allocate(size_t n);
		static void deallocate(T *ptr);
		static void deallocate(T *ptr, size_t n);

		//2、对象的构造与析构，使用construct
		static void construct(T *ptr);
		static void construct(T *ptr, const T &value);
		static void destroy(T *ptr);
		static void destroy(T *first, T *last);

	public:
		allocator(){};
		~allocator(){};
	};

	template <class T>
	T *allocator<T>::allocate()
	{
		//operator new分配内存
		return static_cast<T *>(::operator new(sizeof(T)));
	}

	template <class T>
	T *allocator<T>::allocate(size_t n)
	{
		if (0 == n)
			return nullptr;
		//operator new分配内存
		return static_cast<T *>(::operator new(sizeof(T) * n));
	}

	template <class T>
	void allocator<T>::deallocate(T *ptr)
	{
		if (nullptr == ptr)
			return;
		::operator delete(ptr);
	}

	template <class T>
	void allocator<T>::deallocate(T *ptr, size_t n)
	{
		if (0 == n || nullptr == ptr)
			return;
		::operator delete(ptr);
	}

	//调用default placement new
	template <class T>
	void allocator<T>::construct(T *ptr)
	{
		MySTL::construct(ptr, T());
	}
	//带参调用 placement new
	template <class T>
	void allocator<T>::construct(T *ptr, const T &value)
	{
		MySTL::construct(ptr, value);
	}
	template <class T>
	void allocator<T>::destroy(T *ptr)
	{
		MySTL::destroy(ptr);
	}
	template <class T>
	void allocator<T>::destroy(T *first, T *last)
	{
		MySTL::destroy(first, last);
	}

} // namespace MySTL

#endif