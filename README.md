MySTL
=====
## 简介
基于《STL源码剖析》，学习STL中的相关数据结构和算法，了解C++ Template编程，并实现了一部分STL中的容器与函数。

## 环境

* 操作系统 Ubuntu 18.04.4
* 编译器 g++ 7.4.0

## 主要实现的容器及相关组件

- alloc（内存管理，以字节为单位进行内存分配与释放，供allocator内部使用）

- construct（对象的构造与析构）

- allocator

  - 内存的分配与释放（以对象为单位） allocate deallocate

  - 对象的构造与析构 construct destroy

- iterator（迭代器相关：五种迭代器类型、iterator_traits迭代器指向类型萃取等等）

- type_traits（类型萃取）

- algorithm（几个容器中会用到的基本算法）

  - fill_n、fill
  - copy、copy_backward
  - equal
  - swap
  - max

- uninitialized（用于对未初始化空间构造元素，定义了全局函数，用来填充（fill）或复制（copy）大块内存数据；根据对象的类型（是否是POD）最差情况下会调用construct()，最佳情况下会使用C标准函数memmove()直接进行内存数据的移动）

- **vector**：序列式容器

- hashtable：哈希表，使用（separate chaining）开链法来解决（碰撞 collision）问题，由 vector 和 linked list 组合实现。

- **unordered_set**：关联式容器（底层是hashtable）

- AVLTree：平衡二叉搜索树（任一节点对应的两棵子树的最大高度差为1。）

- **set**：关联式容器（底层是AVLTree）

## 文档
*目录*

- [vector](https://github.com/sly461/MySTL/blob/main/container_test/vector/vector.md)
- [hashtable](https://github.com/sly461/MySTL/blob/main/container_test/HashTable/HashTable.md)
- [AVLTree](https://github.com/sly461/MySTL/blob/main/container_test/AVLTree/AVLTree.md)

## 测试


