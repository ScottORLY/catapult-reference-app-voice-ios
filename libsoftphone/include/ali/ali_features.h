/*
 *  ali_features.h
 *  ali library
 *
 *  Copyright 2013 Acrobits, s.r.o. All rights reserved.
 */

#pragma once

// C++11 standard features
#if __cplusplus >= 201103L
#   define ALI_FEATURE_VARIADIC_TEMPLATES
#   define ALI_FEATURE_RVALUE_REF
#   define ALI_FEATURE_CONSTEXPR
#   define ALI_FEATURE_NULLPTR
#   define ALI_FEATURE_INITIALIZER_LIST
#   define ALI_FEATURE_DELETED_FUNCTIONS
#   define ALI_FEATURE_OVERRIDE
#endif

// GCC when in experimental C++0x mode
#if defined(__GNUC__) && defined(__GXX_EXPERIMENTAL_CXX0X__)
#   if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3))
#       define ALI_FEATURE_RVALUE_REF
#       define ALI_FEATURE_VARIADIC_TEMPLATES
#   endif
#   if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 4))
#       define ALI_FEATURE_INITIALIZER_LIST
#   endif
#   if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6))
#       define ALI_FEATURE_CONSTEXPR
#       define ALI_FEATURE_NULLPTR
#       define ALI_FEATURE_DELETED_FUNCTIONS
#   endif
#   if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7))
#       define ALI_FEATURE_OVERRIDE
#   endif
#endif

// Clang has this nice way to tell us
#if !defined(__has_extension) && defined(__has_feature)
#   define __has_extension __has_feature
#endif
#if defined(__has_extension)
#   if __has_extension(cxx_rvalue_references)
#       define ALI_FEATURE_RVALUE_REF
#   endif
#   if __has_extension(cxx_variadic_templates)
#       define ALI_FEATURE_VARIADIC_TEMPLATES
#   endif
#   if __has_extension(cxx_constexpr)
#       define ALI_FEATURE_CONSTEXPR
#   endif
#   if __has_extension(cxx_nullptr)
#       define ALI_FEATURE_NULLPTR
#   endif
#   if __has_extension(cxx_initializer_list)
#       define ALI_FEATURE_INITIALIZER_LIST
#   endif
#   if __has_extension(cxx_deleted_functions)
#       define ALI_FEATURE_DELETED_FUNCTIONS
#   endif
#   if __has_extension(cxx_override_control)
#       define ALI_FEATURE_OVERRIDE
#   endif
#endif

#if defined(_MSC_VER)
#   if _MSC_VER >= 1600 // MS Visual C++ 2010 (16.00)
#       define ALI_FEATURE_RVALUE_REF
#       define ALI_FEATURE_NULLPTR
#   endif
#   if _MSC_VER >= 1700 //  MS Visual C++ 2012
#   endif
#   if _MSC_VER >= 1800 //  MS Visual C++ 2013
#       define ALI_FEATURE_VARIADIC_TEMPLATES
#       define ALI_FEATURE_INITIALIZER_LIST
#       define ALI_FEATURE_DELETED_FUNCTIONS
#       define ALI_FEATURE_OVERRIDE
#   endif
#endif
