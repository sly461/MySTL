/**
 * 定义了全局函数，用来填充（fill）或复制（copy）大块内存数据
 * 根据对象的类型（是否是POD）最差情况下会调用construct()
 * 最佳情况下会使用C标准函数memmove()直接进行内存数据的移动
**/
#ifndef _UNINITIALIZED_H_
#define _UNINITIALIZED_H_


#include <memory>
#include <iostream>
#include "type_traits.hpp"
#include "iterator.hpp"
#include "construct.hpp"

namespace MySTL {
    /*****************************************************************************************
     * uninitialized_fill 
     * 在 [first, last) 区间内填充元素值
    *****************************************************************************************/
    template<class ForwardIterator, class T>
    void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __true_type) {
        //调用STL算法fill()
        
    }
    //必须一个一个元素地构造，不能批量
    template<class ForwardIterator, class T>
    void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __false_type) {
        ForwardIterator cur = first;
        //cur可能是原生指针，也可能是迭代器，所以需要解引用再取地址
        for( ; cur!=last; ++cur) construct(&*cur, x);
    }
    template<class ForwardIterator, class T, class T1>
    void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*) {
        using is_POD = typename __type_traits<T1>::is_POD_type;
        __uninitialized_fill_aux(first, last, x, is_POD());
    }
    template<class ForwardIterator, class T>
    void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x) {
        __uninitialized_fill(first, last, x, value_type(first));
    }


    /*****************************************************************************************
     * uninitialized_fill_n
     * 从 first 位置开始，填充 n 个元素值，返回填充结束的位置
    *****************************************************************************************/
    template<class ForwardIterator, class Size, class T>
    ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type) {
        //调用STL算法fill_n()
        std::fill_n(first, n, x);
    }
    //必须一个一个元素地构造，不能批量
    template<class ForwardIterator, class Size, class T>
    ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type) {
        ForwardIterator cur = first;
        //cur可能是原生指针，也可能是迭代器，所以需要解引用再取地址
        while(n--) {
            construct(&*cur, x);
            std::cout<< x <<std::endl;
            ++cur;
        }
        return cur;
    }
    template<class ForwardIterator, class Size, class T, class T1>
    ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*) {
        using is_POD = typename __type_traits<T1>::is_POD_type;
        __uninitialized_fill_n_aux(first, n, x, is_POD());
    }
    template<class ForwardIterator, class Size, class T>
    ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
        return __uninitialized_fill_n(first, n, x, value_type(first));
    }

}


#endif