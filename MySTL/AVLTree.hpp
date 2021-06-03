/**
 * AVLTree
**/
#ifndef _AVLTREE_H_
#define _AVLTREE_H_

#include <utility>
#include "allocator.hpp"
#include "algorithm.hpp"

namespace MySTL {
    /*****************************************************************************************
     * AVL树的节点
    *****************************************************************************************/
    template<class Value>
    struct __AVLTree_node {
        Value val;
        long long height;  //高度
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
        if(cur->height == -1) {
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
        if(cur->height == -1) {
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
        //另：header的height设为-1，此值无意义，但可以用来标识header节点（iterator::operator--()中有用到）
        node* header;
        Compare key_compare;

    public:
        using iterator = __AVLTree_iterator<value_type>;
        using const_iterator = __AVLTree_const_iterator<value_type>;
        
        //构造函数
        AVLTree(const Compare& comp=Compare()): node_count(0), key_compare(comp) { init(); }
        AVLTree(const AVLTree& at): node_count(0), key_compare(at.key_compare) {
            if(at.root() == nullptr) init();
            else {
                init();
                root() = copy(at.root(), header);
                leftmost() = minimum(root());
                rightmost() = maximum(root());
            }
            node_count = at.node_count;
        }
        //赋值操作符
        AVLTree& operator=(const AVLTree& at);
        ~AVLTree() { clear(); delete_node(header); }

        void clear();
        void swap(AVLTree& t);

        //几个size相关函数
        size_type size() const { return node_count; }
        size_type max_size() const { return size_type(-1); }
        bool empty() const { return node_count==0; }

        //begin、end、key_comp
        Compare key_comp() const { return key_compare; }
        iterator begin() { return iterator(leftmost()); }
        const_iterator begin() const { return const_iterator(leftmost()); }
        iterator end() { return iterator(header); }
        const_iterator end() const { return const_iterator(header); }

        //插入相关
        //插入元素，不允许重复
        void insert_unique(const value_type& obj);
         //插入序列
        template<class InputIterator>
        void insert_unique(InputIterator first, InputIterator last) { insert_unique(first, last, iterator_category(first)); }
        //InputIterator类型
        template<class InputIterator>
        void insert_unique(InputIterator first, InputIterator last, input_iterator_tag);
        //ForwardIterator类型及以上
        template<class ForwardIterator>
        void insert_unique(ForwardIterator first, ForwardIterator last, forward_iterator_tag);

        //查找
        iterator find(const key_type& k);
        const_iterator find(const key_type& k) const;
        size_type count(const key_type& k) const;
        std::pair<iterator, iterator> equal_range(const key_type& k);
        std::pair<const_iterator, const_iterator> equal_range(const key_type& k) const;
        iterator lower_bound(const key_type& k);
        const_iterator lower_bound(const key_type& k) const;
        iterator upper_bound(const key_type& k);
        const_iterator upper_bound(const key_type& k) const;

        //erase
        void erase(const key_type& k);
        void erase(iterator pos);
        void erase(iterator first, iterator last);

        //友元声明
        template<class Ky, class Vl, class KV,
                 class Cp, template<class t> class Al>
        friend bool operator== (const AVLTree<Ky, Vl, KV, Cp, Al>& at1,
                                const AVLTree<Ky, Vl, KV, Cp, Al>& at2);

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
        static const Key& key(const node* x) { return KeyOfValue()(value(const_cast<node*>(x))); }
        static long long height(node* x) { return x ? x->height : -1; }
        
        static node* minimum(node* x) { return node::minimum(x); }
        static node* maximum(node* x) { return node::maximum(x); }

    private:
        //一些辅助函数
        node* copy(node* x, node* p);
        void init();
        node* __insert(node* &tree, node* parent, const value_type& obj);
        //供以上erase函数使用
        node* __erase(node* &tree, node* x);
        //此函数特殊，删除以x为顶的整棵子树 供clear使用
        void __erase(node* x);

        //四种情况对应的旋转操作
        node* leftLeftRotation(node* x);
        node* rightRightRotation(node* x);
        node* leftRightRotation(node* x);
        node* rightLeftRotation(node* x);
    };

    /*****************************************************************************************
     * 赋值操作符
     * clear、swap
    *****************************************************************************************/
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>& 
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(const AVLTree& at) {
        if(this != &at) {
            clear();
            node_count = 0;
            key_compare = at.key_compare;
            if(nullptr == at.root()) {
                root() = nullptr;
                leftmost() = header;
                rightmost() = header;
            }
            else {
                root() = copy(at.root(), header);
                leftmost() = minimum(root());
                rightmost() = maximum(root());
                node_count = at.node_count;
            }
        }
        return *this;
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::clear() {
        if(0 != node_count) {
            __erase(root());
            root() = nullptr;
            leftmost() = header;
            rightmost() = header;
            node_count = 0;
        }
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::swap(AVLTree& at) {
        MySTL::swap(header, at.header);
        MySTL::swap(node_count, at.node_count);
        MySTL::swap(key_compare, at.key_compare);
    }
    /*****************************************************************************************
     * 节点的get、new、delete、clone
    *****************************************************************************************/
   template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node* 
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::get_node() {
        //配置空间
        return node_allocator::allocate();
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node* 
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::new_node(const value_type& obj) {
        //配置空间
        node * n = get_node();
        n->left = n->right = n->parent = nullptr;
        n->height = 0;
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
    typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node* 
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::clone_node(node* n) {
        node * tmp = new_node(n->val);
        tmp->height = n->height;
        tmp->left = tmp->right = tmp->parent = nullptr;
        return tmp;
    }

    /*****************************************************************************************
     * 一些辅助函数的具体实现
     * init、copy、__insert、__erase
    *****************************************************************************************/
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::init() {
        header = get_node();
        header->height = -1;
        
        root() = nullptr;
        leftmost() = header;
        rightmost() = header;
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node* 
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
            __erase(top);
        }
        return top;
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node*
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(node* &tree, node* parent, const value_type& obj) {
        if(nullptr == tree) {
            tree = new_node(obj);
            tree->parent = parent;
            node_count++;
        }
        //插入到左子树
        else if(key_compare(KeyOfValue()(obj), key(tree))) {
            tree->left = __insert(tree->left, tree, obj);
            //失去平衡
            if(height(tree->left)-height(tree->right) >= 2) {
                //LL情况
                if(key_compare(KeyOfValue()(obj), key(tree->left)))
                    tree = leftLeftRotation(tree);
                //LR情况
                else tree = leftRightRotation(tree);
            }
        }
        else if(key_compare(key(tree), KeyOfValue()(obj))) {
            tree->right = __insert(tree->right, tree, obj);
            //失去平衡
            if(height(tree->right)-height(tree->left) >= 2) {
                //RR情况
                if(key_compare(key(tree->right) ,KeyOfValue()(obj)))
                    tree = rightRightRotation(tree);
                //RL情况
                else tree = rightLeftRotation(tree);
            }
        }
        //当key相同时，以上两个if都返回false，插入失败 不应重复插入
        else {}
        //求高度 叶子节点高度为0，某节点的高度为max(其左右子节点高度)+1
        tree->height = max(height(tree->left), height(tree->right))+1;

        return tree;
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node* 
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::__erase(node* &tree, node* x) {
        if(nullptr==tree || nullptr==x)
            return nullptr;  
        //删除节点在左子树
        if(key_compare(key(x), key(tree))) {
            tree->left = __erase(tree->left, x);
            //失衡
            if(height(tree->right)-height(tree->left) >= 2) {
                node* r = tree->right;
                if(height(r->left) > height(r->right))
                    tree = rightLeftRotation(tree);
                else 
                    tree = rightRightRotation(tree);
            }
        }
        else if(key_compare(key(tree), key(x))) {
            tree->right = __erase(tree->right, x);
            if(height(tree->left)-height(tree->right) >= 2) {
                node* l = tree->left;
                if(height(l->right)>height(l->left))
                    tree = leftRightRotation(tree);
                else tree = leftLeftRotation(tree);
            }
        }
        //tree是需要删除的节点
        else {
            //左右子节点均非空
            if((nullptr!=tree->left) && (nullptr!=tree->right)) {
                //若tree的左子树比右子树高 找出左子树中最大节点 将该最大节点的值赋值给tree 删除该最大节点
                if(height(tree->left) > height(tree->right)) {
                    node* max = maximum(tree->left);
                    tree->val = max->val;
                    tree->left = __erase(tree->left, max);
                }
                else {
                    node* min = minimum(tree->right);
                    tree->val = min->val;
                    tree->right = __erase(tree->right, min);
                }
            }
            else {
                node* tmp = tree;
                tree = (nullptr!=tree->left) ? tree->left : tree->right;
                if(nullptr != tree) {
                    tree->parent = tmp->parent;
                }
                delete_node(tmp);
                node_count--;
                //更新
                if(nullptr == root()) {
                    leftmost() = header;
                    rightmost() = header;
                }
                else {
                    leftmost() = minimum(root());
                    rightmost() = maximum(root());
                }
            }
        }
        return tree;
    }
    // erase without rebalancing
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::__erase(node* x) {
        while(nullptr != x) {
            __erase(right(x));
            node* y = left(x);
            delete_node(x);
            x = y;
        }
    } 
    /*****************************************************************************************
     * 四种情况对应的旋转操作
    *****************************************************************************************/
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node*
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::leftLeftRotation(node* x) {
        node* n = x->left;
        x->left = n->right;
        n->right = x;
        //parent
        n->parent = x->parent;
        x->parent = n;
        if(nullptr != x->left) x->left->parent = x;

        x->height = max(height(x->left), height(x->right))+1;
        n->height = max(height(n->left), x->height)+1;
        
        return n;
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node*
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::rightRightRotation(node* x) {
        node* n = x->right;
        x->right = n->left;
        n->left = x;
        //parent
        n->parent = x->parent;
        x->parent = n;
        if(nullptr != x->right) x->right->parent = x;
        
        x->height = max(height(x->left), height(x->right))+1;
        n->height = max(height(n->right), x->height)+1;
        
        return n;
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node*
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::leftRightRotation(node* x) {
        x->left = rightRightRotation(x->left);
        
        return leftLeftRotation(x);
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::node*
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::rightLeftRotation(node* x) {
        x->right = leftLeftRotation(x->right);

        return rightRightRotation(x);
    }
    /*****************************************************************************************
     * 插入相关
    *****************************************************************************************/
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& obj) {
        size_type old = node_count;
        __insert(root(), header, obj);
        //更新
        if(old < node_count) {
            leftmost() = minimum(root());
            rightmost() = maximum(root());
        }
    }
    //插入序列
    //InputIterator类型
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    template<class InputIterator>
    void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(InputIterator first, InputIterator last, input_iterator_tag) {
        for( ; first!=last; first++)
            insert_unique(*first);
    }
    //ForwardIterator类型及以上
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    template<class ForwardIterator>
    void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
        size_type n = distance(first, last);
        for( ; n>0; n--,first++)
            insert_unique(*first);
    }
    /*****************************************************************************************
     * 查找相关
    *****************************************************************************************/
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type& k) {
        node* y = header;
        node* x = root();
        
        while(nullptr != x) {
            //k<=key(x) 则向左 并用y记录x
            if(!key_compare(key(x), k))
                y = x, x = left(x);
            else x = right(x);
        }

        iterator j = iterator(y);
        //满足j!=end()并且k>=key(x) 结合之前的k<=key(x) 因此k=key(x)
        return (j==end() || key_compare(k, key(j.cur))) ? end() : j;
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type& k) const {
        node* y = header;
        node* x = root();
        
        while(nullptr != x) {
            //k<=key(x) 则向左 并用y记录x
            if(!key_compare(key(x), k))
                y = x, x = left(x);
            else x = right(x);
        }

        const_iterator j = const_iterator(y);
        //满足j!=end()并且k>=key(x) 结合之前的k<=key(x) 因此k=key(x)
        return (j==end() || key_compare(k, key(j.cur))) ? end() : j;
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::size_type 
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::count(const key_type& k) const {
        std::pair<const_iterator, const_iterator> p = equal_range(k);
        size_type n = distance(p.first, p.second);
        return n;
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    std::pair<typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,
    typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::iterator>
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type& k) {
        return std::pair<iterator, iterator>(lower_bound(k), upper_bound(k));
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    std::pair<typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator,
    typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator>
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type& k) const {
        return std::pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type& k) {
        node* y = header;
        node* x = root();
        while(nullptr != x) {
            //k<=key(x)
            if(!key_compare(key(x), k))
                y = x, x = left(x);
            else x = right(x);
        }
        return iterator(y);
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator 
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type& k) const {
        node* y = header;
        node* x = root();
        while(nullptr != x) {
            //k<=key(x)
            if(!key_compare(key(x), k))
                y = x, x = left(x);
            else x = right(x);
        }
        return const_iterator(y);
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type& k) {
        node* y = header;
        node* x = root();
        while(nullptr != x) {
            //k<key(x)
            if(key_compare(k, key(x)))
                y = x, x = left(x);
            else x = right(x);
        }
        return iterator(y);
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    typename AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator 
    AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type& k) const {
        node* y = header;
        node* x = root();
        while(nullptr != x) {
            //k<key(x)
            if(key_compare(k, key(x)))
                y = x, x = left(x);
            else x = right(x);
        }
        return const_iterator(y);
    }
    /*****************************************************************************************
     * erase相关
    *****************************************************************************************/
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const key_type& k) {
        node* x;
        if(nullptr != (x=find(k).cur))
            root() = __erase(root(), x);
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator pos) {
        root() = __erase(root(), pos.cur);
    }
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc>
    void AVLTree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator first, iterator last) {
        if(first==begin() && last==end())
            clear();
        else while(first != last) erase(first++);
    }
    /*****************************************************************************************
     * operator==、operator!=
    *****************************************************************************************/
    template<class Ky, class Vl, class KV,
                 class Cp, template<class t> class Al>
    bool operator== (const AVLTree<Ky, Vl, KV, Cp, Al>& at1,
                     const AVLTree<Ky, Vl, KV, Cp, Al>& at2) {
        return at1.size()==at2.size() && 
               equal(at1.begin(), at1.end(), at2.begin());
    }
    template<class Ky, class Vl, class KV,
                 class Cp, template<class t> class Al>
    bool operator!= (const AVLTree<Ky, Vl, KV, Cp, Al>& at1,
                     const AVLTree<Ky, Vl, KV, Cp, Al>& at2) {
        return !(at1==at2);
    }
}




#endif