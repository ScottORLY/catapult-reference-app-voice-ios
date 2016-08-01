/*
 *  ali_shared_ptr_intrusive.h
 *  ali Library
 *
 *  Copyright 2013 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

//#include "ali_auto_ptr.h"
#include "ali_exception_memory.h"
#include "ali_nullptr.h"
#include "ali_safebool.h"
#include "ali_typeof.h"
#include "ali_utility.h"

namespace ali
{

// ******************************************************************
template <typename T>
class shared_ptr_intrusive
// ******************************************************************
{
public:     // Typedefs
    typedef T element_type;

public:
    shared_ptr_intrusive( void ) {}
    
    shared_ptr_intrusive( ali::nullptr_type ) {}

    explicit shared_ptr_intrusive( T* ptr )
    :   _ptr{ptr}
    {
        if ( _ptr != nullptr )
            _ptr->retain();
    }

    template <typename U>
    shared_ptr_intrusive( shared_ptr_intrusive<U> const& b )
    :   _ptr{b._ptr}
    {
        if ( _ptr != nullptr )
            _ptr->retain();
    }

    shared_ptr_intrusive( shared_ptr_intrusive const& b )
    :   _ptr{b._ptr}
    {
        if ( _ptr != nullptr )
            _ptr->retain();
    }

#if defined(ALI_FEATURE_RVALUE_REF)
    template <typename U>
    shared_ptr_intrusive( shared_ptr_intrusive<U>&& b )
    :   _ptr{b._ptr}
    {
        b._ptr = nullptr;
    }

    shared_ptr_intrusive( shared_ptr_intrusive&& b )
    :   _ptr{b._ptr}
    {
        b._ptr = nullptr;
    }
#endif

    //template <typename U>
    //shared_ptr_intrusive( ali::auto_ptr<U> b )
    //:   _ptr{b.get()}
    //{
    //    if ( _ptr != nullptr )
    //        _ptr->retain();
    //}

    ~shared_ptr_intrusive( void )
    {
        if ( _ptr != nullptr )
            _ptr->release();
    }

    template <typename U>
    shared_ptr_intrusive& operator=( shared_ptr_intrusive<U> const& b )
    {
        shared_ptr_intrusive{b}.swap(*this);
        return *this;
    }

    shared_ptr_intrusive& operator=( shared_ptr_intrusive b )
    {
        swap(b);
        return *this;
    }

    T* get( void ) const
    {
        return _ptr;
    }

    void reset( T* ptr = nullptr )
    {
        shared_ptr_intrusive{ptr}.swap(*this);
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

    template <typename U>
    bool operator==( shared_ptr_intrusive<U> const& b ) const
    {
        return _ptr == b.get();
    }

    template <typename U>
    bool operator!=( shared_ptr_intrusive<U> const& b ) const
    {
        return _ptr != b.get();
    }

    template <typename U>
    bool operator<( shared_ptr_intrusive<U> const& b ) const
    {
        return _ptr < b.get();
    }

    template <typename U>
    bool operator<=( shared_ptr_intrusive<U> const& b ) const
    {
        return _ptr <= b.get();
    }

    template <typename U>
    bool operator>( shared_ptr_intrusive<U> const& b ) const
    {
        return _ptr > b.get();
    }

    template <typename U>
    bool operator>=( shared_ptr_intrusive<U> const& b ) const
    {
        return _ptr >= b.get();
    }

    void swap( shared_ptr_intrusive& b )
    {
        using ali::swap;
        swap(_ptr, b._ptr);
    }

    friend void swap( shared_ptr_intrusive& a, shared_ptr_intrusive& b )
    {
        a.swap(b);
    }

private:    // Data members
    T*  _ptr{};
};


// ******************************************************************
template <typename T>
inline ali::shared_ptr_intrusive<T> make_shared_ptr_intrusive( T* t )
// ******************************************************************
{
    return ali::shared_ptr_intrusive<T>{t};
}

#if defined(ALI_FEATURE_VARIADIC_TEMPLATES) && defined(ALI_FEATURE_RVALUE_REF)
// ******************************************************************
template <typename T,
          typename... Params>
inline ali::shared_ptr_intrusive<T> new_shared_ptr_intrusive( Params&&... params )
// ******************************************************************
{
    return ali::shared_ptr_intrusive<T>{out_of_memory_if_null(
        new T{ali::forward<Params>(params)...}, ALI_HERE)};
}
#endif

}   //  namespace ali
