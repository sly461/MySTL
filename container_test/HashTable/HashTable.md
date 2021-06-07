# Hash Table

## 散列表(哈希表)的定义

用于海量数据的快速查找，效率高，时间复杂度接近 O(1)。

散列表是根据给定一个关键字 Key(整数)，通过散列函数(映射函数)，将关键字 Key 映射到散列表(存放记录的数组)中的一个位置(数组的索引 index)，
那么 arr[index] 就填充 Key。

`HashFunction(Key) = index，arr[index] = Key`

## 两种方式寻址

* 直接寻址表
  + 关键字的全域范围 U 比较小；
  + 用数组实现直接寻址表，数组的下标就是关键字的值；
  + 没有两个元素具有相同的关键字；
  + 查找、插入、删除都为 O(1)；

* 散列表
  + 适用关键字的范围 U 很大；
  + 通过散列函数来计算出数组的索引，将关键字 Key，通过散列函数计算后的索引比关键字的全域 U 要小得多，这样散列表的存储空间小很多，节省空间；
  + 存在一个问题：不同的关键字，经过散列函数，可能映射到同一个索引上，这样会起冲突，所以需要解决碰撞问题；

## 散列函数

假设所有元素被散列到 m 个槽中的每一个的可能性是相同的，这个假设为简单一致散列(simple uniform hashing)。

一个好的散列函数应该近似地满足简单一致散列，每个关键字都等可能的散列到 m 个槽位上的任何一个中去，并与其它的关键字已被散列到哪一个槽位无关。

当关键字不是自然数 N = {0, 1, 2...}，那么需要转换为自然数。例如当关键字是字符串时，可以通过将字符串中的每个字符的 ASCII 码相加，转换为自然数。

### 散列函数的设计方法

* 除法散列法(常用)

关键字 Key 映射到 m 个槽中的某一位置，散列函数为：h(Key) = Key mod m，其中 m 不应是 2 的幂，通常 m 的值是与 2 的整数幂不太接近的质数。

 `arr[h(Key)] = Key`

* 乘法散列法

用关键字 key 乘上常数 A(0<A<1)，并抽取出 `key*A` 的小数部分。用 m 乘以这个抽取出来的小数，再对乘积向下取整(floor)。总之，散列函数为：

`h(Key) = floor(m*(Key*A mod 1))`； 其中 mod 1 的意思就是取小数部分。 `Key*A mod 1` 即为 `Key*A - floor(Key*A)`。

乘法散列法的优点是对 m 的选择没什么特别的要求。一般选 m 为 2 的某次幂(2^p，p为某个整数)

* 全域散列法

为了尽可能地避免最坏情况的发生，我们不使用某个特定的散列函数，而是准备好一系列的散列函数，在执行开始时随机选择一个作为之后的散列函数。这种方法称作全域散列(universal hashing)。

## 碰撞(collision)问题

通常有两类方法处理碰撞：开放寻址(Open Addressing)法 和 链接(Chaining)法。前者是将所有结点均存放在散列表 T[0..m-1] 中；后者通常是把散列到同一槽中的所有元素放在一个链表中，而将此链表的头指针放在散列表 T[0..m-1] 中。

### 开放寻址法

开放寻址法是将所有的元素都存放在散列表中，每个表项包含一个元素或者包含 NIL，但是不包含链表或者其它的处于散列表外的辅助结构。

当插入一个元素时，如果映射的位置已经被其它元素占用，那么需要通过散列函数再产生另一个映射值(称为探查)，直到找到空槽或发现表中没有空槽为止。

在开放寻址法中，字典操作需要找到一个”槽序列”，比如要插入元素，需要按照某个槽序列探查散列表，直到找到一个空槽。探查的序列不一定是0,1…m-1。而是要依赖于待插入的关键字。对于每一个关键字k，探查序列为：h(k,i)  (0 <= i <= m-1)。伪代码如下:

```
 HASH_INSERT(T, k)            
            i= 0                      
              repeat
                 j = h(k, i);                                     
                 if T[j] == NIL
                      T[j]= k;              
                       return  j              
                 else                     
                      i += 1;               
              until  i==m
     error  "hash table overflow"

 HASH_SERACH(T,k)
              i = 0
              repeat
                     j = h(k, i)
                     if T[j] == k
                            return j
                     i = i+1
              until T[j] == NIL or i ==m
              return NIL
```

**缺点：删除操作执行起来比较困难，当我们从槽i中删除关键字时，不能简单地让T[i]=NIL，因为这样会破坏查找的过程。假设关键字k在i之后 插入到散列表中，如果T[i]被设为NIL，那么查找过程就再也找不到k了。解决这个问题的方法是引入一个新的状态DELETED，而不是NIL，这样在插入过程中，一旦发现DELETED的槽，便可以在该槽中放置数据，而查找过程不需要任何改动。但如此一来，查找时间就不再依赖于装载因子了，所以在必须删除关键字的应用中，往往采用链接法来解决碰撞。**

有三种方式常用来计算开放寻址法中的探查序列：**线性探查**、**二次探查**、**双重探查**。

> 线性探查(常用)

给定一个普通的散列函数 h':U-->{0,1,...,m-1}，称为辅助散列函数，线性探查方法采用的散列函数为

`h(Key,i)=(h'(Key)+i) mod m, i = 0,1,...,m-1`

