/**
 * set 基于AVL树
**/
#ifndef _SET_H_
#define _SET_H_


#include <functional>

#include "AVLTree.hpp"
#include "allocator.hpp"

namespace MySTL {
    template<class Key, class Compare=std::less<Key>, template<class T> class Alloc=allocator>
    class set {
    public:
        //alias
        using key_type = Key;
        using value_type = Key;
        using key_compare = Compare;
        using value_compare = Compare;

    private:
        using rep_type = AVLTree<key_type, value_type, std::_Identity<value_type>, key_compare, Alloc>;
        rep_type t;

    public:
        using pointer = typename rep_type::const_pointer;
        using const_pointer = typename rep_type::const_pointer;
        using reference = typename rep_type::const_reference;
        using const_reference = typename rep_type::const_reference;
        //注意此处都定义为const_iterator，意为迭代器无法执行写入操作，不允许用户在任意处进行写入操作
        using iterator = typename rep_type::const_iterator;
        using const_iterator = typename rep_type::const_iterator;
        using size_type = typename rep_type::size_type;
        using difference_type = typename rep_type::difference_type;

        //构造函数
        set(): t(Compare()) {}
        explicit set(const Compare& comp): t(comp) {}
        template<class InputIterator>
        set(InputIterator first, InputIterator last): t(Compare()) { t.insert_unique(first, last); }
        template<class InputIterator>
        set(InputIterator first, InputIterator last, const Compare& comp): t(comp) { t.insert_unique(first, last); }

        set(const set& x): t(x.t) {}
        //赋值操作符
        set& operator=(const set& x) {
            t = x.t;
            return *this;
        }

        key_compare key_comp() const { return t.key_comp(); }
        value_compare value_comp() const { return t.key_comp(); }
        iterator begin() const { return t.begin(); }
        iterator end() const { return t.end(); }
        bool empty() const { return t.empty(); }
        size_type size() const { return t.size(); }
        size_type max_size() const { return t.max_size(); }
        void swap(set& x) { t.swap(x); }
        
        //insert
        void insert(const value_type& x) { t.insert_unique(x); }
        template<class InputIterator>
        void insert(InputIterator first, InputIterator last) { t.insert_unique(first, last); }
        //erase
        void erase(const key_type& x) { t.erase(x); }
        void erase(iterator pos) { 
            using rep_iterator = typename rep_type::iterator;
            t.erase((rep_iterator&)pos);
        }
        void erase(iterator first, iterator last) {
            using rep_iterator = typename rep_type::iterator;
            t.erase((rep_iterator&)first, (rep_iterator&)last);
        }
        void clear() { t.clear(); }

        //查找相关
        iterator find(const key_type& x) const { return t.find(x); }
        size_type count(const key_type& x) const { return t.count(x); }
        iterator lower_bound(const key_type& x) const { return t.lower_bound(x); }
        iterator upper_bound(const key_type& x) const { return t.upper_bound(x); }
        std::pair<iterator, iterator> equal_range(const key_type& x) const { return t.equal_range(x); }
        
        //友元
        template<class Ky, class Cp, template<class t> class Al>
        friend bool operator==(const set<Ky, Cp, Al>& x, const set<Ky, Cp, Al>& y);
    };
    //operator== != <
    template<class Ky, class Cp, template<class t> class Al>
    bool operator==(const set<Ky, Cp, Al>& x, const set<Ky, Cp, Al>& y) {
        return x.t==y.t;        
    }
    template<class Ky, class Cp, template<class t> class Al>
    bool operator!=(const set<Ky, Cp, Al>& x, const set<Ky, Cp, Al>& y) {
        return !(x==y);
    }
}

#endif