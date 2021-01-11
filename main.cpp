#include <iostream>
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
};

int main()
{
    MySTL::vector<testA> testVec(10, 3);
    MySTL::vector<testA>::iterator c = testVec.begin();
    c+=9;
    std::cout << c->a;
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
    

    return 0;
}