/**
 * hashtable
**/
#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include "allocator.hpp"
#include "iterator.hpp"

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
             class ExtractKey, class EqualKey, class Alloc>
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
             class ExtractKey, class EqualKey, class Alloc>
    class hashtable {
    public:

    private:
        
    };
}



#endif