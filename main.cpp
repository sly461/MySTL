#include <iostream>
#include <vector>
#include <string>
#include <hash_set>
#include "MySTL/vector.hpp"
#include "MySTL/hashtable.hpp"
using std::cin;
using std::cout;
using std::string;
using std::endl;

struct Test {
    int a;
    string b;
    Test(int _a, string _b):a(_a), b(_b){
        cout << "construct" << endl;
    }
    Test(const Test& test):a(test.a), b(test.b) {
        cout << "copy" << endl;
    }
    Test(Test&& test):a(test.a), b(move(test.b)) {
        cout << "move" << endl;
    }
    Test& operator=(const Test & test) {
        a = test.a;
        b = test.b;
    }
};

void test(MySTL::__true_type)
{
    std::cout << "true";
}

void test(MySTL::__false_type)
{
    std::cout << "false";
}

void test(int)
{
    std::cout << "int";
}

template <class ForwardIterator>
void destroy(ForwardIterator first)
{
    using tria = typename std::iterator_traits<ForwardIterator>::value_type;
    using trivial_destructor = typename MySTL::__type_traits<tria>::has_trivial_destructor;
    test(trivial_destructor());
}


class testA {
    public:
        int a;
    public:
        testA(): a(11) {}
        testA(int _a): a(_a) {}
        testA(const testA & ta) { a = ta.a; std::cout<<"testA copy constructor!"<<std::endl; }
        bool operator == (const testA& b) { return a==b.a; }
        bool operator != (const testA& b) { return !(*this==b); }

};

