#include <iostream>
#include "../MySTL/hashtable.hpp"
#include "../MySTL/allocator.hpp"


using std::hash;
using std::_Identity;
using std::equal_to;
using std::cout;
using std::cin;
using std::endl;

using MySTL::hashtable;
using MySTL::allocator;


int main() {
    hashtable<int,
        int,
        hash<int>,
        _Identity<int>,
        equal_to<int>,
        allocator> iht(50,hash<int>(), equal_to<int>());    //指定保留50个bucket(桶)
    cout<<iht.size()<<endl;                //0
    cout<<iht.bucket_count()<<endl;        //53这是STL提供的最小质数
    cout<<iht.max_bucket_count()<<endl;    //4294967291

    iht.insert_unique(59);
    iht.insert_unique(63);
    iht.insert_unique(108);
    iht.insert_unique(2);
    iht.insert_unique(53);
    iht.insert_unique(55);
    cout<<iht.size()<<endl;                //6,此即hashtable<T>::num_elements

    /*一下声明一个hashtable迭代器*/
    hashtable<int,
        int,
        hash<int>,
        _Identity<int>,
        equal_to<int>,
        allocator>::iterator ite = iht.begin();

    for(int i = 0;i<iht.size();i++,ite++)
        cout<< *ite <<' ';    //输出：53 55 2 108 59 63   并没有顺序
    cout<<endl;

    /*遍历所有buckets,如果其节点个数不为0，就打印节点个数，为0不打印*/
    for(int i = 0;i<iht.bucket_count();i++)
    {
        int n = iht.elems_in_bucket(i);
        if(n!=0)
            cout<<"bucket["<<i<<"] has"<<n<<" elems."<<endl;
    }
    int test = 1;
    /*
    输出：
    bucket[0] has 1 elems.
    bucket[2] has 3 elems.
    bucket[6] has 1 elems.
    bucket[10] has 1 elems.
    */

    /*为了验证“bucket(list)”的容量就是buckets vector的大小(这是从
    hashtable<T>::resize()得知的结果)，我刻意将元素加到54个，
    看看是否发生重建表*/

    // for(int i = 0;i<=47;i++)
    //     iht.insert_equal(i);
    // cout<<iht.size()<<endl;        //54
    // cout<<iht.bucket_count()<<endl;    //97确实扩容了(重建)

    // for (int i=0; i<iht.bucket_count(); i++)
    // {
    //     int n = iht.elems_in_bucket(i);
    //     if(n!=0)
    //         cout<<"bucket["<<i<<"] has"<<n<<" elems."<<endl;
    // }
    // /*打印的结果
    // bucket[2]到bucket[11]的节点个数为2
    // 其余的bucket[0]~bucket[47]的节点个数为1
    // 此外，bucket[53],[55],[59],[63]的节点个数均为1*/

    // /*以迭代器变量hashtable,将所有节点打印出来*/
    // ite = iht.begin();
    // for(int i = 0;i<iht.size();i++,ite++)
    //     cout<< *ite << ' ';
    // cout<<endl;

    // cout<<*(iht.find(2))<<endl;//2
    // cout<<iht.count(2)<<endl;//2

    // auto _pair = iht.equal_range(2);
    // for(auto i=_pair.first; i!=_pair.second; i++) {
    //     cout << *i << " ";
    // }
    // cout<<endl;

    // hashtable<int,
    //     int,
    //     hash<int>,
    //     _Identity<int>,
    //     equal_to<int>,
    //     allocator> iht2(50,hash<int>(), equal_to<int>());    //指定保留50个bucket(桶)

    // cout << (iht!=iht2 ? "true" : "false") << endl;
}