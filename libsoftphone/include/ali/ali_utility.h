/*
 *  ali_utility.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_utility_forward.h"
#include "ali_features.h"
#include "ali_iterator.h"
#include "ali_meta.h"
#include "ali_wchar.h"
#include "ali_xrange.h"


#if defined(ALI_FEATURE_CONSTEXPR)
#   define ALI_CONSTEXPR constexpr
#else
#   define ALI_CONSTEXPR
#endif


#if defined(ALI_FEATURE_RVALUE_REF)

namespace ali
{

    // ******************************************************************
    template <typename T>
    ALI_CONSTEXPR inline T&& forward( typename ali::meta::remove_ref<T>::type & t )
    // ******************************************************************
    {
        return static_cast<T&&>(t);
    }

    // ******************************************************************
    template <typename T>
    ALI_CONSTEXPR inline T&& forward( typename ali::meta::remove_ref<T>::type && t )
    // ******************************************************************
    {
        return static_cast<T&&>(t);
    }

    // ******************************************************************
    template <typename T>
    ALI_CONSTEXPR inline typename ali::meta::remove_ref<T>::type && move( T && t )
    // ******************************************************************
    {
        return static_cast<typename ali::meta::remove_ref<T>::type &&>(t);
    }

    // ******************************************************************
    template <typename T>
    ALI_CONSTEXPR inline typename ali::meta::remove_ref<T>::type && move_if_noexcept( T && t )
    // ******************************************************************
    //  ISSUE:  Always move, for now.
    // ******************************************************************
    {
        return static_cast<typename ali::meta::remove_ref<T>::type &&>(t);
    }

}   //  namespace ali

#else   //  !defined(ALI_FEATURE_RVALUE_REF)

namespace ali
{

    // ******************************************************************
    template <typename T>
    ALI_CONSTEXPR inline T forward( T t )
    // ******************************************************************
    {
        return t;
    }

    // ******************************************************************
    template <typename T>
    ALI_CONSTEXPR inline T & move( T & t )
    // ******************************************************************
    {
        return t;
    }

}   //  namespace ali

#endif




namespace ali
{

// ******************************************************************
template <typename T, int n>
ALI_CONSTEXPR inline int array_size( T(&)[n] )
// ******************************************************************
{
    return n;
}

// ******************************************************************
inline void noop( void )
// ******************************************************************
{}

#if defined(ALI_FEATURE_VARIADIC_TEMPLATES)
// ******************************************************************
template <typename Function,
          typename... T>
inline void for_each( Function && func,
                      T &&... values )
// ******************************************************************
{
    int dummy[sizeof...(values)] = { (func(values), 0)... };
    (void)dummy;
}
#endif

// ******************************************************************
template <typename T>
inline void swap_by_copy( T& x, T& y )
// ******************************************************************
{
    T const t(x); x = y; y = t;
}

#define ALI_DEFINE_SWAP_FOR(Type)           \
    inline void swap( Type& x, Type& y )    \
    {                                       \
        ali::swap_by_copy(x, y);            \
    }

ALI_DEFINE_SWAP_FOR(bool)
ALI_DEFINE_SWAP_FOR(char)
ALI_DEFINE_SWAP_FOR(signed char)
ALI_DEFINE_SWAP_FOR(unsigned char)
ALI_DEFINE_SWAP_FOR(wchar)
ALI_DEFINE_SWAP_FOR(short)
ALI_DEFINE_SWAP_FOR(unsigned short)
ALI_DEFINE_SWAP_FOR(int)
ALI_DEFINE_SWAP_FOR(unsigned int)
ALI_DEFINE_SWAP_FOR(long)
ALI_DEFINE_SWAP_FOR(unsigned long)
ALI_DEFINE_SWAP_FOR(long long)
ALI_DEFINE_SWAP_FOR(unsigned long long)
ALI_DEFINE_SWAP_FOR(float)
ALI_DEFINE_SWAP_FOR(double)
ALI_DEFINE_SWAP_FOR(long double)

#undef  ALI_DEFINE_SWAP_FOR

// ******************************************************************
template <typename T>
inline void swap( T*& x, T*& y )
// ******************************************************************
{
    ali::swap_by_copy(x, y);
}


// ******************************************************************
template <typename T, unsigned N>
inline void swap( T(&x)[N], T(&y)[N] )
// ******************************************************************
{
    using ali::swap;
    for ( unsigned i = N; i != 0; )
        --i, swap(x[i], y[i]);
}

namespace detail
{

// ******************************************************************
template <typename FI1, typename FI2, typename T>
inline void iter_swap( FI1 x, FI2 y, T* )
// ******************************************************************
{
    T const t(*x); *x = *y; *y = t;
}

}   //  namespace detail

// ******************************************************************
template <typename FI1, typename FI2>
inline void iter_swap( FI1 x, FI2 y )
// ******************************************************************
{
    detail::iter_swap(x, y, get_iterator_value_ptr(x));
}


// ******************************************************************
template <typename FI1, typename FI2>
inline FI2 swap_ranges( FI1 it, FI1 end, FI2 x )
// ******************************************************************
{
    for ( ; it != end; ++it, ++x )
        iter_swap(it, x);
    return x;
}


// ******************************************************************
template <typename T>
inline T const& mini( T const& x, T const& y )
// ******************************************************************
{
    return x < y ? x : y;
}


// ******************************************************************
template <typename T>
inline T const& maxi( T const& x, T const& y )
// ******************************************************************
{
    return y < x ? x : y;
}

// ******************************************************************
// ******************************************************************

//  A prvalue of type bool can be converted to a prvalue of type int,
//  with false becoming zero and true becoming one.
//      -- ISO/IEC 14882:2011
//
//  a < b ? -1 : b < a ? 1 : 0 == (a > b) - (a < b)
//
#define ALI_DEFINE_COMPARE_FOR(Type)        \
    inline int compare( Type a, Type b )    \
    {                                       \
        return (a > b) - (a < b);           \
    }

ALI_DEFINE_COMPARE_FOR(char)
ALI_DEFINE_COMPARE_FOR(signed char)
ALI_DEFINE_COMPARE_FOR(unsigned char)
ALI_DEFINE_COMPARE_FOR(wchar)
ALI_DEFINE_COMPARE_FOR(short)
ALI_DEFINE_COMPARE_FOR(unsigned short)
ALI_DEFINE_COMPARE_FOR(int)
ALI_DEFINE_COMPARE_FOR(unsigned int)
ALI_DEFINE_COMPARE_FOR(long)
ALI_DEFINE_COMPARE_FOR(unsigned long)
ALI_DEFINE_COMPARE_FOR(long long)
ALI_DEFINE_COMPARE_FOR(unsigned long long)
ALI_DEFINE_COMPARE_FOR(float)
ALI_DEFINE_COMPARE_FOR(double)
ALI_DEFINE_COMPARE_FOR(long double)
template <typename T>
ALI_DEFINE_COMPARE_FOR(T const*)

#undef  ALI_DEFINE_COMPARE_FOR

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct default_comparator
// ******************************************************************
{
    template <typename T, typename U>
    int operator()( T const& a, U const& b ) const
    {
        using ali::compare;
        return compare(a, b);
    }

    bool operator==( default_comparator const& ) const
    {
        return true;
    }

    bool operator!=( default_comparator const& b ) const
    {
        return !operator==(b);
    }

    void swap( default_comparator& ) {}

    friend void swap(
        default_comparator&,
        default_comparator& ) {}
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct less
// ******************************************************************
{
    template <typename T, typename U>
    bool operator()( T const& a, U const& b ) const
    {
        return a < b;
    }

    friend bool operator==( less, less ) { return true; }
    friend bool operator!=( less, less ) { return false; }

    void swap( less& ) {}
    friend void swap( less&, less& ) {}
};

// ******************************************************************
struct deref_less
// ******************************************************************
{
    template <typename T, typename U>
    bool operator()( T const& a, U const& b ) const
    {
        return *a < *b;
    }

    friend bool operator==( deref_less, deref_less ) { return true; }
    friend bool operator!=( deref_less, deref_less ) { return false; }

    void swap( deref_less& ) {}
    friend void swap( deref_less&, deref_less& ) {}
};

// ******************************************************************
template <typename comparator>
struct less_from_comparator
// ******************************************************************
{
    less_from_comparator( void ) {}

    explicit less_from_comparator( comparator cmp )
    :   _cmp(cmp)
    {}
    
    template <typename T, typename U>
    bool operator()( T const& a, U const& b ) const
    {
        return _cmp(a, b) < 0;
    }

    template <typename T, typename U>
    bool operator()( T const* a, U const* b ) const
    {
        return _cmp(*a, *b) < 0;
    }

    friend bool operator==(
        less_from_comparator,
        less_from_comparator )
    {
        return true;
    }

    friend bool operator!=(
        less_from_comparator a,
        less_from_comparator b )
    {
        return !(a == b);
    }

    void swap( less_from_comparator& b )
    {
        using ali::swap;
        swap(_cmp, b._cmp);
    }

    friend void swap(
        less_from_comparator& a,
        less_from_comparator& b )
    {
        a.swap(b);
    }
    
    comparator _cmp;
};


// ******************************************************************
template <typename T>
struct less_from_compare
// ******************************************************************
{
    explicit less_from_compare( int (*cmp)( T const&, T const& ) )
    :   _cmp(cmp)
    {}
    
    bool operator()( T const& a, T const& b ) const { return (*_cmp)(a, b) < 0; }
    bool operator()( T const* a, T const* b ) const { return (*_cmp)(*a, *b) < 0; }

    friend bool operator==(
        less_from_compare a,
        less_from_compare b )
    {
        return a._cmp == b._cmp;
    }

    friend bool operator!=(
        less_from_compare a,
        less_from_compare b )
    {
        return !(a == b);
    }

    void swap( less_from_compare& b )
    {
        using ali::swap;
        swap(_cmp, b._cmp);
    }

    friend void swap( less_from_compare& a, less_from_compare& b )
    {
        a.swap(b);
    }
    
    int (*_cmp)( T const&, T const& );
};

// ******************************************************************
template <typename T>
struct less_from_compare_ptr
// ******************************************************************
{
    explicit less_from_compare_ptr( int (*cmp)( T const*, T const* ) )
    :   _cmp(cmp)
    {}
    
    bool operator()( T const& a, T const& b ) const { return (*_cmp)(&a, &b) < 0; }
    bool operator()( T const* a, T const* b ) const { return (*_cmp)(a, b) < 0; }

    friend bool operator==(
        less_from_compare_ptr a,
        less_from_compare_ptr b )
    {
        return a._cmp == b._cmp;
    }

    friend bool operator!=(
        less_from_compare_ptr a,
        less_from_compare_ptr b )
    {
        return !(a == b);
    }

    void swap( less_from_compare_ptr& b )
    {
        using ali::swap;
        swap(_cmp, b._cmp);
    }

    friend void swap( less_from_compare_ptr& a, less_from_compare_ptr& b )
    {
        a.swap(b);
    }

    int (*_cmp)( T const*, T const* );
};
/*
// ******************************************************************
template <typename T>
struct equal_to
// ******************************************************************
{
    bool operator()( T const& a, T const& b ) const { return a == b; }
    bool operator()( T const* a, T const* b ) const { return *a == *b; }
};
*/

