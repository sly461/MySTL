/**
 * hashtable
**/
#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <utility>
#include "allocator.hpp"
#include "iterator.hpp"
#include "vector.hpp"

namespace MySTL {
    /*****************************************************************************************
     * hashtable的节点
    *****************************************************************************************/
    template<class Value>
    struct __hashtable_node {
        Value val;
        __hashtable_node* next;
    };

    /*****************************************************************************************
     * 前置声明
     * Value:节点的实值型别 Key:节点的键值型别 HashFcn:hash function的函数型别
     * ExtractKey:从节点中取出键值的方法 EqualKey:判断键值相同与否的方法 Alloc:空间配置器
    *****************************************************************************************/
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc=allocator>
    class hashtable;
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, class Alloc>
    class __hashtable_iterator;
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, class Alloc>
    class __hashtable_const_iterator;

    /*****************************************************************************************
     * hashtable的迭代器 
     * __hashtable_iterator
     * __hashtable_const_iterator
    *****************************************************************************************/
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, class Alloc>
    struct __hashtable_iterator {
        using hashtable = hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
        using iterator = __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
        using const_iterator = __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
        using node = __hashtable_node<Value>;

        using iterator_category = forward_iterator_tag;
        using value_type = Value;
        using difference_type = ptrdiff_t;
        using size_type = size_t;
        using reference = Value&;  
        using pointer = Value*;

        node *cur;
        hashtable *ht;

        __hashtable_iterator(node* n, hashtable* table): cur(n), ht(table) {}
        __hashtable_iterator() {}
        reference operator*() const { return cur->val; }
        pointer operator->() const { return &(operator*()); }
        iterator& operator++();
        iterator operator++(int);
        bool operator==(const iterator & it) const { return cur==it.cur; }
        bool operator!=(const iterator & it) const { return cur!=it.cur; }
    };
    //operator++实现
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, class Alloc>
    __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>& 
    __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>:: operator++() {
        const node* old = cur;
        cur = cur->next;
        if(!cur) {
            //定位old在哪个bucket
            size_type bucket = ht->bkt_num(old->val);
            while(!cur && ++bucket < ht->buckets.size()) {
                cur = ht->buckets[bucket];
            }
        }
        return *this;
    }
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, class Alloc>
    __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> 
    __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>:: operator++(int) {
        iterator tmp = this;
        ++*this;
        return tmp;
    }

    //__hashtable_const_iterator
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, class Alloc>
    struct __hashtable_const_iterator {
        using hashtable = hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
        using iterator = __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
        using const_iterator = __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
        using node = __hashtable_node<Value>;

        using iterator_category = forward_iterator_tag;
        using value_type = Value;
        using difference_type = ptrdiff_t;
        using size_type = size_t;
        using reference = const Value&;  
        using pointer = const Value*;

        const node *cur;
        const hashtable *ht;

        __hashtable_const_iterator(const node* n, const hashtable* table): cur(n), ht(table) {}
        __hashtable_const_iterator() {}
        __hashtable_const_iterator(const iterator& it): cur(it.cur), ht(it.ht) {}
        reference operator*() const { return cur->val; }
        pointer operator->() const { return &(operator*()); }
        const_iterator& operator++();
        const_iterator operator++(int);
        bool operator==(const const_iterator & it) const { return cur==it.cur; }
        bool operator!=(const const_iterator & it) const { return cur!=it.cur; }
    };
    //operator++实现
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, class Alloc>
    __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>& 
    __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>:: operator++() {
        const node* old = cur;
        cur = cur->next;
        if(!cur) {
            //定位old在哪个bucket
            size_type bucket = ht->bkt_num(old->val);
            while(!cur && ++bucket < ht->buckets.size()) {
                cur = ht->buckets[bucket];
            }
        }
        return *this;
    }
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, class Alloc>
    __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> 
    __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>:: operator++(int) {
        const_iterator tmp = this;
        ++*this;
        return tmp;
    }
    /*****************************************************************************************
     * hashtable
    *****************************************************************************************/
   //提供 28 个质数，用质数大小来设计 buckets
    static const int __stl_num_primes = 28;
    static const unsigned long __stl_prime_list[__stl_num_primes] = {
        53ul,         97ul,         193ul,       389ul,       769ul,
        1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
        49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
        1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
        50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
        1610612741ul, 3221225473ul, 4294967291ul
    };
    //找出上述28个质数中最接近并大等于n的那个质数
    unsigned long __stl_next_prime(unsigned long n) {
        //遍历查找 可用二分查找优化
        for(auto i: __stl_prime_list) {
            if(i >= n) return i;
        }
        //28个质数都比n小
        return __stl_prime_list[__stl_num_primes-1];
    }
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc=allocator>
    class hashtable {
    public:
        using key_type = Key;
        using value_type = Value;
        using hasher = HashFcn;
        using key_equal = EqualKey;

        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;

