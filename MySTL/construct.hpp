/**
 * 对象的构造与析构
**/
#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_

#include <new>
#include "type_traits.hpp"
#include "iterator.hpp"

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
		//iterator_traits萃取出迭代器或者原生指针所指之物的类型
		using __value_type = typename iterator_traits<ForwordIterator>::value_type;
		using trivial_destoructor = typename __type_traits<__value_type>::has_trivial_destructor;
		__destroy(first, last, trivial_destoructor());
	}

	template <class ForwardIterator>
	void __destroy(ForwardIterator first, ForwardIterator last, __true_type)
	{
	}

	//与上函数同名 根据不同型别使得重载机制得以有效运行 重载机制是编译时决定（静态联编）
	template <class ForwardIterator>
	void __destroy(ForwardIterator first, ForwardIterator last, __false_type)
	{
		for (; first < last; ++first)
		{
			destroy(&*first);
		}
	}

} // namespace MySTL

#endif