#ifndef MYTINYSTL_UNORDERED_SET_TEST_H_
#define MYTINYSTL_UNORDERED_SET_TEST_H_

// unordered_set test : 测试 unordered_set 的接口与 insert 的性能

#include <unordered_set>

#include "../../MySTL/unordered_set.hpp"
#include "../test.hpp"

namespace MySTL
{
namespace test
{
namespace unordered_set_test
{

void unordered_set_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[-------------- Run container test : unordered_set -------------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  int a[] = { 5,4,3,2,1 };
  MySTL::unordered_set<int> us1;
  MySTL::unordered_set<int> us2(520);
  MySTL::unordered_set<int> us3(520, std::hash<int>());
  MySTL::unordered_set<int> us4(520, std::hash<int>(), std::equal_to<int>());
  MySTL::unordered_set<int> us5(a, a + 5);
  MySTL::unordered_set<int> us6(a, a + 5, 100);
  MySTL::unordered_set<int> us7(a, a + 5, 100, std::hash<int>());
  MySTL::unordered_set<int> us8(a, a + 5, 100, std::hash<int>(), std::equal_to<int>());
  MySTL::unordered_set<int> us9(us5);
  MySTL::unordered_set<int> us10(std::move(us5));
  MySTL::unordered_set<int> us11;
  us11 = us6;
  MySTL::unordered_set<int> us12;
  us12 = std::move(us6);
  //MySTL::unordered_set<int> us13{ 1,2,3,4,5 };
  MySTL::unordered_set<int> us14;
  //us13 = { 1,2,3,4,5 };

  //FUN_AFTER(us1, us1.emplace(1));
  //FUN_AFTER(us1, us1.emplace_hint(us1.end(), 2));
  FUN_AFTER(us1, us1.insert(5));
  //FUN_AFTER(us1, us1.insert(us1.begin(), 5));
  FUN_AFTER(us1, us1.insert(a, a + 5));
  FUN_AFTER(us1, us1.erase(us1.begin()));
  FUN_AFTER(us1, us1.erase(us1.begin(), us1.find(3)));
  FUN_AFTER(us1, us1.erase(1));
  std::cout << std::boolalpha;
  FUN_VALUE(us1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(us1.size());
  FUN_VALUE(us1.bucket_count());
  FUN_VALUE(us1.max_bucket_count());
  //FUN_VALUE(us1.bucket(1));
  //FUN_VALUE(us1.bucket_size(us1.bucket(5)));
  FUN_AFTER(us1, us1.clear());
  FUN_AFTER(us1, us1.swap(us7));
  FUN_VALUE(*us1.begin());
  std::cout << std::boolalpha;
  FUN_VALUE(us1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(us1.size());
  FUN_VALUE(us1.max_size());
  FUN_VALUE(us1.bucket_count());
  //FUN_AFTER(us1, us1.reserve(1000));
  //FUN_VALUE(*us1.begin(us1.bucket(1)));
  FUN_VALUE(us1.size());
  FUN_VALUE(us1.bucket_count());
  //FUN_VALUE(us1.bucket_size(1));
  //FUN_VALUE(us1.bucket_size(2));
  //FUN_VALUE(us1.bucket_size(3));
  //FUN_AFTER(us1, us1.rehash(150));
  FUN_VALUE(us1.bucket_count());
  FUN_VALUE(us1.count(1));
  FUN_VALUE(*us1.find(3));
  auto first = *us1.equal_range(3).first;
  auto second = *us1.equal_range(3).second;
  std::cout << " us1.equal_range(3) : from " << first << " to " << second << std::endl;
  //FUN_VALUE(us1.load_factor());
  //FUN_VALUE(us1.max_load_factor());
  //FUN_AFTER(us1, us1.max_load_factor(1.5f));
  //FUN_VALUE(us1.max_load_factor());
  PASSED;
#if PERFORMANCE_TEST_ON
  std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|        insert       |";
#if LARGER_TEST_DATA_ON
  CON_TEST_P1(unordered_set<int>, insert, rand(), LEN1 _L, LEN2 _L, LEN3 _L);
#else
  CON_TEST_P1(unordered_set<int>, insert, rand(), LEN1 _M, LEN2 _M, LEN3 _M);
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  PASSED;
#endif
  std::cout << "[-------------- End container test : unordered_set -------------]" << std::endl;
}

} // namespace unordered_set_test
} // namespace test
} // namespace MySTL


int main() {
	MySTL::test::unordered_set_test::unordered_set_test();
	return 0;
}


#endif // !MYTINYSTL_UNORDERED_SET_TEST_H_
