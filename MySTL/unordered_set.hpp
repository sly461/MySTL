/**
 * unordered_set容器（底层是hashtable）
**/
#ifndef _UNORDERED_SET_H_
#define _UNORDERED_SET_H_

#include <functional>
#include "allocator.hpp"
#include "hashtable.hpp"

namespace MySTL {
    template<class Value, class HashFcn=std::hash<Value>,
             class EqualKey=std::equal_to<Value>, template<class T> class Alloc=allocator>
    class unordered_set {
    private:
        //默认的ExtractKey函数是identity，因为unordered_set的实值（value）即为键值（key）
        using ht = hashtable<Value, Value, HashFcn, std::_Identity<Value>, EqualKey, Alloc>;
        
        ht rep;
    public:
        using key_type = typename ht::key_type;
        using value_type = typename ht::value_type;
        using hasher = typename ht::hasher;
        using key_equal = typename ht::key_equal;
        
        using size_type = typename ht::size_type;
        using difference_type = typename ht::difference_type;
        using pointer = typename ht::const_pointer;
        using const_pointer = typename ht::const_pointer;
        using reference = typename ht::const_reference;
        using const_reference = typename ht::const_reference;

        using iterator = typename ht::const_iterator;
        using const_iterator = typename ht::const_iterator;

        hasher hash_funct() const { return rep.hash_funct(); }
        key_equal key_eq() const { return rep.key_eq(); }

        //构造函数
        

    } 
}



#endif