// ******************************************************************
template <typename RefT>
class ref_holder
// ******************************************************************
{
public:
    ref_holder( RefT ref ) : _ref(ref) {}
    operator RefT( void ) const { return _ref; }

private:
    RefT _ref;
};


// ******************************************************************
template <typename T>
ref_holder<T&> by_ref( T& t )
// ******************************************************************
{
    return ref_holder<T&>(t);
}


// ******************************************************************
template <typename T>
ref_holder<T const&> by_const_ref( T const& t )
// ******************************************************************
{
    return ref_holder<T const&>(t);
}


// ******************************************************************
template <typename T1, typename T2>
struct pair
// ******************************************************************
{
    typedef T1              first_type;
    typedef T2              second_type;

    pair( void )
    :   first(),
        second()
    {}

    pair( T1 const& val1, T2 const& val2 )
    :   first(val1),
        second(val2)
    {}

#ifdef  ALI_FEATURE_RVALUE_REF

    pair( pair const& ) = default;
    pair( pair&& ) = default;

    pair( T1 const& val1, T2&& val2 )
    :   first(val1),
        second(ali::move(val2))
    {}

    pair( T1&& val1, T2 const& val2 )
    :   first(ali::move(val1)),
        second(val2)
    {}

    pair( T1&& val1, T2&& val2 )
    :   first(ali::move(val1)),
        second(ali::move(val2))
    {}

    pair& operator=( pair const& ) = default;
    pair& operator=( pair&& ) = default;

#endif  //  ALI_FEATURE_RVALUE_REF

    
    template <typename U1, typename U2>
    pair( pair<U1, U2> const& b )
    :   first(b.first),
        second(b.second)
    {}

    void swap( pair& b )
    {
        using ali::swap;
        swap(first, b.first);
        swap(second, b.second);
    }

    friend void swap( pair& a, pair& b )
    {
        a.swap(b);
    }

    T1 first;
    T2 second;
};

