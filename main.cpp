#include <iostream>
#include <vector>
#include "MySTL/vector.hpp"
//#include "MySTL/allocator.hpp"


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
        testA(): a(0) {}
        testA(int _a): a(_a) {}
        testA(const testA & ta) { a = ta.a; std::cout<<"testA copy constructor!"<<std::endl; }
};

int main()
{
    MySTL::vector<testA> testVec(10, testA(3));
    MySTL::vector<testA>::iterator c = testVec.begin();
    c->a = 100;
    std::cout << c->a << std::endl;
    MySTL::vector<testA> testVec3(testVec);
    std::cout << testVec3.begin()->a << std::endl;


    MySTL::vector<testA> testVec1(15, testA());
    MySTL::vector<testA>::iterator c1 = testVec1.begin();
    std::cout << c1->a << std::endl;
    
    MySTL::vector<int> testVec2(10, 1);
    MySTL::vector<int>::iterator c2 = testVec2.begin();
    std::cout<< *c2 << std::endl;

    testVec1 = testVec;
    std::cout << testVec1.capacity() << " " <<testVec1.size() << " " << testVec1.begin()->a << std::endl;

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