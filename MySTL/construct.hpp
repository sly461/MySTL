/**
 * 对象的构造与析构
**/
#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_

#include <new>
#include "type_traits.hpp"

namespace MySTL
{
	//placement new
	template <class T1, class T2>
	void construct(T1 *p, const T2 &value)
	{
		new (p) T1(value);
	}
	//调用析构函数,第一个版本，接受一个指针
	template <class T>
	void destroy(T *pointer)
	{
		pointer->~T();
	}
	//接收两个迭代器
	template <class ForwordIterator>
	void destroy(ForwordIterator first, ForwordIterator last)
	{
		using __value_type = typename std::iterator_traits<ForwordIterator>::value_type;
		using trivial_destoructor = typename MySTL::__type_traits<__value_type>::has_trivial_destructor;
		__destroy(first, last, trivial_destoructor());
	}

	template <class ForwardIterator>
	void __destroy(ForwardIterator first, ForwardIterator last, MySTL::__true_type)
	{
	}

	template <class ForwardIterator>
	void __destroy(ForwardIterator first, ForwardIterator last, MySTL::__false_type)
	{
		for (; first < last; ++first)
		{
			destroy(&*first);
		}
	}

} // namespace MySTL

#endif