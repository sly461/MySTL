/**
 * vector容器
**/
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <cstdlib>
#include "allocator.hpp"

namespace MySTL {
    template<class T, class Alloc = allocator<T>>
    class vector {
    public:
        //vector的嵌套型别定义
        using value_type = T;
        using pointer = value_type*;
        using iterator = value_type*;
        using reference = value_type&;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
    protected:
        iterator start;             //目前使用空间的头
        iterator finish;            //目前使用空间的尾
        iterator end_of_storage;    //目前可用空间的尾

        using data_allocator = Alloc;

    public:
        //相关构造函数及析构函数及赋值运算符
        vector(): start(nullptr), finish(nullptr), end_of_storage(nullptr) {}
        vector(size_type n, const T& value) {}
        explicit vector(size_type n) {}

        ~vector() {
            
        }
    }
}



#endif