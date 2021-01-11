/**
 * vector容器
**/
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <cstdlib>
#include "allocator.hpp"
#include "uninitialized.hpp"

namespace MySTL {
    //声明
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

        //分配内存
        using data_allocator = Alloc;

        //配置空间 填满内容 同时设置start、finish、end_of_storage等迭代器
        iterator allocate_and_fill(size_type n, const T& x);
        void fill_initialize(size_type n, const T& value);

    public:
        //相关构造函数及析构函数及赋值运算符
        vector(): start(nullptr), finish(nullptr), end_of_storage(nullptr) {}
        vector(size_type n, const T& value) { fill_initialize(n, value); }
        vector(int n, const T& value) { fill_initialize(n, value); }
        vector(long n, const T& value) { fill_initialize(n, value); }
        explicit vector(size_type n) { fill_initialize(n, T()); }
        template<class InputIterator>
        vector(InputIterator first, InputIterator last);
        //拷贝构造
        vector(const vector & rhs);
        vector(vector && rhs);
        //赋值运算
        vector& operator = (const vector & rhs);
        vector& operator = (vector && rhs);
        //析构函数
        ~vector();


        //比较操作
        bool operator == (const vector& v) const;
        bool operator != (const vector& v) const;


        //迭代器相关
        iterator begin() { return start; }
        iterator end() { return end; }


        //与容量相关
        

    };

    /*****************************************************************************************
     * 具体实现
    *****************************************************************************************/
    //配置空间 填满内容
    template<class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::allocate_and_fill(size_type n, const T& x) {
        iterator result = data_allocator::allocate(n);
        //暂时用memory头文件中的函数 
        //提取迭代器所指对象的类型 value_type()
        //判断型别是否是POD型别 __type_traits<>
        //然后采用有效率或者保险安全的初值填写方法
        uninitialized_fill_n(result, n, x);
        return result;
    }
    //设置start、finish、end_of_storage等迭代器
    template<class T, class Alloc> 
    void vector<T, Alloc>::fill_initialize(size_type n, const T& value) {
        start = allocate_and_fill(n, value);
        finish = end_of_storage = start + n;
    }
    template<class T, class Alloc>
    template<class InputIterator>
    vector<T, Alloc>::vector(InputIterator first, InputIterator last) {

    }
    //析构
    template<class T, class Alloc>
    vector<T, Alloc>::~vector() {

    }
    //拷贝构造
    template<class T, class Alloc>
    vector<T, Alloc>::vector(const vector & rhs) {
        
    }

}



#endif