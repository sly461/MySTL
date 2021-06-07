#ifndef MYTINYSTL_SET_TEST_H_
#define MYTINYSTL_SET_TEST_H_

// set test : 测试 set 的接口与 insert 的性能

#include <set>

#include "../../MySTL/set.hpp"
#include "../test.hpp"

namespace MySTL
{
namespace test
{
namespace set_test
{

void set_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[------------------ Run container test : set -------------------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  int a[] = { 5,4,3,2,1 };
  MySTL::set<int> s1;
  MySTL::set<int, std::greater<int>> s2;
  MySTL::set<int> s3(a, a + 5);
  MySTL::set<int> s4(a, a + 5);
  MySTL::set<int> s5(s3);
  MySTL::set<int> s6(std::move(s3));
  MySTL::set<int> s7;
  s7 = s4;
  MySTL::set<int> s8;
  s8 = std::move(s4);
//   MySTL::set<int> s9{ 1,2,3,4,5 };
//   MySTL::set<int> s10;
//   s10 = { 1,2,3,4,5 };

  for (int i = 5; i > 0; --i)
  {
    FUN_AFTER(s1, s1.insert(i));
  }
  //FUN_AFTER(s1, s1.emplace_hint(s1.begin(), 0));
  FUN_AFTER(s1, s1.erase(s1.begin()));
  FUN_AFTER(s1, s1.erase(0));
  FUN_AFTER(s1, s1.erase(1));
  FUN_AFTER(s1, s1.erase(s1.begin(), s1.end()));
  for (int i = 0; i < 5; ++i)
  {
    FUN_AFTER(s1, s1.insert(i));
  }
  FUN_AFTER(s1, s1.insert(a, a + 5));
  FUN_AFTER(s1, s1.insert(5));
  //FUN_AFTER(s1, s1.insert(s1.end(), 5));
  FUN_VALUE(s1.count(5));
  FUN_VALUE(*s1.find(3));
  FUN_VALUE(*s1.lower_bound(3));
  FUN_VALUE(*s1.upper_bound(3));
  auto first = *s1.equal_range(3).first;
  auto second = *s1.equal_range(3).second;
  std::cout << " s1.equal_range(3) : from " << first << " to " << second << std::endl;
  FUN_AFTER(s1, s1.erase(s1.begin()));
  FUN_AFTER(s1, s1.erase(1));
  FUN_AFTER(s1, s1.erase(s1.begin(), s1.find(3)));
  FUN_AFTER(s1, s1.clear());
  FUN_AFTER(s1, s1.swap(s5));
  FUN_VALUE(*s1.begin());
  //FUN_VALUE(*s1.rbegin());
  std::cout << std::boolalpha;
  FUN_VALUE(s1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(s1.size());
  FUN_VALUE(s1.max_size());
  PASSED;
#if PERFORMANCE_TEST_ON
  std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|        insert       |";
#if LARGER_TEST_DATA_ON
  CON_TEST_P1(set<int>, insert, rand(), LEN1 _L, LEN2 _L, LEN3 _L);
#else
  CON_TEST_P1(set<int>, insert, rand(), LEN1 _M, LEN2 _M, LEN3 _M);
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  PASSED;
#endif
  std::cout << "[------------------ End container test : set -------------------]" << std::endl;
}

} // namespace set_test
} // namespace test
} // namespace MySTL


int main() {
	MySTL::test::set_test::set_test();
	return 0;
}

#endif // !MYTINYSTL_SET_TEST_H_
