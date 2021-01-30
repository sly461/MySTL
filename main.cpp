#include <iostream>
#include <vector>
#include "MySTL/vector.hpp"


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
    MySTL::vector<testA> testVec(10, testA(3));
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
    MySTL::vector<int> testVec2(size_t(10), 1);
    //testVec2.push_back(20000);
    //testVec2.insert(testVec2.end(), 20001);
    testVec2.insert(testVec2.begin(), size_t(3), 20003);
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
    int *aa = new int();
    std::cout<< *aa << std::endl;
    std::vector<int> kkk(10);
    std::cout<< kkk[0] << std::endl;

    return 0;
}