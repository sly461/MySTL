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
    };

    /*****************************************************************************************
     * AVL树的迭代器
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
        if() {
            
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

    /*****************************************************************************************
     * AVLTree
     * Key：键值型别 Valule：实值型别 KeyOfValue：从节点中取出键值的方法
     * Compare:比较键值的方法 Alloc:空间配置器
    *****************************************************************************************/
    template<class Key, class Value, class KeyOfValue,
             class Compare, template <class T> class Alloc=allocator>
    class AVLTree {
        
    };
}




#endif