int main()
{
    // MySTL::vector<int> intVeck(20, 11);
    // intVeck[10] = 12;
    // MySTL::vector<int> intVeckk(intVeck.begin(), intVeck.end());
    // std::cout << intVeckk.size() << ":" << intVeckk[10] << std::endl;
    // MySTL::vector<int> intVec(20, 1);
    // std::cout << *intVec.begin() << std::endl;
    //3隐式转换为testA类型
    MySTL::vector<testA> testVec(size_t(10), 3);
    MySTL::vector<testA>::iterator c = testVec.begin();
    c->a = 100;
    std::cout << c->a << std::endl;
    MySTL::vector<testA> testVec3(testVec);
    std::cout << testVec3.begin()->a << std::endl;


    MySTL::vector<testA> testVec1(15, testA(111));
    testVec1.resize(10);
    testVec1.shrink_to_fit();
    for(int i=0; i<15; i++) std::cout << testVec1[i].a << " ";
    std::cout << std::endl;
    

    MySTL::vector<int> testVec100(size_t(10), 100);
    //值初始化 调用默认构造函数
    MySTL::vector<int> testVec2 = {};
    for(int i=0; i<testVec2.size(); i++) std::cout << testVec2[i] << " ";
    std::cout << std::endl;
    testVec2 = {10,10,10,10,10,10,10,10,10,10};
    for(int i=0; i<testVec2.size(); i++) std::cout << testVec2[i] << " ";
    std::cout << std::endl;
    testVec2.push_back(20000);
    testVec2.insert(testVec2.begin(), 20001);
    //testVec2.insert(testVec2.end(), 20001);
    testVec2.insert(testVec2.begin(), size_t(3), 20003);
    for(int i=0; i<testVec2.size(); i++) std::cout << testVec2[i] << " ";
    std::cout << std::endl;
    MySTL::vector<int>::iterator c2 = testVec2.begin();
    std::cout<< "size: " << testVec2.size() << "capacity: " << testVec2.capacity() << *(c2) << *(c2+1) << *(c2+2) << *(c2+3) << *(testVec2.end()-1) << std::endl;
    testVec2.resize(10);
    std::cout<< "size: " << testVec2.size() << "capacity: " << testVec2.capacity() << std::endl;
    testVec2.shrink_to_fit();
    std::cout<< "size: " << testVec2.size() << "capacity: " << testVec2.capacity() << std::endl;
    for(int i=0; i<13; i++) std::cout << testVec2[i] << " ";
    std::cout << std::endl;

    testVec1 = testVec;
    std::cout << bool(testVec1 == testVec) << std::endl;
    std::cout << testVec1.capacity() << " " <<testVec1.size() << " " << testVec1.begin()->a << std::endl;

    testVec2.insert(testVec2.begin(), testVec100.begin(), testVec100.end());
    std::cout << "size: " << testVec2.size() << std::endl;

    testVec2.reserve(10);
    std::cout << "capacity: " << testVec2.capacity() << std::endl;
    for(int i=0; i<testVec2.size(); i++) std::cout << testVec2[i] << " ";
    std::cout << std::endl;

    // for (int i = 0; i < 100; i++)
    // {
    //     testVec.push_back(i);
    // }
    // for (auto it = testVec.begin(); it != testVec.end(); it++)
    // {
    //     std::cout << *it << " ";
    // }
    // std::cout << std::endl;
    // destroy(testVec.begin());

    // int str[10] = {0, 1};
    // void * add = str;
    // void * add1 = str+1;
    //std::cout << *static_cast<int *>(add+4) << "      " << str[1] << std::endl;
    int b(122);
    int *aa = new int(b);
    std::cout<< *aa << std::endl;
    std::vector<int> kkk(10);
    std::cout<< kkk[0] << std::endl;

    MySTL::vector<int> vecInt;
    //vecInt: 0, 1, 2, 3, 4, 444
    vecInt.push_back(444);
	for(int i = 0; i != 5; i++){
		vecInt.push_back(i);
	}
	
	//erase the element 444
    auto iter = vecInt.begin();
	for(iter = vecInt.begin(); iter != vecInt.end(); iter++){
		if(*iter == 444){		
			vecInt.erase(iter);		
            break;	
		}
	}
    std:: cout << "iter：";
    std::cout << *iter << std::endl;

    vecInt.swap(testVec2);

    MySTL::vector<Test> v;
    int aaaaa = 0;
    string bbbbb = "test";
    Test tt = Test(aaaaa, bbbbb);
    cout << "--push_back--" << endl;
    v.push_back(tt);
    cout << "--push_back--" << endl;
    v.push_back(Test(aaaaa, bbbbb));


    /*****************************************************************************************
     * hashtable test
    *****************************************************************************************/
    MySTL::hashtable<int,
        int,
        std::hash<int>,
        std::_Identity<int>,
        std::equal_to<int>,
        MySTL::allocator> iht(50,std::hash<int>(), std::equal_to<int>());    //指定保留50个bucket(桶)
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
    MySTL::hashtable<int,
        int,
        std::hash<int>,
        std::_Identity<int>,
        std::equal_to<int>,
        MySTL::allocator>::iterator ite = iht.begin();

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

    for(int i = 0;i<=47;i++)
        iht.insert_equal(i);
    cout<<iht.size()<<endl;        //54
    cout<<iht.bucket_count()<<endl;    //97确实扩容了(重建)

    for (int i; i<iht.bucket_count();i++ )
    {
        int n = iht.elems_in_bucket(i);
        if(n!=0)
            cout<<"bucket["<<i<<"] has"<<n<<" elems."<<endl;
    }
    /*打印的结果
    bucket[2]到bucket[11]的节点个数为2
    其余的bucket[0]~bucket[47]的节点个数为1
    此外，bucket[53],[55],[59],[63]的节点个数均为1*/

    /*以迭代器变量hashtable,将所有节点打印出来*/
    ite = iht.begin();
    for(int i = 0;i<iht.size();i++,ite++)
        cout<< *ite << ' ';
    cout<<endl;

    cout<<*(iht.find(2))<<endl;//2
    cout<<iht.count(2)<<endl;//2


    MySTL::hashtable<int,
        int,
        std::hash<int>,
        std::_Identity<int>,
        std::equal_to<int>,
        MySTL::allocator> iht2(50,std::hash<int>(), std::equal_to<int>());    //指定保留50个bucket(桶)

    cout << (iht!=iht2 ? "true" : "false") << endl;

    return 0;
}