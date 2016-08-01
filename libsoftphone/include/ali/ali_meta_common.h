#pragma once

#include "ali_features.h"

namespace ali
{

namespace meta
{

// ******************************************************************
struct empty_type {};
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
typedef char yes_type;
// ******************************************************************

// ******************************************************************
struct no_type { char _[2]; };
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T, T value>
struct define_result
// ******************************************************************
{
    static T const result = value;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <bool value>
struct define_bool_result
// ******************************************************************
    :   define_result<bool, value> {};

// ******************************************************************    
struct define_true_result
// ******************************************************************
    :   define_bool_result<true> {};
    
// ******************************************************************    
struct define_false_result
// ******************************************************************
    :   define_bool_result<false> {};
    
// ******************************************************************
// ******************************************************************

// ******************************************************************    
template <typename T>
struct define_type
// ******************************************************************
{
    typedef T type;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
T make( void );
// ******************************************************************

// ******************************************************************
// ******************************************************************

#ifdef  ALI_FEATURE_VARIADIC_TEMPLATES

// ******************************************************************
template <int a, int... rest>
struct maxi
// ******************************************************************
    :   define_result<int, maxi<a, maxi<rest...>::result>::result> {};

// ******************************************************************
template <int a>
struct maxi<a>
// ******************************************************************
    :   define_result<int, a> {};

// ******************************************************************
template <int a, int b>
struct maxi<a, b>
// ******************************************************************
    :   define_result<int, (b > a ? b : a)> {};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <int a, int... rest>
struct mini
// ******************************************************************
    :   define_result<int, mini<a, mini<rest...>::result>::result> {};

// ******************************************************************
template <int a>
struct mini<a>
// ******************************************************************
    :   define_result<int, a> {};

// ******************************************************************
template <int a, int b>
struct mini<a, b>
// ******************************************************************
    :   define_result<int, (b < a ? b : a)> {};

// ******************************************************************
// ******************************************************************

#else   //  !ALI_FEATURE_VARIADIC_TEMPLATES

// ******************************************************************
template <int a, int b>
struct maxi
// ******************************************************************
    :   define_result<int, (b > a ? b : a)> {};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <int a, int b>
struct mini
// ******************************************************************
    :   define_result<int, (b < a ? b : a)> {};

// ******************************************************************
// ******************************************************************

#endif  //  ALI_FEATURE_VARIADIC_TEMPLATES

#ifdef  ALI_FEATURE_VARIADIC_TEMPLATES

// ******************************************************************
template <int idx, typename... ts>
struct type_at;
// ******************************************************************

// ******************************************************************
template <typename head, typename... rest>
struct type_at<0, head, rest...>
// ******************************************************************
    : define_type<head> {};

// ******************************************************************
template <int idx, typename head, typename... rest>
struct type_at<idx, head, rest...>
// ******************************************************************
    : define_type<typename type_at<idx - 1, rest...>::type> {};

#endif  //  ALI_FEATURE_VARIADIC_TEMPLATES

}   //  namespace meta

}   //  namespace ali