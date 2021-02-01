/**
 * STL的基本算法
**/

#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include <string.h>
#include "type_traits.hpp"
#include "iterator.hpp"

namespace MySTL
{
    /*****************************************************************************************
     * fill_n
     * 从 first 位置开始填充 n 个值
    *****************************************************************************************/
    template<class OutputIterator, class Size, class T>
    OutputIterator fill_n(OutputIterator first, Size n, const T & value) {
        for( ; n>0; --n, ++first) {
            *first = value;
        }
        return first;
    }
    /*****************************************************************************************
     * fill
     * 为 [first, last)区间内的所有元素填充新值
    *****************************************************************************************/
    //ForwardIterator版本
    template<class ForwardIterator, class T>
    void __fill(ForwardIterator first, ForwardIterator last, const T & value, forward_iterator_tag) {
        for( ; first!=last; ++first) {
            *first = value;
        }
    }
    //RandomAccessIterator版本
    template<class RandomAccessIterator, class T>
    void __fill(RandomAccessIterator first, RandomAccessIterator last, const T & value, random_access_iterator_tag) {
        fill_n(first, last-first, value);
    }
    template<class ForwardIterator, class T>
    void fill(ForwardIterator first, ForwardIterator last, const T & value) {
        __fill(first, last, value, iterator_category(first));
    }
    /*****************************************************************************************
     * copy
     * 把 [first, last) 上的内容复制到以result为起始处的空间，返回复制结束的位置
    *****************************************************************************************/
    //InputIterator版本
    template<class InputIterator, class OutputIterator>
    OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
        //只能以迭代器等同与否，判断循环是否继续，速度慢
        for ( ; first != last; ++result, ++first) {
            //copy operator assignment
            *result = *first;
        }
        return result;
    }
    //__copy_d
    template<class RandomAccessIterator, class OutputIterator, class Distance>
    OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance *) {
        //以n决定循环的执行次数，速度快
        for(Distance n = last-first; n>0; --n, ++result, ++first) {
            *result = *first;
        }
        return result;
    }
    //RandomAccessIterator版本
    template<class RandomAccessIterator, class OutputIterator>
    OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag) {
        // 又划分一个函数，为的是其他地方也可能用到
        return __copy_d(first, last, result, difference_type(first));
    }
    
    //完全泛化版本
    template<class InputIterator, class OutputIterator>
    struct __copy_dispatch {
        OutputIterator operator() (InputIterator first, InputIterator last, OutputIterator result) {
            return __copy(first, last, result, iterator_category(first));
        }
    };
    //__copy_t trivial
    template<class T>
    T* __copy_t(const T* first, const T* last, T* result, __true_type) {
        // memmove以字节为单位copy 在内存覆盖上比memcpy函数做得更好 
        memmove(result, first, (last-first) * sizeof(T));
        return result + (last - first);
    }
    //__copy_t non-trivial
    template<class T>
    T* __copy_t(const T* first, const T* last, T* result, __false_type) {
        //原生指针也是一种RandomAccessIterator
        return __copy_d(first, last, result, difference_type(first));
    }
    //偏特化 T*, T*
    template<class T>
    struct  __copy_dispatch<T*, T*>
    {
        T* operator() (T* first, T* last, T* result) {
            using t = typename __type_traits<T>::has_trivial_assignment_operator;
            return __copy_t(first, last, result, t());
        }
    };
    //偏特化 const T*, T*
    template<class T>
    struct  __copy_dispatch<const T *, T*>
    {
        T* operator() (const T* first, const T* last, T* result) {
            using t = typename __type_traits<T>::has_trivial_assignment_operator;
            return __copy_t(first, last, result, t());
        }
    };
    //copy
    template<class InputIterator, class OutputIterator>
    OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
        return __copy_dispatch<InputIterator, OutputIterator>()
                    (first, last, result);
    }
    /*****************************************************************************************
     * copy_backward 逆向复制
     * 把 [first, last) 内的每个元素，以逆行的方式复制到以result-1为起点，方向亦为逆行的区间上，即[result-(last-first), result)
    *****************************************************************************************/
    template<class BidirectionalIterator1, class BidirectionalIterator2>
    BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
                                         BidirectionalIterator2 result) {
           
    }
    /*****************************************************************************************
     * equal
     * 比较第一序列在 [first, last)区间上的元素值是否和第二序列相等
    *****************************************************************************************/
    template<class InputIterator1, class InputIterator2>
    bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
        for( ; first1 != last1; ++first1, ++first2) {
            if(*first1 != *first2) return false;
        }
        return true;
    }
    /*****************************************************************************************
     * swap
     * 交换两个对象的内容
    *****************************************************************************************/
    template<class T>
    void swap(T& a, T& b) {
        T tmp = a;
        a = b;
        b = tmp;
    }
    /*****************************************************************************************
     * max
     * 取二者中的较大值，语义相等时保证返回第一个参数
    *****************************************************************************************/
    template<class T>
    const T& max(const T& lhs, const T& rhs) {
        return lhs < rhs ? rhs : lhs;
    }
    //重载版本 使用仿函数comp来比较大小
    template<class T, class Compare>
    const T& max(const T& lhs, const T& rhs, Compare comp) {
        return comp(lhs, rhs) ? rhs : lhs;
    }
}


#endif