给定一个关键字 Key，首先探查槽 T[h'(Key)], 即由辅助散列函数所给出的槽位。再探测 T[h'(Key)+1]，依次类推，直到槽 T[m-1]。然后，又绕到槽 T[0], T[1],...,直到最后探测到槽 T[h'(Key)-1]。

**缺点：随着连续被占用的槽不断增加，平均查找时间也随之不断增加。**

> 二次探查

`h(Key,i)=(h'(Key)+c₁i+c₂i²) mod m  , i = 0,1,...,m-1`

其中 h' 是一个辅助散列函数，c₁ 和 c₂ 为正的辅助常数，i = 0,1,...m-1。初始的探查位置为 T[h'(Key)]，后续的探查位置要加上一个偏移量，该偏移量以二次的方式依赖于探查序号i。这种探查方法的效果要比线性探查好很多，但是，为了能够充分利用散列表，c₁，c₂和 m 的值要受到限制。此外，如果两个关键字的初始探查位置相同，那么它们的探查序列也是相同的。这一性质可导致一种轻度的群集，称为二次群集。

**缺点：不易探查到整个散列空间。**

> 双重探查

双重散列是用于开放寻址法的最好方法之一，因为它产生的排列近似于随机选择的排列。它采用如下形式的散列函数：

`h(Key, i) = (h1(Key) + i*h2(Key)) mod m`

其中 h1 和 h2 为辅助散列函数。初始探测位置为 T[h1(Key)]，后续的探测位置在此基础上加上偏移量 h2(Key) 模 m。

### 链接法(常用)

将散列到同一槽中的所有元素（冲突的元素）都放在一个链表中。

若选定的散列表长度为 m，则可将散列表定义为一个由 m 个头指针组成的指针数组 T[0..m-1]。凡是散列地址为 i 的结点，均插入到以 T[i] 为头指针的单链表中。T中各分量的初值均应为空指针。

**优点：删除操作比较方便。**

## STL hashtable

SGI STL hash table 使用（separate chaining）开链法来解决（碰撞 collision）问题。

### hashtable 内部实现

* 桶 buckets：用 vector 表示
* hash table 节点：链表的节点

大致描述：对于一个数，进行散列函数处理，获得一个索引(就是桶的节点)；当数量很大时，经过散列函数处理，会得到相同的索引，那么桶的节点位置一样，
此时，将相同的节点使用链表连接起来。

```cpp
// hash table 节点
template <class _Val>
struct _Hashtable_node
{
    _Hashtable_node* _M_next;
    _Val _M_val;
}; 
```

hash table 迭代器必须永远维持整个 "buckets vector" 的关系，同时并记录目前所指的节点。hash table 迭代器类型为 forward_iterator。

因为是forward_iterator，因此迭代器只能进行 operator++ 操作，如果目前节点正好是当前 list 的尾端，就跳到下一个 bucket 上。

```cpp
// hash table 迭代器
template <class _Val, class _Key, class _HashFcn,
          class _ExtractKey, class _EqualKey, class _Alloc>
struct _Hashtable_iterator {
    typedef hashtable<_Val,_Key,_HashFcn,_ExtractKey,_EqualKey,_Alloc> _Hashtable;
    typedef _Hashtable_node<_Val> _Node; // hash table 节点

    typedef forward_iterator_tag iterator_category; // 迭代器类型：前向迭代器

    //迭代器类中主要的数据成员
    _Node* _M_cur; // 迭代器目前所指的节点
    _Hashtable* _M_ht; // 保持对容器的连接关系，bucket
    ...
};
```

```cpp
// hash table 数据结构
// _Val 节点的实值类型，_Key 节点的键值类型，_HashFcn 哈希函数的类型
// _ExtractKey 从节点中取出键值的方法，_EqualKey 判断键值是否相同的方法
// _Alloc 空间配置器
template <class _Val, class _Key, class _HashFcn,
          class _ExtractKey, class _EqualKey, class _Alloc>
class hashtable {
public:
public:
    typedef _Key key_type; // 节点的键值
    typedef _Val value_type; // 节点的实值
    typedef _HashFcn hasher; // 哈希函数
    typedef _EqualKey key_equal; // 判断键值是否相同
    typedef size_t size_type;
private:
    typedef _Hashtable_node<_Val> _Node; // 节点
    
    //hashtable类中的主要数据成员
    vector<_Node*,_Alloc> _M_buckets; // vector 容器
    size_type num_elements;
};
```

```cpp
  // 接受键值和 buckets 个数，求出该元素在属于 bucket
  size_type _M_bkt_num_key(const key_type& __key, size_t __n) const
  {
    return _M_hash(__key) % __n;
  }
```

SGI STL 使用质数来设计 buckets 个数，质数从 53 开始，总共 28 个质数，当用户需要多少个 buckets，选取“最接近某数并大于某数”的质数。

### insert相关

默认都是**头插法**

> insert_unique

键值不能相同，供unordered_set、unordered_map使用。

> insert_equal

允许键值重复，供unordered_multiset、unordered_multimap使用，另：插入时，遍历到键值相同的节点，直接插入其后面，意即键值相同的节点是相连在一起的

### 总结

SGI STL 的 hash table 实现由 vector 和 linked list 组合而成。

SGI STL 默认的 hash function 是计算元素位置的函数，元素类型只支持 `char*`，`char`，`int`，`long`，`short`。

## 参考资料

* <<算法导论>> 第11章 散列表
* CS210 Lab: Hash Table：http://www.cs.uregina.ca/Links/class-info/210/Hash/#EXERCISE
* http://www.cnblogs.com/Anker/archive/2013/01/27/2879150.html
* http://blog.csdn.net/duan19920101/article/details/51579136
* http://www.cnblogs.com/zhoutaotao/p/4067749.html#zaisanliewenti