// ******************************************************************
template <typename T1, typename T2>
inline bool operator==( pair<T1, T2> const& a,
                        pair<T1, T2> const& b )
// ******************************************************************
{
    return a.first == b.first && a.second == b.second;
}

// ******************************************************************
template <typename T1, typename T2>
inline bool operator!=( pair<T1, T2> const& a,
                        pair<T1, T2> const& b )
// ******************************************************************
{
    return !(a == b);
}


// ******************************************************************
template <typename T1, typename T2>
inline pair<T1, T2> make_pair( T1 const& a, T2 const& b )
// ******************************************************************
{
    return pair<T1, T2>(a, b);
}

namespace detail
{

// ******************************************************************
template <typename T1, typename T2>
struct tier
// ******************************************************************
{
    tier( T1& a, T2& b ) : first(a), second(b) {}

    tier<T1, T2>& operator=( pair<T1, T2> const& p )
    {
        first = p.first;
        second = p.second;
        return *this;
    }

    T1& first;
    T2& second;

private:  // Methods
    tier<T1, T2> operator=( tier<T1, T2> const& );
};

}

// ******************************************************************
template <typename T1, typename T2>
inline detail::tier<T1, T2> tie( T1& a, T2& b )
// ******************************************************************
{
    return detail::tier<T1, T2>(a, b);
}


// ******************************************************************
template<typename II1, typename II2>
inline bool equal( II1 it1, II1 end1, II2 it2 )
// ******************************************************************
{
    for ( ; it1 != end1; ++it1, ++it2 )
        if ( !(*it1 == *it2) )
            return false;
    return true;
}

// ******************************************************************
inline void* advance_n_bytes( void* ptr, int n )
// ******************************************************************
{
    return reinterpret_cast<char*>(ptr) + n;
}

// ******************************************************************
template <typename intT>
class flags_type
// ******************************************************************
{
public:
    flags_type( intT flags = 0 )
    :   _flags(flags)
    {}
    
    flags_type& set( intT flag, bool val = true )
    {
        val ? _flags |= flag : _flags &= ~flag;
        return *this;
    }
    
    flags_type& set( intT flags, intT mask  )
    {
        _flags &= ~mask;
        _flags |= flags;
        return *this;
    }
    
    bool test( intT flag ) const
    {
        return (_flags & flag) == flag;
    }
    
    intT get( void ) const
    {
        return _flags;
    }
    
private:
    intT    _flags;
};

}   //  namespace ali
