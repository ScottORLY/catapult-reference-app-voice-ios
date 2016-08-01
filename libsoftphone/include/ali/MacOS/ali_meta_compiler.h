#pragma once

#include "ali_meta_common.h"
#include <type_traits>

namespace ali
{

namespace meta
{

// ******************************************************************
template <bool condition, typename _true, typename _false>
struct select;
// ******************************************************************

template <typename _true, typename _false>
struct select<false, _true, _false>
    :   define_type<_false> {};

template <typename _true, typename _false>
struct select<true, _true, _false>
    :   define_type<_true> {};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <
    bool condition,
    typename param,
    template <typename> class _true,
    template <typename> class _false>
struct template_select;
// ******************************************************************

template <
    typename param,
    template <typename> class _true,
    template <typename> class _false>
struct template_select<false, param, _true, _false>
    :   define_type<_false<param> > {};

template <
    typename param,
    template <typename> class _true,
    template <typename> class _false>
struct template_select<true, param, _true, _false>
    :   define_type<_true<param> > {};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <bool a, bool b>
struct _or;
// ******************************************************************

template <bool b>
struct _or<true, b>
    :   define_bool_result<true> {};

template <bool b>
struct _or<false, b>
    :   define_bool_result<b> {};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <bool a, bool b>
struct _and;
// ******************************************************************
template <bool b>
struct _and<false, b>
    :   define_bool_result<false> {};

template <bool b>
struct _and<true, b>
    :   define_bool_result<b> {};

// ******************************************************************
// ******************************************************************

template <typename T>
struct is_pod
    :   define_bool_result<std::is_pod<T>::value> {};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
struct is_pointer
// ******************************************************************
    :   define_false_result {};

template <typename T>
struct is_pointer<T*>
    :   define_true_result {};
// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
struct is_const
// ******************************************************************
    :   define_false_result {};

template <typename T>
struct is_const<T const>
    :   define_true_result {};
// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
struct is_volatile
// ******************************************************************
    :   define_false_result {};

template <typename T>
struct is_volatile<T volatile>
    :   define_true_result {};
// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
struct remove_c
// ******************************************************************
    :   define_type<T> {};
    
template <typename T>
struct remove_c<T const>
    :   define_type<T> {};
// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
struct remove_v
// ******************************************************************
    :   define_type<T> {};
    
template <typename T>
struct remove_v<T volatile>
    :   define_type<T> {};
// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
struct is_ref
// ******************************************************************
    :   define_false_result {};

template <typename T>
struct is_ref<T&>
    :   define_true_result {};
// ******************************************************************

// ******************************************************************
// ******************************************************************
template <typename T>
struct remove_ref
// ******************************************************************
    :   define_type<T> {};

template <typename T>
struct remove_ref<T&>
    :   define_type<T> {};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T, typename U>
struct is_same_type
// ******************************************************************
    :   define_false_result {};

template <typename T>
struct is_same_type<T, T>
    :   define_true_result {};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template<bool,
         typename T = void>
struct enable_if {};
// ******************************************************************

template <typename T>
struct enable_if<true, T>
    :   define_type<T> {};

// ******************************************************************
// ******************************************************************

}   //  namespace meta

}   //  namespace ali
