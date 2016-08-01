/*
 *  ali_memory.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_features.h"
#include "ali_meta.h"
#include "ali_new.h"
#include "ali_utility.h"

namespace ali
{

// ******************************************************************
void* allocate( size_t size );
// ******************************************************************

// ******************************************************************
void* reallocate( void* p, size_t size );
// ******************************************************************

// ******************************************************************
void deallocate( void* p );
// ******************************************************************

#ifdef  ALI_FEATURE_RVALUE_REF

// ******************************************************************
template <typename T>
inline typename meta::base<T>::type* construct( void* p, T&& val )
// ******************************************************************
{
    return new(p) typename meta::base<T>::type(ali::forward<T>(val));
}

#else   //  !ALI_FEATURE_RVALUE_REF

// ******************************************************************
template <typename T>
inline T* construct( void* p, T const& val )
// ******************************************************************
{
    return new(p) T(val);
}

#endif  //  ALI_FEATURE_RVALUE_REF

// ******************************************************************
template <typename T>
inline T* construct( void* p )
// ******************************************************************
{
    return new(p) T();
}

// ******************************************************************
template <typename T>
inline void destroy( T const& p )
// ******************************************************************
{
    p.~T();
}

// ******************************************************************
template <typename T>
inline void destroy( T* const& ) {}
// ******************************************************************

}   //  namespace ali
