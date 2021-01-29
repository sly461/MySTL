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
        using const_iterator = const iterator;
        using reference = value_type&;
        using const_reference = const reference;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

    protected:
        iterator start;             //目前使用空间的头
        iterator finish;            //目前使用空间的尾
        iterator end_of_storage;    //目前可用空间的尾

        //分配内存
        using data_allocator = Alloc;

        //配置空间 填满内容(fill or copy) 同时设置start、finish、end_of_storage等迭代器
        iterator allocate_and_fill(size_type n, const T& x);
        template<class InputIterator>
        iterator allocate_and_copy(InputIterator first, InputIterator last);
        
        void fill_initialize(size_type n, const T& value);
        template<class InputIterator>
        void copy_initialize(InputIterator first, InputIterator last);

        //修改容器相关操作
        //push_back调用 insert调用
        void insert_aux(iterator position, const T& value);

    public:
        //相关构造函数及析构函数及赋值运算符
        vector(): start(nullptr), finish(nullptr), end_of_storage(nullptr) {}
        vector(size_type n, const T& value) { fill_initialize(n, value); }
        vector(int n, const T& value) { fill_initialize(n, value); }
        vector(long n, const T& value) { fill_initialize(n, value); }
        explicit vector(size_type n) { fill_initialize(n, T()); }
        template<class InputIterator>
        vector(InputIterator first, InputIterator last) { copy_initialize(first, last); }
        //拷贝构造
        vector(const vector & rhs) { copy_initialize(rhs.start, rhs.finish); }
        vector(vector && rhs);
        //赋值运算
        vector& operator = (const vector & rhs);
        vector& operator = (vector && rhs);
        //析构函数
        ~vector();


        //迭代器相关
        iterator begin() { return start; }
        const_iterator begin() const { return start; }
        const_iterator cbegin() const { return start; }
        iterator end() { return finish; }
        const_iterator end() const { return finish; }
        const_iterator cend() const { return finish; }


        //访问元素相关
        reference operator[] (const size_type i) { return *(begin() + i); }
        const_reference operator[] (const size_type i) const { return *(begin() + i); }
        reference front() { return *begin(); }
        const_reference front() const { return *begin(); }
        reference back() { return *(end()-1); }
        const_reference back() const { return *(end()-1); }


        //修改容器相关操作
        void push_back(const value_type & value);
        void pop_back();
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
        //销毁对象 但未释放空间
        void clear() { erase(begin(), end()); }
        void swap(vector& v);


        //与容量相关
        difference_type size() const { return finish-start; }
        difference_type capacity() const { return end_of_storage-start; }
        bool empty() const { return start==finish; }
        //void resize(size_type new_size, const value_type & value = value_type());
        

    };

    /*****************************************************************************************
     * 具体实现
     * 相关构造函数及析构函数及赋值运算符
    *****************************************************************************************/
    //配置空间 填满内容
    template<class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::allocate_and_fill(size_type n, const T& x) {
        iterator result = data_allocator::allocate(n);
        //暂时用memory头文件中的函数 
        //std::uninitialized_fill_n(result, n, x);
        //提取迭代器所指对象的类型 value_type()
        //判断型别是否是POD型别 __type_traits<>
        //然后采用有效率或者保险安全的初值填写方法
        uninitialized_fill_n(result, n, x);
        return result;
    }
    template<class T, class Alloc>
    template<class InputIterator>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::allocate_and_copy(InputIterator first, InputIterator last) {
        size_type n = static_cast<size_type>(last-first);
        iterator result = data_allocator::allocate(n);
        uninitialized_copy(first, last, result);
        return result;
    }
    //设置start、finish、end_of_storage等迭代器
    template<class T, class Alloc> 
    void vector<T, Alloc>::fill_initialize(size_type n, const T& value) {
        start = allocate_and_fill(n, value);
        finish = end_of_storage = start + n;
    }
    //设置start、finish、end_of_storage等迭代器
    template<class T, class Alloc> 
    template<class InputIterator>
    void vector<T, Alloc>::copy_initialize(InputIterator first, InputIterator last) {
        size_type n = static_cast<size_type>(last-first);
        start = allocate_and_copy(first, last);
        finish = end_of_storage = start + n;
    }
    //拷贝构造
    template<class T, class Alloc>
    vector<T, Alloc>::vector(vector && rhs)
        : start(rhs.start),
          finish(rhs.finish),
          end_of_storage(rhs.end_of_storage)
    {
        rhs.start = rhs.finish = rhs.end_of_storage = nullptr;
    }
    //赋值运算 原来对象如何处理？
    template<class T, class Alloc>
    vector<T, Alloc>& vector<T, Alloc>::operator = (const vector & rhs) {
        if(&rhs != this) {
            const size_type rhsLen = rhs.size();
            //>capacity 重新申请一块足够大的地址
            if(rhsLen > capacity()) {
                iterator tmp = allocate_and_copy(rhs.begin(), rhs.end());
                //销毁对象
                destroy(start, finish);
                //释放内存
                data_allocator::deallocate(start, capacity());
                start = tmp;
                end_of_storage = start + rhsLen;
            }
            //>size, <=capacity
            else if(rhsLen > size()) {
                //<=size的部分
                copy(rhs.begin(), rhs.begin()+size(), start);
                //>size的部分 这部分空间尚未初始化（未构造对象）
                uninitialized_copy(rhs.begin()+size(), rhs.end(), finish);
            }
            //<=size
            else {
                iterator i = copy(rhs.begin(), rhs.end(), begin());
                //注意销毁掉[i, finish)的对象
                destroy(i, finish);
            }
            finish = start + rhsLen;
        }
        return *this;
    }
    template<class T, class Alloc>
    vector<T, Alloc>& vector<T, Alloc>::operator = (vector && rhs) {
        if(this != &rhs) {
            //销毁对象
            if(size() != 0) destroy(start, finish);
            //释放内存
            if(capacity() != 0) data_allocator::deallocate(start, capacity());
            start = rhs.start;
            finish = rhs.finish;
            end_of_storage = rhs.end_of_storage;
            rhs.start = rhs.finsh = rhs.end_of_storage = nullptr;
        }
        return *this;
    }
    //析构
    template<class T, class Alloc>
    vector<T, Alloc>::~vector() {
        //销毁对象
        if(size() != 0) destroy(begin(), end());
        //释放内存
        if(capacity() != 0) data_allocator::deallocate(begin(), capacity());
        start = finish = end_of_storage = nullptr;
    }



    /*****************************************************************************************
     * 具体实现
     * 修改容器相关操作
    *****************************************************************************************/
    //供push_back和insert内部使用
    template<class T, class Alloc>
    void vector<T, Alloc>::insert_aux(iterator position, const T& value) {
        //还有备用空间
        if(finish != end_of_storage) {
            //在备用空间起始处构造一个元素，并以vector最后一个元素值为其初值
            construct(finish, *(finish-1));
            ++finish;
            T value_copy = value;
            std::copy_backward(position, finish-2, finish-1);
            *position = value_copy;
        }
        //无备用空间
        else {
            const size_type old_size = size();
            //配置原则
            //如果原大小为0，则配置1个元素
            //否则配置原大小的两倍
            const size_type len = old_size == 0 ? 1 : old_size*2;

            //分配内存
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;
            try {
                //将原vector拷贝到新vector
                new_finish = uninitialized_copy(start, position, new_start);
                //构造新值
                construct(new_finish, value);
                ++new_finish;
                //将position及后面的部分拷贝过来
                new_finish = uninitialized_copy(position, finish, new_finish);
            }
            catch(...) {
                // "commit or rollback"
                destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, len);
                throw;
            }
            
            //销毁原对象并释放空间
            destroy(begin(), end());
            data_allocator::deallocate(begin(), capacity());

            //调整迭代器指向新vector
            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + len;
        }
    }
    template<class T, class Alloc>
    void vector<T, Alloc>::push_back(const value_type & value) {
        //有备用空间
        if(finish != end_of_storage) {
            construct(finish, value);
            ++finish;
        }
        else insert_aux(end(), value);
    }
    template<class T, class Alloc>
    void vector<T, Alloc>::pop_back() {
        --finish;
        //销毁对象
        destroy(finish);
    }
    //清除某个位置上的元素
    template<class T, class Alloc>
    vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator position) {
        if(position+1 != end()) {
            copy(position+1, end(), position);
        }
        --finish;
        destroy(finish);
        return position;
    }
    template<class T, class Alloc>
    vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last) {
        iterator i = copy(last, finish, first);
        destroy(i, finish);
        finish = finish - (last - first);
        return first;
    }



    /*****************************************************************************************
     * 具体实现
     * 与容量相关
    *****************************************************************************************/
    // template<class T, class Alloc>
    // void vector<T, Alloc>:: resize(size_type new_size, const value_type & value = value_type()) {
        
    // }



    /*****************************************************************************************
     * 其他相关函数
    *****************************************************************************************/
    //重载比较操作
    template<class T, class Alloc>
    bool operator == (const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs) {
        return (lhs.size() == rhs.size() && equal(lhs.begin(), lhs.end(), rhs.begin()));
    }
    template<class T, class Alloc>
    bool operator != (const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs) {
        return !(lhs == rhs);
    }

}



#endif