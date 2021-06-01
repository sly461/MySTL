#include <iostream>
#include <string>
#include "../MySTL/allocator.hpp"
#include "../MySTL/AVLTree.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::_Identity;
using std::less;
using MySTL::AVLTree;
using MySTL::allocator;

int main() {
    int i;
    int ia[5] = {0,1,2,3,4};

    AVLTree<int, int, _Identity<int>, less<int>, allocator> at;
    at.insert_unique(0);
    at.insert_unique(1);
    at.insert_unique(2);
    at.insert_unique(3);
    at.insert_unique(4);

    cout<<"size="<<at.size() <<endl;        //5
    cout<<"3 count=" <<at.count(3) <<endl;  //1
    at.insert_unique(3);
    cout<<"size="<<at.size() <<endl;        //5
    cout<<"3 count=" <<at.count(3) <<endl;  //1
    at.insert_unique(5);            
    cout<<"size="<<at.size() <<endl;        //6
    cout<<"3 count=" <<at.count(3) <<endl;  //1
    at.erase(1);            
    cout<<"size="<<at.size() <<endl;        //5
    cout<<"3 count=" <<at.count(3) <<endl;  //1
    cout<<"1 count=" <<at.count(1) <<endl;  //0

    AVLTree<int, int, _Identity<int>, less<int>, allocator>::iterator ite1 = at.begin();
    AVLTree<int, int, _Identity<int>, less<int>, allocator>::iterator ite2 = at.end();

    for(; ite1!=ite2; ++ite1)
        cout << *ite1;
    cout << endl;                   //02345

    ite1 = at.find(3);
    if(ite1 != at.end()) cout << "3 found" <<endl;
    at.erase(ite1);
    ite1 = at.find(3);
    if(ite1 == at.end()) cout << "3 not found" <<endl;

    ite1 = at.find(1);
    if(ite1 == at.end()) cout << "1 not found" <<endl;

    AVLTree<int, int, _Identity<int>, less<int>, allocator> at2;

    at2 = at;
    cout << *at2.lower_bound(1) << endl;
    
    if(at == at2) cout << "at=at2" <<endl;

    return 0;
}