#include <iostream>
#include <cstring> 
#include <functional>
#include "../MySTL/unordered_set.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::hash;
using MySTL::unordered_set;

struct eqstr
{
	bool operator() (const char* s1, const char* s2) const
	{
		return strcmp(s1, s2) == 0;
	}
};

void setFind(const unordered_set<const char*, hash<const char*>, eqstr>& s, const char* word)
{
	unordered_set<const char*, hash<const char*>, eqstr>::const_iterator it
		= s.find(word);
	
	cout << word << ": " << (it != s.end() ? "present" : "not present") << endl;
}

int main()
{
	unordered_set<const char*, hash<const char*>, eqstr> s;

	cout << s.size() << endl;

	s.insert("why");
	s.insert("always");
	s.insert("me");
	s.insert("star");
	s.insert("steve");

	cout << s.size() << endl;
	
	setFind(s, "always");
	setFind(s, "apple");

	unordered_set<const char*, hash<const char*>, eqstr>::iterator it = s.begin();
	for (; it != s.end(); it++) {
		cout << *it << endl;
	}

	int test[]= {0,1,2,3};
	unordered_set<int> s1(test, test+4);
	unordered_set<int> s2(s1);
	//s2 = s1;
	for(auto i: s2) cout << i;
	cout<<endl;

	
	return 0;
}