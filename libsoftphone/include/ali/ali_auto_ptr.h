/*
 *  ali_auto_ptr.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_auto_ptr_forward.h"
#include "ali_deprecated.h"
#include "ali_exception_memory.h"
#include "ali_features.h"
#include "ali_meta.h"
#include "ali_nullptr.h"
#include "ali_safebool.h"
#include "ali_utility.h"

namespace ali
{

// ******************************************************************
template <typename T>
class auto_ptr
// ******************************************************************
{
public:
    typedef T element_type;

    auto_ptr( void ) {};
    
    auto_ptr( ali::nullptr_type ) {}

    explicit auto_ptr( T* ptr )
    :   _ptr{ptr}
    {}

    template <typename U>
    auto_ptr( auto_ptr<U> const& b )
    :   _ptr{b.release()}
    {}

    auto_ptr( auto_ptr const& b )
    :   _ptr{b.release()}
    {}

    ~auto_ptr( void )
    {
        delete _ptr;
    }

    template <typename U>
    auto_ptr& operator=( auto_ptr<U> const& b )
    {
        reset(b.release());
        return *this;
    }

    auto_ptr& operator=( auto_ptr const& b )
    {
        reset(b.release());
        return *this;
    }

    T& operator*( void ) const
    {
        ali_assert(_ptr != nullptr);
        return *_ptr;
    }

    T* operator->( void ) const
    {
        ali_assert(_ptr != nullptr);
        return _ptr;
    }

    bool is_null( void ) const
    {
        return _ptr == nullptr;
    }

    operator safebool( void ) const
    {
        return safeboolize(!is_null());
    }

    bool operator!( void ) const
    {
        return is_null();
    }

    T* get( void ) const
    {
        return _ptr;
    }

    T* release( void ) const
    {
        T* const t{_ptr};
        _ptr = nullptr;
        return t;
    }

    T* reset( T* ptr = nullptr ) const
    {
        if ( ptr != _ptr )
            delete _ptr;
        return _ptr = ptr;
    }

    void swap( auto_ptr& b )
    {
        using ali::swap;
        swap(_ptr, b._ptr);
    }

    friend void swap( auto_ptr& a, auto_ptr& b )
    {
        a.swap(b);
    }

private:
    mutable T* _ptr{};
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class auto_ptr<T[]>
// ******************************************************************
{
public:
    typedef T element_type;

    auto_ptr( void ) {};
    
    auto_ptr( ali::nullptr_type ) {}

    explicit auto_ptr( T* ptr )
    :   _ptr{ptr}
    {}

    explicit auto_ptr( int size )
    :   _ptr{out_of_memory_if_null(
            new T[size], ALI_HERE)}
    {}

    auto_ptr( auto_ptr const& b )
    :   _ptr{b.release()}
    {}

    ~auto_ptr( void )
    {
        delete[] _ptr;
    }

    auto_ptr& operator=( auto_ptr const& b )
    {
        reset(b.release());
        return *this;
    }

    bool is_null( void ) const
    {
        return _ptr == nullptr;
    }

    operator safebool( void ) const
    {
        return safeboolize(!is_null());
    }

    bool operator!( void ) const
    {
        return is_null();
    }

    T& operator[]( int i ) const
    {
        ali_assert(_ptr != nullptr);
        return _ptr[i];
    }

    T* get( void ) const
    {
        return _ptr;
    }

    T* release( void ) const
    {
        T* const t{_ptr};
        _ptr = nullptr;
        return t;
    }

    T* reset( T* ptr = nullptr ) const
    {
        if ( ptr != _ptr )
            delete[] _ptr;
        return _ptr = ptr;
    }

    void swap( auto_ptr& b )
    {
        using ali::swap;
        swap(_ptr, b._ptr);
    }

    friend void swap( auto_ptr& a, auto_ptr& b )
    {
        a.swap(b);
    }

private:
    mutable T* _ptr{};
};

// ******************************************************************
// ******************************************************************

namespace meta
{

// ******************************************************************
template <typename T>
struct effective<ali::auto_ptr<T> >
// ******************************************************************
    :   define_type<ali::auto_ptr<T> > {};

// ******************************************************************
// ******************************************************************

}   //  namespace meta

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class auto_ptr_lvalue
// ******************************************************************
{
public:
    auto_ptr_lvalue( T*& ptr )
    :   _ptr(ptr)
    {}

    operator T*( void ) const { return _ptr; }
    T* operator->( void ) const { return _ptr; }

    auto_ptr_lvalue<T>& operator =( ali::auto_ptr<T> b )
    {
        return operator =(b.release());
    }

    auto_ptr_lvalue<T>& operator =( T* b )
    {
        delete _ptr;
        _ptr = b;
        return *this;
    }

#if defined(ALI_FEATURE_DELETED_FUNCTIONS)
    void operator =(auto_ptr_lvalue<T> const&) = delete;
#else
private:
    void operator =(auto_ptr_lvalue<T> const&);
#endif

private:
    T*& _ptr;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline ali::auto_ptr<T> make_auto_ptr( T* t )
// ******************************************************************
{
    return ali::auto_ptr<T>{t};
}

#if defined(ALI_FEATURE_VARIADIC_TEMPLATES) && defined(ALI_FEATURE_RVALUE_REF)
// ******************************************************************
template <typename T, typename... Params>
inline ali::auto_ptr<T> new_auto_ptr( Params&&... params )
// ******************************************************************
{
    return ali::auto_ptr<T>{out_of_memory_if_null(
        new T{ali::forward<Params>(params)...}, ALI_HERE)};
}
#endif

}   //  namespace ali
