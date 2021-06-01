/**
 * unordered_set容器（底层是hashtable）
**/
#ifndef _UNORDERED_SET_H_
#define _UNORDERED_SET_H_

#include <functional>
#include "allocator.hpp"
#include "hashtable.hpp"

namespace MySTL {
    template<class Value, class HashFcn=std::hash<Value>,
             class EqualKey=std::equal_to<Value>, template<class T> class Alloc=allocator>
    class unordered_set {
    private:
        //默认的ExtractKey函数是identity，因为unordered_set的实值（value）即为键值（key）
        using ht = hashtable<Value, Value, HashFcn, std::_Identity<Value>, EqualKey, Alloc>;
        
        ht rep;
    public:
        using key_type = typename ht::key_type;
        using value_type = typename ht::value_type;
        using hasher = typename ht::hasher;
        using key_equal = typename ht::key_equal;
        
        using size_type = typename ht::size_type;
        using difference_type = typename ht::difference_type;
        using pointer = typename ht::const_pointer;
        using const_pointer = typename ht::const_pointer;
        using reference = typename ht::const_reference;
        using const_reference = typename ht::const_reference;

        using iterator = typename ht::const_iterator;
        using const_iterator = typename ht::const_iterator;

        hasher hash_funct() const { return rep.hash_funct(); }
        key_equal key_eq() const { return rep.key_eq(); }

        //构造函数 默认使用大小100的表格，hashtable将自动调整为最接近且较大的质数
        unordered_set(): rep(100, hasher(), key_equal()) {}
        explicit unordered_set(size_type n): rep(n, hasher(), key_equal()) {}
        unordered_set(size_type n, const hasher& hf): rep(n, hf, key_equal()) {}
        unordered_set(size_type n, const hasher& hf, const key_equal& eql): rep(n, hf, eql) {}
        //如下的插入序列的操作只能用insert_unique
        template<class InputIterator>
        unordered_set(InputIterator first, InputIterator last)
            : rep(100, hasher(), key_equal()) { rep.insert_unique(first, last); }
        template<class InputIterator>
        unordered_set(InputIterator first, InputIterator last, size_type n)
            : rep(n, hasher(), key_equal()) { rep.insert_unique(first, last); }
        template<class InputIterator>
        unordered_set(InputIterator first, InputIterator last, size_type n, const hasher& hf)
            : rep(n, hf, key_equal()) { rep.insert_unique(first, last); }
        template<class InputIterator>
        unordered_set(InputIterator first, InputIterator last, size_type n, const hasher& hf, const key_equal& eql)
            : rep(n, hf, eql) { rep.insert_unique(first, last); }

        unordered_set(const unordered_set& x): rep(x.rep) {}
        //赋值操作
        unordered_set& operator=(const unordered_set& x) {
            rep = x.rep;
            return *this;
        }

        //几个size相关函数
        size_type size() const { return rep.size(); }
        size_type max_size() const { return rep.max_size(); }
        bool empty() const { return rep.empty(); }
        
        //与bucket相关
        size_type bucket_count() const { return rep.bucket_count(); }
        size_type max_bucket_count() const { return rep.max_bucket_count(); }
        size_type elems_in_bucket(size_type bucket) const { return rep.elems_in_bucket(bucket); }

        void swap(unordered_set& us) { rep.swap(us.rep); }
        void clear() { rep.clear(); }
        void resize(size_type num_elements_hint) { rep.resize(num_elements_hint); }

        //begin end
        const_iterator begin() const { return rep.begin(); }
        const_iterator end() const { return rep.end(); }
        
        //插入操作 不允许键值重复
        std::pair<iterator, bool> insert(const value_type& obj) {
            //注意 unordered_set::iterator是ht::const_iterator而不是ht::iterator
            std::pair<typename ht::iterator, bool> p = rep.insert_unique(obj);
            return std::pair<iterator, bool>(p.first, p.second);
        }
        template<class InputIterator>
        void insert(InputIterator first, InputIterator last) { rep.insert_unique(first, last); }
        //插入前 不进行表格重建（扩充bucket）的判断
        std::pair<iterator, bool> insert_noresize(const value_type& obj) {
            std::pair<typename ht::iterator, bool> p = rep.insert_unique_noresize(obj);
            return std::pair<iterator, bool>(p.first, p.second);
        }

        //查找相关
        iterator find(const key_type& key) const { return rep.find(key); }
        size_type count(const key_type& key) const { return rep.count(key); }
        std::pair<iterator, iterator> equal_range(const key_type& key) const { return rep.equal_range(key); }

        //erase相关
        size_type erase(const key_type& key) { return rep.erase(key); }
        void erase(iterator it) { rep.erase(it); }
        void erase(iterator first, iterator last) { rep.erase(first, last); }

        //声明友元 operator==
        template<class Vl, class HF,
                 class Eq, template<class t> class Al>
        friend bool operator== (const unordered_set<Vl, HF, Eq, Al>& ht1,
                                const unordered_set<Vl, HF, Eq, Al>& ht2);
    };

    /*****************************************************************************************
     * operator== operator!=
    *****************************************************************************************/
    template<class Vl, class HF,
             class Eq, template<class t> class Al>
    bool operator== (const unordered_set<Vl, HF, Eq, Al>& us1,
                     const unordered_set<Vl, HF, Eq, Al>& us2) {
        return us1.rep == us2.rep;                     
    }
    template<class Vl, class HF,
             class Eq, template<class t> class Al>
    bool operator!= (const unordered_set<Vl, HF, Eq, Al>& us1,
                     const unordered_set<Vl, HF, Eq, Al>& us2) {
        return !(us1==us2);              
    }
}



#endif