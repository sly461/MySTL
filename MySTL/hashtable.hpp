/**
 * hashtable
**/
#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include "allocator.hpp"
#include "iterator.hpp"

namespace MySTL {
    //hashtable的节点
    template<class Value>
    struct __hashtable_node {
        Value val;
        __hashtable_node* next;
    };

    //前置声明
    //Value:节点的实值型别 Key:节点的键值型别 HashFcn:hash function的函数型别
    //ExtractKey:从节点中取出键值的方法 EqualKey:判断键值相同与否的方法 Alloc:空间配置器
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, class Alloc>
    class hashtable;
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, class Alloc>
    class __hashtable_iterator;
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, class Alloc>
    class __hashtable_const_iterator;

    //hashtable的迭代器 
    //Value:节点的实值型别 Key:节点的键值型别 HashFcn:hash function的函数型别
    //ExtractKey:从节点中取出键值的方法 EqualKey:判断键值相同与否的方法 Alloc:空间配置器
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

    

}



#endif