    private:
        using node = __hashtable_node<Value>;
        //节点的空间配置器
        using node_allocator = Alloc<node>;
    
        hasher hash;
        key_equal equals;
        ExtractKey get_key;
        vector<node*> buckets;
        size_type num_elements;
    public:
        using iterator = __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
        using const_iterator = __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;

        //构造函数
        hashtable(size_type n, const HashFcn& hf, const EqualKey& eql, const ExtractKey& ext)
            : hash(hf), equals(eql), get_key(ext) ,num_elements(0)
        {
            initialize_buckets(n);
        }
        hashtable(size_type n, const HashFcn& hf, const EqualKey& eql)
            : hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0)
        {
            initialize_buckets(n);
        }
        hashtable(const hashtable& ht)
            : hash(ht.hash), equals(ht.equals), get_key(ht.get_key), num_elements(0)
        {
            copy_from(ht);
        }
        //赋值函数
        hashtable& operator=(const hashtable& ht) {
            if(&ht != this) {
                clear();
                hash = ht.hash;
                equals = ht.equals;
                get_key = ht.get_key;
                copy_from(ht);
            }
            return *this;
        }
        //析构函数
        ~hashtable() { clear(); }

        //几个size相关函数
        size_type size() { return num_elements; }
        size_type max_size() { return size_type(-1); }
        bool empty() { return size()==0; }
        //与bucket相关
        size_type bucket_count() const { return buckets.size(); }
        size_type max_bucket_count() const { return __stl_prime_list[__stl_num_primes-1]; }
        size_type elems_in_bucket(size_type bucket) const;

        iterator begin();
        iterator end() { return iterator(nullptr, this); }
        const_iterator begin() const();
        const_iterator end() const() { return const_iterator(nullptr, this); }
        void swap(hashtable& ht);

        //插入相关
        //插入元素，不允许重复
        std::pair<iterator, bool> insert_unique(const value_type& obj);
        //插入元素，允许重复
        iterator insert_equal(const value_type& obj);
        //供以上两个函数使用
        std::pair<iterator, bool> insert_unique_noresize(const value_type& obj);
        iterator insert_equal_noresize(const value_type& obj);
        //插入序列
        template<class InputIterator>
        void insert_unique(InputIterator first, InputIterator last) { insert_unique(first, last, iterator_category(first)); }
        template<class InputIterator>
        void insert_equal(InputIterator first, InputIterator last) { insert_equal(first, last, iterator_category(first)); }
        //InputIterator类型
        template<class InputIterator>
        void insert_unique(InputIterator first, InputIterator last, input_iterator_tag);
        template<class InputIterator>
        void insert_equal(InputIterator first, InputIterator last, input_iterator_tag);
        //ForwardIterator类型及以上
        template<class ForwardIterator>
        void insert_unique(ForwardIterator first, ForwardIterator last, forward_iterator_tag);
        template<class ForwardIterator>
        void insert_equal(ForwardIterator first, ForwardIterator last, forward_iterator_tag);
        
        //查找相关
        iterator find(const key_type& key);
        const_iterator find(const key_type& key) const;
        

        void clear();
        //表格重建 扩充bucket
        void resize(size_type num_elements_hint);

    private:
        void initialize_buckets(size_type n);
        //hashtable由vector和linked-list组成，因此复制需要考虑内存相关问题
        void copy_from(const hashtable& ht);
        
        
    };
}








#endif