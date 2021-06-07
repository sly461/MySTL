MySTL
=====
## 简介
基于《STL源码剖析》，学习STL中的相关数据结构和算法，了解C++ Template编程，并实现了一部分STL中的容器与相关组件。

## 环境

* 操作系统 linux, ubuntu 18.04
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
**目录**

- [vector](https://github.com/sly461/MySTL/blob/main/container_test/vector/vector.md)
- [hashtable](https://github.com/sly461/MySTL/blob/main/container_test/HashTable/HashTable.md)
- [AVLTree](https://github.com/sly461/MySTL/blob/main/container_test/AVLTree/AVLTree.md)

## 测试

> 基于[STL容器测试框架](https://github.com/Alinshans/MyTinySTL/tree/master/Test)进行单元测试和性能测试

### 测试环境

- CPU：Intel(R) Xeon(R) CPU E5-26xx v4
- 内存：2G
- linux, ubuntu 18.04, g++ 7.4.0

### 单元测试

- [vector](https://github.com/sly461/MySTL/blob/main/container_test/vector/test_vector.cpp) *(100%/100%)*
- [unordered_set](https://github.com/sly461/MySTL/blob/main/container_test/HashTable/test_unordered_set.cpp) *(100%/100%)*
- [set](https://github.com/sly461/MySTL/blob/main/container_test/AVLTree/test_set.cpp) *(100%/100%)*

### 性能测试

以下均以int作为容器元素进行测试

- vector

  | push_back | 500000（个元素） | 5000000（个元素） | 50000000（个元素） |
  | --------- | ---------------- | ----------------- | ------------------ |
  | std       | 15 ms            | 159 ms            | 1548 ms            |
  | MySTL     | 9 ms             | 100 ms            | 983 ms             |

- unordered_set

  | insert | 100000（个元素） | 1000000（个元素） | 10000000（个元素） |
  | ------ | ---------------- | ----------------- | ------------------ |
  | std    | 44 ms            | 751 ms            | 9257 ms            |
  | MySTL  | 30 ms            | 416 ms            | 5070 ms            |

- set

  | insert | 100000（个元素） | 1000000（个元素） | 10000000（个元素） |
  | ------ | ---------------- | ----------------- | ------------------ |
  | std    | 91 ms            | 1380 ms           | 20625 ms           |
  | MySTL  | 289 ms           | 1736 ms           | 29271 ms           |

  这个性能差距的原因是内部实现机制不同，std::set底层是红黑树，MySTL::set底层是AVL树，其中，AVL树严格保证任一节点对应的两棵子树的最大高度差为1，可以说是一种”绝对平衡“。相比于AVL树，红黑树则牺牲了部分平衡性，以换取删除/插入操作时少量的旋转次数，整体来说，性能优于AVL树。

### 另：关于内存泄漏

使用 Valgrind 检测了内存泄漏，其中由于使用了内存池，因此，当程序终止时，内存池的内存和以链表形式挂载在 `free_list`上内存块才会被操作系统回收。

> 程式結束的時候我們申請的記憶體该如何釋放 ？
>
> 我們都知道, 由程式設計師配置的記憶體一定要由程式設計師進行釋放, 否則可能會造成記憶體流失, 但是這樣的問題在記憶池中並不存在. 這是因為記憶池承載了整個程式運作時的記憶體配置, 所以記憶池中的記憶體回收一定是在程式終結之後, 並且是在所有程式有關的物件被回收之後才進行釋放. 而作業系統在某一個程式結束之後, 也會承擔起回收這一個程式在運作的時候申請的所有資源, 這其中包括沒有被程式設計師釋放的記憶體. 從程式運作效率的角度來考慮, 與其我們自己來釋放記憶池, 不如直接讓作業系統在回收資源的時候連帶我們的記憶池一起回收, 前後的相差並不大. 而且如果由程式設計師自己釋放記憶池, 那麼會產生更多的程式碼, 並且對於記憶池的運作效率會產生一定的影響. 記憶池是屬於比較低層的物件, 所以我們儘量不要放不必要的操作在記憶池中 (現在想想我們曾經為了節約空間, 將節點做成了等位). 因此, 最終的答案就是不釋放, 由作業系統負責回收

## 参考资料

- 《STL 源代码剖析》 侯捷
- [SGI-STL V3.3](https://github.com/steveLauwh/SGI-STL)
- [STL容器测试框架](https://github.com/Alinshans/MyTinySTL/tree/master/Test)