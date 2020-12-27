/**
 * 迭代器
**/
#ifndef _ITERATOR_H_
#define _ITERATOR_H_


#include <cstddef>

namespace MySTL{
    //五种迭代器类型
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag: public input_iterator_tag {};
    struct bidirectional_iterator_tag: public forward_iterator_tag {};
    struct random_access_iterator_tag: public bidirectional_iterator_tag {};

    //iterator 模板
    template <class Category, class T, class Distance = ptrdiff_t, 
              class Pointer = T*, class Reference = T&>
    struct iterator {
        using iterator_category = Category;
        using value_type = T;
        using difference_type = Distance;
        using pointer = Pointer;
        using reference = Reference;
    };

    //iterator_traits 迭代器指向类型萃取
    template<class Iterator>
    struct iterator_traits {
        using iterator_category = typename Iterator::iterator_category;
        using value_type = typename Iterator::value_type;
        using difference_type = typename Iterator::difference_type;
        using pointer = typename Iterator::pointer;
        using reference = typename Iterator::reference;
    };

    //原生指针
    template<class T>
    struct iterator_traits<T *> {
        //原生指针是一种随机访问的迭代器
        using iterator_category = random_access_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using reference = T&;
    };

    //原生指针 - const
    template<class T>
    struct iterator_traits<const T *> {
        using iterator_category = random_access_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;
    };
    
    //获取迭代器的类型
    template<class Iterator>
    typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator &)
    {
        using category = typename iterator_traits<Iterator>::iterator_category;
        return category();
    }

    //获取迭代器的value_type
    template<class Iterator>
    typename iterator_traits<Iterator>::value_type *
    value_type(const Iterator &)
    {
        using value_type = typename iterator_traits<Iterator>::value_type;
        return static_cast<value_type *>(nullptr);
    }

    //获取迭代器的distance_type
    template<class Iterator>
    typename iterator_traits<Iterator>::difference_type *
    difference_type(const Iterator &)
    {
        using difference_type = typename iterator_traits<Iterator>::difference_type;
        return static_cast<difference_type *>(nullptr);
    }
    
    //整组advance函数：操作迭代器移动distance个位置

    //ForwardIterator继承自InputIterator，因此ForwardIterator与InputIterator
    //都能调用此函数 STL源码中称之为“消除了单纯传递调用的函数”
    template<class InputIterator, class Distance>
    void __advance(InputIterator & i, Distance n, input_iterator_tag) {
        while(n--) ++i;
    }

    template<class BidirectionalIterator, class Distance>
    void __advance(BidirectionalIterator & i, Distance n, bidirectional_iterator_tag) {
        if(n > 0) {
            while(n--) ++i;
        }
        else {
            while(n++) --i;
        }
    }

    template<class RandomAccessIterator, class Distance>
    void __advance(RandomAccessIterator & i, Distance n, random_access_iterator_tag) {
        i += n;
    }

    //template 型别参数命名为InputIterator，遵循STL算法的命名规则
    //即以算法所能接受之最初级类型来为其迭代器型别参数命名
    template<class InputIterator, class Distance>
    void advance(InputIterator & i, Distance n) {
        __advance(i, n, iterator_category(i));
    }

    //整组distance函数:迭代器first与last之间的距离

    //ForwardIterator继承自InputIterator，因此BidirectionalIterator与ForwardIterator与InputIterator
    //都能调用此函数 STL源码中称之为“消除了单纯传递调用的函数”
    template<class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
    __distance(InputIterator first, InputIterator last, input_iterator_tag) {
        iterator_traits<InputIterator>::difference_type n = 0;
        while(first != last) {
            ++n;
            ++first;
        }
        return n;
    }

    template<class RandomAccessIterator>
    typename iterator_traits<RandomAccessIterator>::difference_type
    __distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
        return last - first;
    }

    //template 型别参数命名为InputIterator，遵循STL算法的命名规则
    //即以算法所能接受之最初级类型来为其迭代器型别参数命名
    template<class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last) {
        __distance(first, n, iterator_category(first));
    }
}


#endif