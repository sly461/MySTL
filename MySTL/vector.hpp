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
        //push_back调用 insert(pos, value)调用 相当于push_back() 是 insert() 函数的一个特例
        void insert_aux(iterator position, const T& value);
        //insert(pos, n, value)调用
        void fill_insert(iterator position, size_type n, const T& value);
        //insert(pos, first, last)调用
        template<class InputIterator>
        void range_insert(iterator position, InputIterator first, InputIterator last, input_iterator_tag);
        //insert(pos, first, last)调用
        template<class ForwardIterator>
        void range_insert(iterator position, ForwardIterator first, ForwardIterator last, forward_iterator_tag);

    public:
        //相关构造函数及析构函数及赋值运算符
        vector(): start(nullptr), finish(nullptr), end_of_storage(nullptr) {}
        vector(size_type n, const T& value) { fill_initialize(n, value); }
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
        void swap(vector& rhs);
        iterator insert(iterator position, const value_type& value);
        void insert(iterator position, size_type n, const value_type& value);
        template<class InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last);


        //与容量相关
        difference_type size() const { return finish-start; }
        difference_type capacity() const { return end_of_storage-start; }
        bool empty() const { return start==finish; }
        void resize(size_type new_size, const value_type & value = value_type());
        void reserve(size_type n);
        //放弃多余空间 即finish到end_of_storage的空间
        //减小容器的容量以适应其大小，并破坏超出该容量size的所有元素。
        void shrink_to_fit();
        

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
        //但是注意如果finish != end_of_storage && position == end()时候 使用copy_backward函数会出错
        //因此种情况直接在末尾构造一个元素即可 具体见insert(iterator position, const value_type& value)
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
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator position) {
        //如果position后面还有元素，需要拷贝;如果position是最后一个元素，则后面没有元素，直接destroy即可
        if(position+1 != end()) {
            copy(position+1, end(), position);
        }
        --finish;
        destroy(finish);
        return position;
    }
    template<class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last) {
        iterator i = copy(last, finish, first);
        destroy(i, finish);
        finish = finish - (last - first);
        return first;
    }
    //vector swap 只是交换三个迭代器
    template<class T, class Alloc>
    void vector<T, Alloc>::swap(vector& rhs) {
        if(this != &rhs) {
            swap(start, rhs.start);
            swap(finish, rhs.finish);
            swap(end_of_storage, rhs.end_of_storage);
        }
    }
    //insert(pos, value)
    template<class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const value_type& value) {
        size_type n = position - begin();
        if(finish != end_of_storage && position == end()) {
            construct(finish, value);
            ++finish;
        }
        else {
            insert_aux(position, value);
        }
        //注意此处为何不能直接返回position?
        //因为可能经过insert_aux函数 原vector的元素可能全部被拷贝到新vector
        //内存地址改变了，原迭代器都失效了
        return begin()+n;
    }
    //供insert(pos, n, value)调用
    template<class T, class Alloc>
    void vector<T, Alloc>::fill_insert(iterator position, size_type n, const T& value) {
        if(n == 0) return;
        //备用空间大等于”新增元素个数“
        if(size_type(end_of_storage-finish) >= n) {
            T value_copy = value;
            // 计算插入点之后的现有元素个数
            const size_type elems_after = finish - position;
            iterator old_finish = finish;
            //因为可能有内存空间重叠的现象出现 因此需要分情况讨论
            // "插入点之后的现有元素" > "新增元素个数"
            if(elems_after > n) {
                uninitialized_copy(finish-n, finish, finish);
                finish += n;
                std::copy_backward(position, old_finish-n, old_finish);
                fill(position, position+n, value_copy);
            }
            // "插入点之后的现有元素" <= "新增元素个数"
            else {
                uninitialized_fill_n(finish, n-elems_after, value_copy);
                finish += n - elems_after;
                uninitialized_copy(position, old_finish, finish);
                finish += elems_after;
                fill(position, old_finish, value_copy);
            }
        }
        //备用空间小于“新增元素个数” 需要重新为vector配置内存空间
        else {
            //空间配置原则：max(原长度的两倍, 原长度+新增元素个数)
            const size_type old_size = size();
            const size_type len = old_size + max(old_size, n);
            //配置新的vector空间
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;
            try{
                //将旧vector中插入点之前的元素复制到新空间
                new_finish = uninitialized_copy(start, position, new_start);
                //新增元素
                new_finish = uninitialized_fill_n(new_finish, n, value);
                //将旧vector中插入点之后的元素复制到新空间
                new_finish = uninitialized_copy(position, finish, new_finish);
            }
            catch(...) {
                // "commit or rollback"
                //销毁对象并释放空间
                destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, len);
                throw;
            }
            // 对旧的vector 销毁对象并释放空间
            destroy(start, finish);
            data_allocator::deallocate(start, capacity());
            //调整迭代器
            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + len;
        }
    }
    //insert(pos, n, value)
    template<class T, class Alloc>
    void vector<T, Alloc>::insert(iterator position, size_type n, const value_type& value) {
        fill_insert(position, n, value);
    }
    //供insert(pos, first, last)调用 InputIterator版本
    template<class T, class Alloc>
    template<class InputIterator>
    void vector<T, Alloc>::range_insert(iterator position, InputIterator first, InputIterator last, input_iterator_tag) {
        for(; first!=last; ++first) {
            position = insert(position, *first);
            ++position;
        }
    }
    //供insert(pos, first, last)调用 ForwardIterator版本
    template<class T, class Alloc>
    template<class ForwardIterator>
    void vector<T, Alloc>::range_insert(iterator position, ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
        if(first == last) return;
        size_type n = distance(first, last);
        //备用空间大等于”新增元素个数“
        if(size_type(end_of_storage-finish) >= n) {
            // 计算插入点之后的现有元素个数
            const size_type elems_after = finish - position;
            iterator old_finish = finish;
            //因为可能有内存空间重叠的现象出现 因此需要分情况讨论
            // "插入点之后的现有元素" > "新增元素个数"
            if(elems_after > n) {
                uninitialized_copy(finish-n, finish, finish);
                finish += n;
                std::copy_backward(position, old_finish-n, old_finish);
                copy(first, last, position);
            }
            // "插入点之后的现有元素" <= "新增元素个数"
            else {
                //前移elems_after个位置
                ForwardIterator mid = first;
                advance(mid, elems_after);
                uninitialized_copy(mid, last, finish);
                finish += n - elems_after;
                uninitialized_copy(position, old_finish, finish);
                finish += elems_after;
                copy(first, mid, position);
            }
        }
        //备用空间小于“新增元素个数” 需要重新为vector配置内存空间
        else {
            //空间配置原则：max(原长度的两倍, 原长度+新增元素个数)
            const size_type old_size = size();
            const size_type len = old_size + max(old_size, n);
            //配置新的vector空间
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;
            try{
                //将旧vector中插入点之前的元素复制到新空间
                new_finish = uninitialized_copy(start, position, new_start);
                //新增元素
                new_finish = uninitialized_copy(first, last, new_finish);
                //将旧vector中插入点之后的元素复制到新空间
                new_finish = uninitialized_copy(position, finish, new_finish);
            }
            catch(...) {
                // "commit or rollback"
                //销毁对象并释放空间
                destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, len);
                throw;
            }
            // 对旧的vector 销毁对象并释放空间
            destroy(start, finish);
            data_allocator::deallocate(start, capacity());
            //调整迭代器
            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + len;
        }
    }
    //insert(pos, first, last)
    template<class T, class Alloc>
    template<class InputIterator>
    void vector<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
        range_insert(position, first, last, iterator_category(first));
    }



    /*****************************************************************************************
     * 具体实现
     * 与容量相关
    *****************************************************************************************/
    template<class T, class Alloc>
    void vector<T, Alloc>::resize(size_type new_size, const value_type & value) {
        if(new_size < size()) {
            erase(begin() + new_size, end());
        }
        else insert(end(), new_size-size(), value);
    }
    //预留存储空间，若 n 大于当前的 capacity() ，则分配新存储，否则该方法不做任何事。
    template<class T, class Alloc>
    void vector<T, Alloc>::reserve(size_type n) {
        if(capacity() >= n) return;
        //重新分配内存空间
        iterator new_start = data_allocator::allocate(n);
        iterator new_finish = new_start;
        try {
            //复制对象
            new_finish = uninitialized_copy(start, finish, new_start);
        }
        catch(...) {
            //"commit or rollback"
            destroy(new_start, new_finish);
            data_allocator::deallocate(new_start, n);
            throw;
        }
        //销毁对象 释放空间
        destroy(start, finish);
        data_allocator::deallocate(start, capacity());
        //调整迭代器
        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + n;
    }
    //减小容器的容量以适应其大小，并破坏超出该容量size的所有元素。“破坏”与“销毁”是两个意思
    template<class T, class Alloc>
    void vector<T, Alloc>::shrink_to_fit() {
        if(finish == end_of_storage) return;
        const size_type old_size = size();
        //以"重新分配内存空间"的方式”破坏“超出size的元素
        iterator new_start = data_allocator::allocate(old_size);
        iterator new_finish = new_start;
        try {
            new_finish = uninitialized_copy(start, finish, new_start);
        }
        catch(...) {
            //"commit or rollback"
            destroy(new_start, new_finish);
            data_allocator::deallocate(new_start, old_size);
            throw;
        }
        //销毁对象 释放空间
        destroy(start, finish);
        data_allocator::deallocate(start, capacity());
        //调整迭代器
        start = new_start;
        finish = new_finish;
        end_of_storage = new_finish;
    }



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