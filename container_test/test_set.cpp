#include <iostream>
#include <string>
#include "../MySTL/allocator.hpp"
#include "../MySTL/set.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using MySTL::set;
using MySTL::allocator;

int main() {
    int i;
    int ia[16] = {3,2,1,4,5,6,7,16,15,14,13,12,11,10,8,9};

    set<int> s(ia, ia+16);

    cout<<"size="<<s.size() <<endl;        //5
    cout<<"3 count=" <<s.count(3) <<endl;  //1
    s.insert(3);
    cout<<"size="<<s.size() <<endl;        //5
    cout<<"3 count=" <<s.count(3) <<endl;  //1
    s.insert(5);            
    cout<<"size="<<s.size() <<endl;        //6
    cout<<"3 count=" <<s.count(3) <<endl;  //1
    s.erase(1);            
    cout<<"size="<<s.size() <<endl;        //5
    cout<<"3 count=" <<s.count(3) <<endl;  //1
    cout<<"1 count=" <<s.count(1) <<endl;  //0

    set<int>::iterator ite1 = s.begin();
    set<int>::iterator ite2 = s.end();

    for(; ite1!=ite2; ++ite1)
        cout << *ite1;
    cout << endl;               //02345

    ite1 = s.find(3);
    if(ite1 != s.end()) cout << "3 found" <<endl;

    ite2 = s.find(0);
    if(ite2!=s.end())s.erase(ite2, ite1);

    ite1 = s.begin();
    ite2 = s.end();
    for(; ite1!=ite2; ++ite1)
        cout << *ite1;
    cout << endl;               //345

    ite1 = s.find(3);
    if(ite1 == s.end()) cout << "3 not found" <<endl;

    ite1 = s.find(1);
    if(ite1 == s.end()) cout << "1 not found" <<endl;

    set<int> s2;

    s2 = s;
    cout << *s2.lower_bound(2) << endl;
    cout << (s2.upper_bound(5)==s2.end() ? "dont have" : "have") << endl;
    
    if(s == s2) cout << "at=at2" <<endl;

    return 0;
}