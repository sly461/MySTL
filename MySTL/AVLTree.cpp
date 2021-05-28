/**
 * AVLTree
**/
#ifndef _AVLTREE_H_
#define _AVLTREE_H_

#include "allocator.hpp"

namespace MySTL {
    /*****************************************************************************************
     * AVL树的节点
    *****************************************************************************************/
    template<class Value>
    struct __AVLTree_node {
        Value val;
        size_t height;  //高度
        __AVLTree_node* left;
        __AVLTree_node* right;
        __AVLTree_node* parent;

        //求该子树上的最小值
        static __AVLTree_node* minimum(__AVLTree_node* x) {
            //一直向左走
            while(x->left) x=x->left;
            return x;
        }
        //求该子树上的最大值
        static __AVLTree_node* maximum(__AVLTree_node* x) {
            while(x->right) x=x->right;
            return x;
        }
    };

    /*****************************************************************************************
     * AVL树的迭代器
     * __AVLTree_iterator
     * __AVLTree_const_iterator
    *****************************************************************************************/
    template<class Value>
    struct __AVLTree_iterator {
        using iterator = __AVLTree_iterator<Value>;
        using node = __AVLTree_node<Value>;

        //双向迭代器
        using iterator_category = bidirectional_iterator_tag;
        using value_type = Value;
        using difference_type = ptrdiff_t;
        using size_type = size_t;
        using reference = Value&;  
        using pointer = Value*;

        node *cur;
        //迫不得已添加一个AVL树的header节点
        //为了operator--()中的特殊情况：cur为header时，实在无法判断cur是root节点还是header节点
        node *header;

        __AVLTree_iterator() {}
        __AVLTree_iterator(node* n): cur(n) {}
        __AVLTree_iterator(const iterator& it) { cur=it.cur; }
        reference operator*() const { return cur->val; }
        pointer operator->() const { return &(operator*()); }
        iterator& operator++();
        iterator operator++(int);
        iterator& operator--();
        iterator operator--(int);
        bool operator==(const iterator & it) const { return cur==it.cur; }
        bool operator!=(const iterator & it) const { return cur!=it.cur; }
    };

    //operator++ operator--实现
    template<class Value>
    __AVLTree_iterator<Value>& 
    __AVLTree_iterator<Value>:: operator++() {
        //若有右子节点
        if(cur->right) {
            cur = cur->right;
            //一路向左
            while(nullptr != cur->left)
                cur = cur->left;
        }
        //否则 往上回溯
        else {
            node* y = cur->parent;
            while(y->right == cur) {
                cur = y;
                y = cur->parent;
            }
            //特殊情况：要求根节点(root)的下一个节点，但根节点无右节点（header->right=root，header->parent=root，root->parent=header）
            //程序运行至此，cur=header，y=root，此时cur->right=y，不执行下面语句 因此root节点的下个节点就是header，亦即end()
            if(cur->right!=y) cur = y;
        }
        return *this;
    }
    template<class Value>
    __AVLTree_iterator<Value>
    __AVLTree_iterator<Value>:: operator++(int) {
        iterator tmp = *this;
        ++*this;
        return tmp;
    }
    template<class Value>
    __AVLTree_iterator<Value>& 
    __AVLTree_iterator<Value>:: operator--() {
        //特殊情况：cur为header时
        if(cur == header) {
            cur = cur->right;
        }
        //若有左子节点
        else if(cur->left) {
            cur = cur->left;
            //一路向右
            while(nullptr != cur->right)
                cur = cur->right;
        }
        //否则 往上回溯
        else {
            node* y = cur->parent;
            while(y->left == cur) {
                cur = y;
                y = cur->parent;
            }
            cur = y;
        }
        return *this;
    }
    template<class Value>
    __AVLTree_iterator<Value>
    __AVLTree_iterator<Value>:: operator--(int) {
        iterator tmp = *this;
        --*this;
        return tmp;
    }

