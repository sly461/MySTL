## vector

### vector 上的常见操作复杂度

* 随机访问——常数 O(1)
* 在末尾插入或移除元素——均摊常数 O(1)
* 插入或移除元素到 vector 结尾的距离成线性 O(n)

### vector 的迭代器类型 Random Access Iterator

vector 的迭代器涵盖了指针所有的算术能力(`operator*，operator->，operator++，operator--，operator+，operator-，operator+=，operator-=`)，
同时 vector 支持随机存取（直接使用原生指针作为迭代器），所以 vector 提供是 Random Access Iterator。

```c++
using iterator = value_type*;
using const_iterator = const iterator;
```

### vector 的数据结构

```cpp
template <class T, class Alloc = alloc>
class vector
{
public:
    typedef T value_type;
    typedef value_type* iterator; // vector 的迭代器是普通指针
protected:
    //vector类中的主要数据成员
    iterator start;  // 表示目前使用空间的头
    iterator finish; // 表示目前使用空间的尾
    iterator end_of_storage; // 表示目前可用空间的尾
};
```

![](https://github.com/steveLauwh/SGI-STL/raw/master/The%20Annotated%20STL%20Sources%20V3.3/Other/vector.PNG)

### vector 底层实现的记录

size_type：无符号整数类型（通常是 std::size_t）

#### uninitialized_fill_n 功能

```cpp
template< class ForwardIt, class Size, class T >
ForwardIt uninitialized_fill_n( ForwardIt first, Size count, const T& value );
```
函数功能是：从 first 起始，将 value 的值复制 count 个。

函数的参数：

* first	-	要初始化的元素范围起始
* count	-	要构造的元素数量
* value	-	构造元素所用的值

#### uninitialized_copy 功能

```cpp
template< class InputIt, class ForwardIt >
ForwardIt uninitialized_copy( InputIt first, InputIt last, ForwardIt d_first );
```
函数的功能：复制来自范围 `[first, last)` 的元素到始于 d_first 的未初始化内存。

函数的参数：

* first, last	-	要复制的元素范围
* d_first	-	目标范围的起始

#### uninitialized_fill 功能   

```cpp
template< class ForwardIt, class T >
void uninitialized_fill( ForwardIt first, ForwardIt last, const T& value );
```
函数的功能：复制给定的 value 到以 `[first, last)` 定义的未初始化内存区域。

函数的参数：

* first, last	-	要初始化的元素的范围
* value	-	构造元素所用的值

#### copy 功能

```cpp
template< class InputIt, class OutputIt >
OutputIt copy( InputIt first, InputIt last, OutputIt d_first );
```
函数的功能：复制 `[first, last)` 所定义的范围中的元素到始于 d_first 的另一范围。

函数的参数：

* first, last	-	要复制的元素范围
* d_first	-	目标范围的起始

```c++
template<class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,BidirectionalIterator2 result);
```

- copy_backward 逆向复制 基本结构与copy相似
- 把 [first, last) 内的每个元素，以逆行的方式复制到以result-1为起点，方向亦为逆行的区间上，即[result-(last-first), result)

#### vector insert 形式

```cpp
// 在 pos 前插入 value，底层实现 _M_insert_aux
iterator insert( iterator pos, const T& value );

// 在 pos 前插入 value 的 count 个副本，底层实现 _M_fill_insert
void insert( iterator pos, size_type count, const T& value );

// 在 pos 前插入来自范围 `[first, last)` 的元素，底层实现 _M_range_insert
template< class InputIt >
void insert( iterator pos, InputIt first, InputIt last);
```

**vector配置空间的原则**

如果原大小为0，则配置1个元素大小的空间；否则配置原大小两倍的空间。

#### resize、reserve、shrink_to_fit

```c++
template<class T, class Alloc>
    void vector<T, Alloc>::resize(size_type new_size, const value_type & value);
```

若new_size小于当前容器的size，则`erase(begin() + new_size, end());`（但其实，如果T类型是POD类型，finish后如果有内存空间，其中残余的元素内容其实是可以访问输出的）

否则，`insert(end(), new_size-size(), value);`

```c++
template<class T, class Alloc>
    void vector<T, Alloc>::reserve(size_type n);
```

预留存储空间，若 n 大于当前的 capacity() ，则重新分配一块新的更大的内存，否则该方法不做任何事。

```c++
template<class T, class Alloc>
    void vector<T, Alloc>::shrink_to_fit();
```

- 放弃多余空间 即finish到end_of_storage的空间减小容器的容量以适应其大小，并破坏超出该容量size的所有元素。
- “破坏”并不是简单地`destroy`元素，而是**以"重新分配内存空间（大小=finish-start）"的方式”破坏“超出size的元素**

### 测试

> vector的resize()和shrink_to_fit() 函数

```c++
// C++ program to illustrate 
// the vector::shrink_to_fit()  
#include <bits/stdc++.h> 
using namespace std; 
  
int main() 
{ 
  
    // Initialized vector 
    vector<int> v(10); 
  
    for (int i = 0; i < 10; i++) 
        v[i] = i; 
  
    // Initial vector 
    cout << "Vector size initially: " << v.size(); 
  
    cout << "\nVector elements are: "; 
    for (int i = 0; i < 10; i++) 
        cout << v[i] << " "; 
  
    // changes the size of the Vector 
    // but does not destroys the elements 
    v.resize(5); 
  
    cout << "\n\nVector size after resize(5): "
    << v.size(); 
  
    cout << "\nVector elements after resize(5) are: "; 
    for (int i = 0; i < 10; i++) 
        cout << v[i] << " "; 
  
    // Shrinks to the size 
    // till which elements are 
    // destroys the elements after 5 
    v.shrink_to_fit(); 
  
    cout << "\n\nVector size after shrink_to_fit(): "
    << v.size(); 
  
    cout << "\nVector elements after shrink_to_fit() are: "; 
    for (int i = 0; i < 10; i++) 
        cout << v[i] << " "; 
  
    return 0; 
}
```

输出

```
Vector size initially: 10
Vector elements are: 0 1 2 3 4 5 6 7 8 9 

Vector size after resize(5): 5
Vector elements after resize(5) are: 0 1 2 3 4 5 6 7 8 9 

Vector size after shrink_to_fit(): 5
Vector elements after shrink_to_fit() are: 0 1 2 3 4 0 127889 0 0 0
```

