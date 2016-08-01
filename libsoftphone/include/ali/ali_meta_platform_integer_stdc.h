#pragma once


#include "ali_meta_compiler.h"
#include <limits.h>
#include <wchar.h>

namespace ali
{

namespace meta
{

namespace integer
{

// ******************************************************************
template <typename T>
struct max_value;
// ******************************************************************

template <>
struct max_value<char>
    :   define_result<char, CHAR_MAX> {};

template <>
struct max_value<wchar_t>
    :   define_result<wchar_t, WCHAR_MAX> {};

template <>
struct max_value<signed char>
    :   define_result<signed char, SCHAR_MAX> {};
    
template <>
struct max_value<unsigned char>
    :   define_result<unsigned char, UCHAR_MAX> {};
    
template <>
struct max_value<signed short>
    :   define_result<signed short, SHRT_MAX> {};
    
template <>
struct max_value<unsigned short>
    :   define_result<unsigned short, USHRT_MAX> {};
    
template <>
struct max_value<signed int>
    :   define_result<signed int, INT_MAX> {};
    
template <>
struct max_value<unsigned int>
    :   define_result<unsigned int, UINT_MAX> {};

template <>
struct max_value<signed long>
    :   define_result<signed long, LONG_MAX> {};
    
template <>
struct max_value<unsigned long>
    :   define_result<unsigned long, ULONG_MAX> {};

template <>
struct max_value<signed long long>
    :   define_result<signed long long, LLONG_MAX> {};
    
template <>
struct max_value<unsigned long long>
    :   define_result<unsigned long long, ULLONG_MAX> {};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
struct min_value;
// ******************************************************************

template <>
struct min_value<char>
    :   define_result<char, CHAR_MIN> {};

template <>
struct min_value<wchar_t>
    :   define_result<wchar_t, WCHAR_MIN> {};

template <>
struct min_value<signed char>
    :   define_result<signed char, SCHAR_MIN> {};
    
template <>
struct min_value<unsigned char>
    :   define_result<unsigned char, 0> {};
    
template <>
struct min_value<signed short>
    :   define_result<signed short, SHRT_MIN> {};
    
template <>
struct min_value<unsigned short>
    :   define_result<unsigned short, 0> {};
    
template <>
struct min_value<signed int>
    :   define_result<signed int, INT_MIN> {};
    
template <>
struct min_value<unsigned int>
    :   define_result<unsigned int, 0> {};

template <>
struct min_value<signed long>
    :   define_result<signed long, LONG_MIN> {};
    
template <>
struct min_value<unsigned long>
    :   define_result<unsigned long, 0> {};

template <>
struct min_value<signed long long>
    :   define_result<signed long long, LLONG_MIN> {};
    
template <>
struct min_value<unsigned long long>
    :   define_result<unsigned long long, 0> {};

}   //  namespace integer

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
struct size_in_bits
// ******************************************************************
    :   define_result<int, CHAR_BIT * sizeof(T)> {};
    
}   //  namespace meta

}   //  namespace ali
