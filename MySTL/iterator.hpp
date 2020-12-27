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

    //iterator_traits
    

}


#endif