    //__AVLTree_const_iterator
    template<class Value>
    struct __AVLTree_const_iterator {
        using const_iterator = __AVLTree_const_iterator<Value>;
        using node = __AVLTree_node<Value>;

        //双向迭代器
        using iterator_category = bidirectional_iterator_tag;
        using value_type = Value;
        using difference_type = ptrdiff_t;
        using size_type = size_t;
        using reference = const Value&;  
        using pointer = const Value*;

        const node *cur;
        //迫不得已添加一个AVL树的header节点
        //为了operator--()中的特殊情况：cur为header时，实在无法判断cur是root节点还是header节点
        const node *header;

        __AVLTree_const_iterator() {}
        __AVLTree_const_iterator(node* n): cur(n) {}
        __AVLTree_const_iterator(const const_iterator& it) { cur=it.cur; }
        reference operator*() const { return cur->val; }
        pointer operator->() const { return &(operator*()); }
        const_iterator& operator++();
        const_iterator operator++(int);
        const_iterator& operator--();
        const_iterator operator--(int);
        bool operator==(const const_iterator & it) const { return cur==it.cur; }
        bool operator!=(const const_iterator & it) const { return cur!=it.cur; }
    };

    //operator++ operator--实现
    //end()为header节点 header节点不可再++
    template<class Value>
    __AVLTree_const_iterator<Value>& 
    __AVLTree_const_iterator<Value>:: operator++() {
        //若有右子节点
        if(cur->right) {
            cur = cur->right;
            //一路向左
            while(nullptr != cur->left)
                cur = cur->left;
        }
        //否则 往上回溯
        else {
            const node* y = cur->parent;
            while(y->right == cur) {
                cur = y;
                y = cur->parent;
            }
            //特殊情况：要求根节点(root)的下一个节点，但根节点无右节点（header->right=root，header->parent=root，root->parent=header）
            //程序运行至此，cur=header，y=root，此时cur->right=y，不执行下面语句 因此root节点的下个节点就是header，亦即end()
            if(cur->right!=y) cur = y;
        }
        return *this;
    }
    template<class Value>
    __AVLTree_const_iterator<Value>
    __AVLTree_const_iterator<Value>:: operator++(int) {
        const_iterator tmp = *this;
        ++*this;
        return tmp;
    }
    //begin()节点为AVL树中最小节点 不可再--
    template<class Value>
    __AVLTree_const_iterator<Value>& 
    __AVLTree_const_iterator<Value>:: operator--() {
        //特殊情况：cur为header时
        if(cur == header) {
            cur = cur->right;
        }
        //若有左子节点
        else if(cur->left) {
            cur = cur->left;
            //一路向右
            while(nullptr != cur->right)
                cur = cur->right;
        }
        //否则 往上回溯
        else {
            const node* y = cur->parent;
            while(y->left == cur) {
                cur = y;
                y = cur->parent;
            }
            cur = y;
        }
        return *this;
    }
    template<class Value>
    __AVLTree_const_iterator<Value>
    __AVLTree_const_iterator<Value>:: operator--(int) {
        const_iterator tmp = *this;
        --*this;
        return tmp;
    }

    /*****************************************************************************************
     * AVLTree
     * Key：键值型别 Valule：实值型别 KeyOfValue：从节点中取出键值的方法
     * Compare:比较键值的方法 Alloc:空间配置器
    *****************************************************************************************/
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc=allocator>
    class AVLTree {
    public:
        using key_type = Key;
        using value_type = Value;

        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;
    protected:
        using node = __AVLTree_node<Value>;
        //节点的空间配置器
        using node_allocator = allocator<node>;

        size_type node_count;
        //关于AVL树中的节点说明：
        //header是AVL树中辅助的头节点，其实值无意义
        //保证其父节点指向根节点root，同时root的父节点也指向header
        //左子节点指向AVL树中最小节点，右子节点指向AVL树中最大节点
        node* header;
        Compare key_compare;

    public:
        using iterator = __AVLTree_iterator<value_type>;
        using const_iterator = __AVLTree_const_iterator<value_type>;
        
