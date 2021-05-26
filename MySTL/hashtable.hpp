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
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    struct __hashtable_iterator;
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    struct __hashtable_const_iterator;

    /*****************************************************************************************
     * hashtable的迭代器 
     * __hashtable_iterator
     * __hashtable_const_iterator
    *****************************************************************************************/
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    struct __hashtable_iterator {
        using _Hashtable = hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
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
        _Hashtable *ht;

        __hashtable_iterator(node* n, _Hashtable* table): cur(n), ht(table) {}
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
             class ExtractKey, class EqualKey, template<class T> class Alloc>
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
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> 
    __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>:: operator++(int) {
        iterator tmp = *this;
        ++*this;
        return tmp;
    }

    //__hashtable_const_iterator
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    struct __hashtable_const_iterator {
        using _Hashtable = hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
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
        const _Hashtable *ht;

        __hashtable_const_iterator(const node* n, const _Hashtable* table): cur(n), ht(table) {}
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
             class ExtractKey, class EqualKey, template<class T> class Alloc>
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
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> 
    __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>:: operator++(int) {
        const_iterator tmp = *this;
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
             class ExtractKey, class EqualKey, template<class T> class Alloc>
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

        hasher hash_funct() const { return hash; }
        key_equal key_eq() const { return equals; }

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

        //声明友元 iterator::operator++中使用了bkt_num函数 
        friend iterator;
        friend const_iterator;


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
        size_type size() const { return num_elements; }
        size_type max_size() const { return size_type(-1); }
        bool empty() const { return size()==0; }

        //与bucket相关
        size_type bucket_count() const { return buckets.size(); }
        size_type max_bucket_count() const { return __stl_prime_list[__stl_num_primes-1]; }
        size_type elems_in_bucket(size_type bucket) const;

        void swap(hashtable& ht);
        void clear();
        //判断是否需要表格重建 扩充bucket
        //判断原则：将元素个数（把新增元素计入后）与bucket vector的大小相比较
        //若前者大于后者，则重建。因此，每个bucket list的最大size和vecotr的大小相同
        void resize(size_type num_elements_hint);

        iterator begin();
        iterator end() { return iterator(nullptr, this); }
        const_iterator begin() const;
        const_iterator end() const { return const_iterator(nullptr, this); }

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
        size_type count(const key_type& key) const;
        std::pair<iterator, iterator> equal_range(const key_type& key);
        std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const;

        //erase相关
        size_type erase(const key_type& key);
        void erase(const iterator& it);
        void erase(iterator first, iterator last);
        void erase(const const_iterator& it);
        void erase(const_iterator first, const_iterator last);

        //声明友元 operator== 注意此处template不能省略且模板参数名字不能和hashtable类相同
        template<class Vl, class Ky, class HF,
                 class Ex, class Eq, template<class t> class Al>
        friend bool operator== (const hashtable<Vl, Ky, HF, Ex, Eq, Al>& ht1,
                                const hashtable<Vl, Ky, HF, Ex, Eq, Al>& ht2);
        
    private:
        //节点配置与释放
        node * new_node(const value_type& obj);
        void delete_node(node* n);
        //返回最接近n且大等于n的质数
        size_type next_size(size_type n) const { return __stl_next_prime(n); }
        void initialize_buckets(size_type n);
        //hashtable由vector和linked-list组成，因此复制需要考虑内存相关问题
        void copy_from(const hashtable& ht);
        
        //判断元素的落脚处
        //版本1：接受value和buckets个数
        size_type bkt_num(const value_type& obj, size_type n) const 
        { return bkt_num_key(get_key(obj), n);  }
        //版本2：接受value
        size_type bkt_num(const value_type& obj) const
        { return bkt_num_key(get_key(obj)); }
        //版本3：接受key
        size_type bkt_num_key(const key_type& key) const
        { return bkt_num_key(key, buckets.size()); }
        //版本4：接受key和buckets个数
        size_type bkt_num_key(const key_type& key, size_type n) const
        { return hash(key)%n; }
        
        //erase辅助函数 erase掉指定bucket下 [first, last)范围内节点
        void erase_bucket(const size_type n, node* first, node* last);
    };

    /*****************************************************************************************
     * 一些辅助函数的具体实现
    *****************************************************************************************/
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::node*
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::new_node(const value_type& obj) {
        node* n = node_allocator::allocate();
        n->next = nullptr;
        try {
            construct(&n->val, obj);
            return n;
        }
        catch(...) {
            // "commit or rollback"
            node_allocator::deallocate(n);
            throw;
        }
    }
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::delete_node(node* n) {
        //销毁对象 释放内存
        destroy(&n->val);
        node_allocator::deallocate(n);
    }
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::initialize_buckets(size_type n) {
        const size_type n_buckets = next_size(n);
        buckets.reserve(n_buckets);
        buckets.insert(buckets.end(), n_buckets, nullptr);
        num_elements = 0;
    }
    //深拷贝
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::copy_from(const hashtable& ht) {
        // (注意：调用此函数之前，已经调用hashtable::clear函数将bucket list上的节点销毁并释放内存，同时所有buckets[i]=nullptr)
        // 清除掉己方的buckets，调用vector::clear
        buckets.clear();
        // 若己方的空间大于对方，就不动，若己方空间小于对方，则增大
        size_type new_size = ht->buckets.size();
        buckets.reserve(new_size);
        // 初始化n个元素为null指针
        buckets.insert(buckets.end(), new_size, nullptr);
        try {
            for(size_type i=0; i<new_size; i++) {
                //深度拷贝每个bucket list的每个节点
                if(const node* cur = ht.buckets[i]) {
                    node* copy = new_node(cur->val);
                    buckets[i] = copy;
                    for(node* next=cur->next; next; cur=next,next=cur->next) {
                        copy->next = new_node(next->val);
                        copy = copy->next;
                    }
                }
            }
            num_elements = ht.num_elements;
        }
        catch(...) {
            // commit or rollback
            clear();
            throw;
        }
    }
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase_bucket(const size_type n, node* first, node* last) {
        //添加一个辅助头节点
        node * head = new_node(Value());
        head->next = buckets[n];
        node * cur = head;
        node * next = cur->next;
        //找到first
        for(next = cur->next; next!=first; cur=next, next=cur->next) ;
        while(next != last) {
            cur->next = next->next;
            delete_node(next);
            next = cur->next;
            num_elements--;
        }
        buckets[n] = head->next;
        //销毁并释放掉辅助头节点
        delete_node(head);
    }

    /*****************************************************************************************
     * 与bucket相关，swap，clear，判断是否需要表格重建、扩充bucket
    *****************************************************************************************/
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::swap(hashtable& ht) {
        MySTL::swap(hash, ht.hash);
        MySTL::swap(equals, ht.equals);
        MySTL::swap(get_key, ht.get_key);
        buckets.swap(ht.buckets);
        MySTL::swap(num_elements, ht.num_elements);
    }
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type 
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::elems_in_bucket(size_type bucket) const {
        size_type result = 0;
        for(node* cur=buckets[bucket]; cur; cur=cur->next)
            result++;
        return result;
    }
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::clear() {
        for(size_type i=0; i<buckets.size(); i++) {
            node* cur = buckets[i];
            while(cur) {
                node *next = cur->next;
                delete_node(cur);
                cur = next;
            }
            buckets[i] = nullptr;
        }
        num_elements = 0;
    }
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::resize(size_type num_elements_hint) {
        const size_type old_n = buckets.size();
        if(num_elements_hint <= old_n) return;
        //需要重新配置
        const size_type n = next_size(num_elements_hint);
        if(n <= old_n) return;
        vector<node *> tmp(n, nullptr);
        try {
            for(size_type bucket=0; bucket<old_n; bucket++) {
                node * first = buckets[bucket];
                while(first) {
                    size_type new_bucket = bkt_num(first->val, n);
                    buckets[bucket] = first->next;
                    first->next = tmp[new_bucket];
                    tmp[new_bucket] = first;
                    first = buckets[bucket];
                }
            }
            //新旧对调
            buckets.swap(tmp);
        }
        catch(...) {
            //commit or callback
            for(size_type bucket=0; bucket<tmp.size(); bucket++) {
                while(tmp[bucket]) {
                    node *next = tmp[bucket]->next;
                    delete_node(tmp[bucket]);
                    tmp[bucket] = next;
                }
            }
            throw;
        }
    }

    /*****************************************************************************************
     * begin()函数
    *****************************************************************************************/
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::begin() {
        for(size_type n=0; n<buckets.size(); n++) {
            if(buckets[n]) return iterator(buckets[n], this);
        }
        return end();
    }
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator 
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::begin() const {
        for(size_type n=0; n<buckets.size(); n++) {
            if(buckets[n]) return const_iterator(buckets[n], this);
        }
        return end();
    }

    /*****************************************************************************************
     * 插入相关
    *****************************************************************************************/
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    std::pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool> 
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique(const value_type& obj) {
        //判断新加入一个元素后是否需要重建表格
        resize(num_elements+1);
        insert_unique_noresize(obj);
    }
    //供insert_unique使用
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    std::pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool> 
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique_noresize(const value_type& obj) {
        const size_type n = bkt_num(obj);
        node * first = buckets[n];
        for(node* cur=first; cur; cur=cur->next) {
            // 判断hashtable中是否已存在相同的键值
            if(equals(get_key(cur->val), get_key(obj)))
                return std::pair<iterator, bool>(iterator(cur, this), false);
        }
        //头插法
        node * tmp = new_node(obj);
        tmp->next = first;
        buckets[n] = tmp;
        num_elements++;
        return std::pair<iterator, bool>(iterator(tmp, this), true);
    }
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal(const value_type& obj) {
        resize(num_elements+1);
        insert_equal_noresize(obj);
    }
    //供insert_equal使用
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal_noresize(const value_type& obj) {
        const size_type n = bkt_num(obj);
        node * first = buckets[n];
        for(node* cur=first; cur; cur=cur->next) {
            // 键值同，插入后面
            if(equals(get_key(cur->val), get_key(obj))) {
                node * tmp = new_node(obj);
                tmp->next = cur->next;
                cur->next = tmp;
                num_elements++;
                return iterator(tmp, this);
            }
        }
        //头插法
        node * tmp = new_node(obj);
        tmp->next = first;
        buckets[n] = tmp;
        num_elements++;
        return iterator(tmp, this);
    }
    
    //插入序列
    //InputIterator类型
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    template<class InputIterator>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique(InputIterator first, InputIterator last, input_iterator_tag) {
        for( ; first!=last; first++)
            insert_unique(*first);
    }
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    template<class InputIterator>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal(InputIterator first, InputIterator last, input_iterator_tag) {
        for( ; first!=last; first++)
            insert_equal(*first);
    }
    //ForwardIterator类型及以上
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    template<class ForwardIterator>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique(ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
        size_type n = distance(first, last);
        resize(num_elements+n);
        for( ; n>0; n--,first++)
            insert_unique_noresize(*first);
    }
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    template<class ForwardIterator>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal(ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
        size_type n = distance(first, last);
        resize(num_elements+n);
        for( ; n>0; n--,first++)
            insert_equal_noresize(*first);
    }

    /*****************************************************************************************
     * 查找相关
    *****************************************************************************************/
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::find(const key_type& key) {
        size_type n = bkt_num_key(key);
        node * first;
        for(first=buckets[n]; first&&!equals(key, get_key(first->val)); first=first->next) ;
        //若未找到，first=nullptr
        return iterator(first, this);
    }
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::find(const key_type& key) const {
        size_type n = bkt_num_key(key);
        const node * first;
        for(first=buckets[n]; first&&!equals(key, get_key(first->val)); first=first->next) ;
        return const_iterator(first, this);
    }
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::count(const key_type& key) const {
        const size_type n = bkt_num_key(key);
        size_type result = 0;
        for(const node* cur=buckets[n]; cur; cur=cur->next) {
            if(equals(key, get_key(cur->val)))
                result++;
        }
        return result;
    }
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    std::pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, 
              typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator> 
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::equal_range(const key_type& key) {
        using Pii = std::pair<iterator, iterator>;
        const size_type n = bkt_num_key(key);
        for(node* first=buckets[n]; first; first=first->next) {
            //键值相同的节点是相连在一起的
            if(equals(key, get_key(first->val))) {
                for(node* cur=first->next; cur; cur=cur->next) {
                    if(!equals(key, get_key(cur->val)))
                        return Pii(iterator(first, this), iterator(cur, this));
                }
                //尾后节点
                for(size_type m=n+1; m<buckets.size(); m++) {
                    if(buckets[m]) {
                        return Pii(iterator(first, this), iterator(buckets[m], this));
                    }
                }
                return Pii(iterator(first, this), end());
            }
        }
        return Pii(end(), end());
    }
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    std::pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator, 
              typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator> 
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::equal_range(const key_type& key) const {
        using Pii = std::pair<const_iterator, const_iterator>;
        const size_type n = bkt_num_key(key);
        for(const node* first=buckets[n]; first; first=first->next) {
            //键值相同的节点是相连在一起的
            if(equals(key, get_key(first->val))) {
                for(const node* cur=first->next; cur; cur=cur->next) {
                    if(!equals(key, get_key(cur->val)))
                        return Pii(const_iterator(first, this), const_iterator(cur, this));
                }
                //尾后节点
                for(size_type m=n+1; m<buckets.size(); m++) {
                    if(buckets[m]) {
                        return Pii(const_iterator(first, this), const_iterator(buckets[m], this));
                    }
                }
                return Pii(const_iterator(first, this), end());
            }
        }
        return Pii(end(), end());
    }

    /*****************************************************************************************
     * erase相关
    *****************************************************************************************/
    //删除指定节点（键值不唯一时可能会删除多个节点）
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const key_type& key) {
        const size_type n = bkt_num_key(key);
        node * first = buckets[n];
        size_type erased = 0;
        if(first) {
            node * cur = first;
            node * next = cur->next;
            while(next) {
                if(equals(key, get_key(next->val))) {
                    cur->next = next->next;
                    delete_node(next);
                    next = cur->next;
                    erased++;
                    num_elements--;
                }
                else {
                    cur = next;
                    next = cur->next;
                }
            }
            if(equals(key, get_key(first->val))) {
                buckets[n] = first->next;
                delete_node(first);
                erased++;
                num_elements--;
            }
        }
        return erased;
    }
    //删除一个节点
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const iterator& it) {
        node *p = it.cur;
        if(p) {
            const size_type n = bkt_num(p->val);
            node * cur = buckets[n];
            if(cur == p) {
                buckets[n] = cur->next;
                delete_node(cur);
                num_elements--;
            }
            else {
                node * next = cur->next;
                while(next) {
                    if(next == p) {
                        cur->next = next->next;
                        delete_node(next);
                        num_elements--;
                        break;
                    }
                    else {
                        cur = next;
                        next = cur->next;
                    }
                }
            }
        }
    }
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const const_iterator& it) {
        erase(iterator(const_cast<node*>(it.cur), const_cast<hashtable*>(it.ht)));
    }
    //删除序列
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(iterator first, iterator last) {
        size_type first_bucket = first.cur ? bkt_num(first.cur->val) : buckets.size();
        size_type last_bucket = last.cur ? bkt_num(last.cur->val) : buckets.size();
        if(first.cur == last.cur) return;
        else if(first_bucket == last_bucket)
            erase_bucket(first_bucket, first.cur, last.cur);
        else {
            erase_bucket(first_bucket, first.cur, nullptr);
            for(size_type n=first_bucket+1; n<last_bucket; n++)
                erase_bucket(n, buckets[n], nullptr);
            if(last_bucket != buckets.size())
                erase_bucket(last_bucket, buckets[last_bucket], last.cur);
        }
    }
    template<class Value, class Key, class HashFcn,
             class ExtractKey, class EqualKey, template<class T> class Alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const_iterator first, const_iterator last) {
        erase(iterator(const_cast<node*>(first.cur), const_cast<hashtable*>(first.ht)),
              iterator(const_cast<node*>(last.cur), const_cast<hashtable*>(last.ht)));
    }
    /*****************************************************************************************
     * operator== operator!=
    *****************************************************************************************/
    template<class Vl, class Ky, class HF,
             class Ex, class Eq, template<class t> class Al>
    bool operator== (const hashtable<Vl, Ky, HF, Ex, Eq, Al>& ht1,
                     const hashtable<Vl, Ky, HF, Ex, Eq, Al>& ht2) {
        using node  = typename hashtable<Vl, Ky, HF, Ex, Eq, Al>::node;
        if(ht1.buckets.size() != ht2.buckets.size()) return false;
        for(int n=0; n<ht1.buckets.size(); n++) {
            node * cur1 = ht1.buckets[n];
            node * cur2 = ht2.buckets[n];
            for( ; cur1&&cur2&&cur1->val==cur2->val; cur1=cur1->next, cur2=cur2->next) ;
            if(cur1 || cur2) return false;
        }
        return true;
    }
    template<class Vl, class Ky, class HF,
             class Ex, class Eq, template<class t> class Al>
    bool operator!= (const hashtable<Vl, Ky, HF, Ex, Eq, Al>& ht1,
                     const hashtable<Vl, Ky, HF, Ex, Eq, Al>& ht2) {
        return !(ht1==ht2);
    }                

}




#endif