        //构造函数
        AVLTree(const Compare& comp=Compare()): node_count(0), key_compare(comp) { init(); }
        AVLTree(const AVLTree& x): node_count(0), key_compare(x.key_compare) {
            if(x.root() == nullptr) init();
            else {
                init();
                root() = copy(x.root(), header);
                leftmost() = minimum(root());
                rightmost() = maximum(root());
            }
            node_count = x.node_count;
        }
        //赋值操作符
        AVLTree& operator=(const AVLTree& x);
        ~AVLTree() { clear(); }

        void clear();
        void swap(AVLTree& t);

        //几个size相关函数
        size_type size() const { return node_count; }
        size_type max_size() const { return size_type(-1); }
        bool empty() const { return node_count==0; }

    protected:
        //节点的构造与初始化
        node* get_node();
        node* new_node(const value_type& obj);
        void delete_node(node* n);
        node* clone_node(node* n);

        //以下函数取header的成员
        node* & root() const { return header->parent; }
        node* & leftmost() const { return header->left; }
        node* & rightmost() const { return header->right; }

        //以下函数用来方便取得节点x的成员
        static node* & left(node* x) { return x->left; }
        static node* & right(node* x) { return x->right; }
        static node* & parent(node* x) { return x->parent; }
        static reference value(node* x) { return x->val; }
        static const Key& key(node* x) { return KeyOfValue()(value(x)); }
        
        static node* minimum(node* x) { return node::minimum(x); }
        static node* maximum(node* x) { return node::maximum(x); }

    private:
        //一些辅助函数
        node* copy(node* x, node* p);
        void init();
    };

    /*****************************************************************************************
     * 赋值操作符
     * clear、swap
    *****************************************************************************************/
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>& 
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(const AVLTree& x) {
        if(this != &x) {
            clear();
            node_count = 0;
            key_compare = x.key_compare;
            if(nullptr == x.root()) {
                root() = nullptr;
                leftmost() = header;
                rightmost() = header;
            }
            else {
                root() = copy(x.root(), header);
                leftmost() = minimum(root());
                rightmost() = maximum(root());
                node_count = x.node_count;
            }
        }
        return *this;
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::clear() {
        if(0 != node_count) {
            erase(root());
            root() = nullptr;
            leftmost() = header;
            rightmost() = header;
            node_count = 0;
        }
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::swap(AVLTree& t) {
        MySTL::swap(header, t.header);
        MySTL::swap(node_count, t.node_count);
        MySTL::swap(key_compare, t.key_compare);
    }

    /*****************************************************************************************
     * 节点的get、new、delete、clone
    *****************************************************************************************/
   template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node* 
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::get_node() {
        //配置空间
        return node_allocator::allocate();
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node* 
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::new_node(const value_type& obj) {
        //配置空间
        node * n = get_node();
        n->left = n->right = n->parent = nullptr;
        //构造内容
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
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::delete_node(node* n) {
        destroy(&n->val);
        node_allocator::deallocate(n);
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node* 
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::clone_node(node* n) {
        node * tmp = new_node(n->val);
        tmp->left = tmp->right = tmp->parent = nullptr;
        return tmp;
    }

    /*****************************************************************************************
     * 一些辅助函数的具体实现
    *****************************************************************************************/
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::init() {
        header = get_node();
        
        root() = nullptr;
        leftmost() = header;
        rightmost() = header;
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node* 
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::copy(node* x, node* p) {
        //递归地copy
        node * top = clone_node(x);
        top->parent = p;
        try{
            if(x->right)
                top->right = copy(right(x), top);
            p = top;
            x = left(x);
            
            while(x) {
                node *y = clone_node(x);
                p->left = y;
                y->parent = p;
                if(x->right)
                    y->right = copy(right(x), y);
                p = y;
                x = left(x);
            }
        }
        catch(...) {
            // "commit or rollback"
            erase(top);
        }
        return top;
    }

    /*****************************************************************************************
     * 一些辅助函数的具体实现
    *****************************************************************************************/
}




#endif