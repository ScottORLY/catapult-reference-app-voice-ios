#pragma once
#include "ali_array_utils_forward.h"
#include "ali_array_forward.h"
#include "ali_array_utils_platform.h"
#include "ali_ctype.h"
#include "ali_debug.h"
#include "ali_deprecated.h"
#include "ali_endianness.h"
#include "ali_functor_types_common.h"
#include "ali_initializer_list.h"
#include "ali_integer.h"
#include "ali_location.h"
#include "ali_meta.h"
#include "ali_nullptr.h"
#include "ali_utility.h"
#include "ali_wchar.h"

namespace ali
{

// ******************************************************************
struct is_internal_element_result { int index_or_negative; };
// ******************************************************************

// ******************************************************************
struct is_internal_pointer_result { int distance_or_negative; };
// ******************************************************************

namespace hidden
{

// ******************************************************************
template <typename T, typename U>
void copy( T* dst, U const* src, int n )
// ******************************************************************
//  Copies arrays of different types.
//  Copies elements from [src; src+n) to [dst; dst+n).
//  Assumption: dst and src don't overlap.
// ******************************************************************
{
    while ( n != 0 )
        --n, dst[n] = src[n];
}

// ******************************************************************
template <typename T>
inline void copy( T* dst, T const* src, int n,
    meta::define_bool_result<true> /*is_pod*/ )
// ******************************************************************
//  Copies arrays of bitwise copyable elements.
// ******************************************************************
{
    if ( n != 0 && dst != src )
        platform::memmove(dst, src, n * sizeof(T));
}

// ******************************************************************
template <typename T>
void copy( T* dst, T const* src, int n,
    meta::define_bool_result<false> /*is_pod*/ )
// ******************************************************************
//  Copies arrays of NOT bitwise copyable elements.
// ******************************************************************
{
    if ( dst < src )
    {
        for ( int i = 0; i != n; ++i )
            dst[i] = src[i];
    }
    else if ( dst != src )
    {
        hidden::copy<T, T>(dst, src, n);
/*
        while ( n != 0 )
            --n, dst[n] = src[n];
*/
    }
}

// ******************************************************************
template <typename T>
inline void copy( T* dst, T const* src, int n )
// ******************************************************************
//  Copies arrays of the same types.
//  Copies elements from [src; src+n) to [dst; dst+n).
//  The arrays may overlap.
//  Uses memmove if possible.
// ******************************************************************
{
    hidden::copy(dst, src, n, typename meta::is_pod<T>());
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T, typename U>
void move( T* dst, U* src, int n )
// ******************************************************************
//  Moves elements of arrays of different types.
//  Moves elements from [src; src+n) to [dst; dst+n).
//  Assumption: dst and src don't overlap.
// ******************************************************************
{
    while ( n != 0 )
        --n, dst[n] = ali::move(src[n]);
}

// ******************************************************************
template <typename T>
inline void move( T* dst, T* src, int n,
    meta::define_bool_result<true> /*is_pod*/ )
// ******************************************************************
//  Moves elements of arrays of bitwise copyable elements.
// ******************************************************************
{
    if ( n != 0 && dst != src )
        platform::memmove(dst, src, n * sizeof(T));
}

// ******************************************************************
template <typename T>
void move( T* dst, T* src, int n,
    meta::define_bool_result<false> /*is_pod*/ )
// ******************************************************************
//  Moves elements of arrays of NOT bitwise copyable elements.
// ******************************************************************
{
    if ( dst < src )
    {
        for ( int i = 0; i != n; ++i )
            dst[i] = ali::move(src[i]);
    }
    else if ( dst != src )
    {
        hidden::move<T, T>(dst, src, n);
/*
        while ( n != 0 )
            --n, dst[n] = ali::move(src[n]);
*/
    }
}

// ******************************************************************
template <typename T>
inline void move( T* dst, T* src, int n )
// ******************************************************************
//  Moves elements of arrays of the same types.
//  Moves elements from [src; src+n) to [dst; dst+n).
//  The arrays may overlap.
//  Uses memmove if possible.
// ******************************************************************
{
    hidden::move(dst, src, n, typename meta::is_pod<T>());
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T, typename U>
void move_if_noexcept( T* dst, U* src, int n )
// ******************************************************************
//  Moves elements of arrays of different types.
//  Moves elements from [src; src+n) to [dst; dst+n).
//  Assumption: dst and src don't overlap.
// ******************************************************************
{
    while ( n != 0 )
        --n, dst[n] = ali::move_if_noexcept(src[n]);
}

// ******************************************************************
template <typename T>
inline void move_if_noexcept( T* dst, T* src, int n,
    meta::define_bool_result<true> /*is_pod*/ )
// ******************************************************************
//  Moves elements of arrays of bitwise copyable elements.
// ******************************************************************
{
    if ( n != 0 && dst != src )
        platform::memmove(dst, src, n * sizeof(T));
}

// ******************************************************************
template <typename T>
void move_if_noexcept( T* dst, T* src, int n,
    meta::define_bool_result<false> /*is_pod*/ )
// ******************************************************************
//  Moves elements of arrays of NOT bitwise copyable elements.
// ******************************************************************
{
    if ( dst < src )
    {
        for ( int i = 0; i != n; ++i )
            dst[i] = ali::move_if_noexcept(src[i]);
    }
    else if ( dst != src )
    {
        hidden::move_if_noexcept<T, T>(dst, src, n);
/*
        while ( n != 0 )
            --n, dst[n] = ali::move(src[n]);
*/
    }
}

// ******************************************************************
template <typename T>
inline void move_if_noexcept( T* dst, T* src, int n )
// ******************************************************************
//  Moves elements of arrays of the same types.
//  Moves elements from [src; src+n) to [dst; dst+n).
//  The arrays may overlap.
//  Uses memmove if possible.
// ******************************************************************
{
    hidden::move_if_noexcept(
        dst, src, n, typename meta::is_pod<T>());
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
struct read_only : meta::select<
    meta::is_fundamental<T>::result,
    T const, T const&> {};
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
void fill( T* arr, typename read_only<T>::type value, int n,
    meta::define_bool_result<false> /*is_default_all_zeros*/ )
// ******************************************************************
//  Fills array with a given value.
// ******************************************************************
{
    if ( n != 0 )
    {
        int count = 1;
        
        hidden::copy(arr, &value, count);
        
        while ( count != n )
        {
            int const copy_count
                = ali::mini(n - count, count);
            
            hidden::copy(arr + count, arr, copy_count);
            
            count += copy_count;
        }
    }
}

// ******************************************************************
template <typename T>
inline void fill( T* arr, typename read_only<T>::type value, int n,
    meta::define_bool_result<true> /*is_default_all_zeros*/ )
// ******************************************************************
//  Fills array of a "default all zeros" type
//  with a given value using memset if possible.
// ******************************************************************
{
    if ( value != T() )
        fill(arr, value, n, meta::define_bool_result<false>());
    else if ( n != 0 )
        platform::memset(arr, 0, n * sizeof(T));
}

// ******************************************************************
template <typename T>
inline void fill( T* arr, typename read_only<T>::type value, int n )
// ******************************************************************
//  Fills array with a given value using memset if possible.
// ******************************************************************
{
    fill(arr, value, n, typename meta::is_default_all_zeros<T>());
}

// ******************************************************************
inline void fill( char* arr, read_only<char>::type value, int n )
// ******************************************************************
//  Fills array of chars with a given value using memset.
// ******************************************************************
{
    if ( n != 0 )
        platform::memset(arr, value, n);
}

// ******************************************************************
inline void fill(
    signed char* arr, read_only<signed char>::type value, int n )
// ******************************************************************
//  Fills array of signed chars with a given value using memset.
// ******************************************************************
{
    if ( n != 0 )
        platform::memset(arr, value, n);
}

// ******************************************************************
inline void fill(
    unsigned char* arr, read_only<unsigned char>::type value, int n )
// ******************************************************************
//  Fills array of unsigned chars with a given value using memset.
// ******************************************************************
{
    if ( n != 0 )
        platform::memset(arr, value, n);
}

// ******************************************************************
inline void fill(
    ali::wchar* arr, read_only<ali::wchar>::type value, int n )
// ******************************************************************
//  Fills array of wchars with a given value using memset.
// ******************************************************************
{
    if ( n != 0 )
        platform::wmemset(arr, value, n);
}

}   //  namespace hidden

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class array_const_ref_common
// ******************************************************************
{
public:     //  Typedefs
    typedef typename hidden::read_only<T>::type read_only_T;

public:
    array_const_ref_common( void )
    :   _begin(nullptr),
        _size(0)
    {}

    array_const_ref_common( T const* begin, int size )
    :   _begin(begin),
        _size(size)
    {
        ali_assert(0 <= this->_size);
        ali_assert(this->_size == 0 || this->_begin != nullptr);
    }

    bool is_empty( void ) const
    {
        return this->_size == 0;
    }

    int size( void ) const
    {
        return this->_size;
    }

    T const& front( void ) const
        //  pre:    !is_empty()
    {
        ali_assert(!this->is_empty());
        
        return this->_begin[0];
    }

    T const& back( void ) const
        //  pre:    !is_empty()
    {
        ali_assert(!this->is_empty());
        
        return this->_begin[this->_size - 1];
    }

    T const& back( int i ) const
        //  pre:    0 <= i && i < size()
    {
        ali_assert(0 <= i);
        ali_assert(i < this->_size);
        
        return this->_begin[this->_size - 1 - i];
    }

    T const& at( int i ) const
    {
        ali_assert(0 <= i);
        ali_assert(i < this->_size);

        return this->_begin[i];
    }

    T const& operator[]( int i ) const
    {
        return this->at(i);
    }

    T const* data( void ) const
    {
        return this->_begin;
    }

    //  reference

    array_const_ref<T> ref( int pos, int n ) const;
        //  pre:    0 <= pos && pos <= size()
        //      &&  0 <= n && n <= size() - pos
    array_const_ref<T> ref( void ) const;
    array_const_ref<T> ref_left( int pos ) const;
        //  pre:    0 <= pos && pos <= size()
    array_const_ref<T> ref_right( int pos ) const;
        //  pre:    0 <= pos && pos <= size()
    array_const_ref<T> ref_front( int n ) const;
        //  pre:    0 <= n && n <= size()
    array_const_ref<T> ref_back( int n ) const;
        //  pre:    0 <= n && n <= size()
    array_const_ref<T> ref_not_front( int n ) const;
        //  pre:    0 <= n && n <= size()
    array_const_ref<T> ref_not_back( int n ) const;
        //  pre:    0 <= n && n <= size()

    //  pointer

    array_const_ptr<T> pointer( int pos, int n ) const;
        //  pre:    0 <= pos && pos <= size()
        //      &&  0 <= n && n <= size() - pos
    array_const_ptr<T> pointer( void ) const;
    array_const_ptr<T> pointer_left( int pos ) const;
        //  pre:    0 <= pos && pos <= size()
    array_const_ptr<T> pointer_right( int pos ) const;
        //  pre:    0 <= pos && pos <= size()
    array_const_ptr<T> pointer_front( int n ) const;
        //  pre:    0 <= n && n <= size()
    array_const_ptr<T> pointer_back( int n ) const;
        //  pre:    0 <= n && n <= size()
    array_const_ptr<T> pointer_not_front( int n ) const;
        //  pre:    0 <= n && n <= size()
    array_const_ptr<T> pointer_not_back( int n ) const;
        //  pre:    0 <= n && n <= size()

    //  for_each

    template <typename body>
    array_const_ref<T> for_each( body b ) const;

    //  is_equal_to

    template <typename U, typename transform>
    bool is_equal_to( array_const_ref<U> b, transform t ) const
    {
        if ( this->_size != b._size )
            return false;
        else for ( int i = this->_size; i != 0; )
            if ( --i, t(this->at(i)) != t(b.at(i)) )
                return false;
        return true;
    }

    template <typename transform>
    bool is_equal_to( array_const_ref<T> b, transform t ) const
    {
        return this->is_equal_to<T, transform>(b, t);
    }

    template <typename U>
    bool is_equal_to( array_const_ref<U> b, functor_types::identity ) const
    {
        return this->_is_equal_to(b, meta::define_bool_result<
            //  Use memcmp if:
                    meta::is_fundamental<T>::result
                &&  meta::is_same_type_remove_cv<T, U>::result>());
    }

    bool is_equal_to( array_const_ref<T> b, functor_types::identity ) const
    {
        return this->is_equal_to<T>(b, ali::identity);
    }

    template <typename U>
    bool is_equal_to( array_const_ref<U> b ) const
    {
        return this->is_equal_to(b, ali::identity);
    }

    bool is_equal_to( array_const_ref<T> b ) const
    {
        return this->is_equal_to<T>(b);
    }

#if 0
    friend bool operator==(
        array_const_ref<T> a,
        array_const_ref<T> b )
    {
        return a.is_equal_to(b);
    }

    friend bool operator!=(
        array_const_ref<T> a,
        array_const_ref<T> b )
    {
        return !(a == b);
    }
#endif  //  0

    //  compare

    template <typename U, typename transform>
    int compare( array_const_ref<U> b, transform t ) const
    {
        int result = 0;

        int const n = ali::mini(this->_size, b._size);

        using ali::compare;

        for ( int i = 0; result == 0 && i != n; ++i )
            result = compare(t(this->at(i)), t(b.at(i)));

        if ( result == 0 )
            result = compare(this->_size, b._size);
        
        return result;
    }

    template <typename transform>
    int compare( array_const_ref<T> b, transform t ) const
    {
        return this->compare<T, transform>(b, t);
    }
/*
    template <typename U>
    int compare( array_const_ref<U> b, functor_types::identity ) const
    {
        return this->_compare(b, meta::define_bool_result<
            //  Use memcmp if:
                    meta::is_fundamental<T>::result
                &&  meta::is_same_type_remove_cv<T, U>::result>());
    }

    int compare( array_const_ref<T> b, functor_types::identity ) const
    {
        return this->compare<T>(b, ali::identity);
    }
*/
    template <typename U>
    int compare( array_const_ref<U> b ) const
    {
        return this->compare(b, ali::identity);
    }

    int compare( array_const_ref<T> b ) const
    {
        return this->compare<T>(b);
    }

#if 0
    friend int compare(
        array_const_ref<T> a,
        array_const_ref<T> b )
    {
        return a.compare(b);
    }

    friend bool operator<(
        array_const_ref<T> a,
        array_const_ref<T> b )
    {
        return compare(a, b) < 0;
    }

    friend bool operator<=(
        array_const_ref<T> a,
        array_const_ref<T> b )
    {
        return compare(a, b) <= 0;
    }

    friend bool operator>(
        array_const_ref<T> a,
        array_const_ref<T> b )
    {
        return compare(a, b) > 0;
    }

    friend bool operator>=(
        array_const_ref<T> a,
        array_const_ref<T> b )
    {
        return compare(a, b) >= 0;
    }
#endif  //  0

    //  overlaps_with

    // ******************************************************************
    bool overlaps_with( array_const_ref<T> b ) const
    // ******************************************************************
    {
        return this->_begin + this->_size > b._begin
            && b._begin + b._size > this->_begin;
    }

    //  begins_with
    
    template <typename transform>
    bool begins_with( read_only_T b, transform t ) const
    {
        return !this->is_empty() && t(this->front()) == t(b);
    }

    bool begins_with( read_only_T b ) const
    {
        return this->begins_with(b, ali::identity);
    }

    template <typename U, typename transform>
    bool begins_with( U const& b, transform t ) const
    {
        return !this->is_empty() && t(this->front()) == t(b);
    }

    template <typename U, typename transform>
    bool begins_with_n( array_const_ref<U> b, transform t ) const;
        //  Matches elements of b against b.size() front elements.

    template <typename transform>
    bool begins_with_n( array_const_ref<T> b, transform t ) const
    {
        return this->begins_with_n<T, transform>(b, t);
    }

    template <typename U>
    bool begins_with( U const& b ) const
    {
        return this->begins_with(b, ali::identity);
    }

    template <typename U>
    bool begins_with_n( array_const_ref<U> b ) const
        //  Matches elements of b against b.size() front elements.
    {
        return this->begins_with_n(b, ali::identity);
    }

    bool begins_with_n( array_const_ref<T> b ) const
    {
        return this->begins_with_n<T>(b);
    }

    //  ends_with

    template <typename transform>
    bool ends_with( read_only_T b, transform t ) const
    {
        return !this->is_empty() && t(this->back()) == t(b);
    }

    bool ends_with( read_only_T b ) const
    {
        return this->ends_with(b, ali::identity);
    }

    template <typename U, typename transform>
    bool ends_with( U const& b, transform t ) const
    {
        return !this->is_empty() && t(this->back()) == t(b);
    }

    template <typename U, typename transform>
    bool ends_with_n( array_const_ref<U> b, transform t ) const;
        //  Matches elements of b against b.size() back elements.

    template <typename transform>
    bool ends_with_n( array_const_ref<T> b, transform t ) const
    {
        return this->ends_with_n<T, transform>(b, t);
    }

    template <typename U>
    bool ends_with( U const& b ) const
    {
        return this->ends_with(b, ali::identity);
    }

    template <typename U>
    bool ends_with_n( array_const_ref<U> b ) const
        //  Matches elements of b against b.size() back elements.
    {
        return this->ends_with_n(b, ali::identity);
    }

    bool ends_with_n( array_const_ref<T> b ) const
    {
        return this->ends_with_n<T>(b);
    }

    //  index_of_first_if

    template <typename predicate>
    int index_of_first_if( predicate p ) const
        //  post:   result == size()    //  not found
        //      ||  (   p(at(result))
        //          &&  for all i in set {0 ... result - 1} &
        //                  !p(at(i)) )
    {
        for ( int i = 0; i != this->_size; ++i )
            if ( p(this->at(i)) )
                return i;
    
        return this->_size;
    }

    //  index_of_first_if_not

    template <typename predicate>
    int index_of_first_if_not( predicate p ) const
        //  post:   result == size()    //  not found
        //      ||  (   !p(at(result))
        //          &&  for all i in set {0 ... result - 1} &
        //                  p(at(i)) )
    {
        for ( int i = 0; i != this->_size; ++i )
            if ( !p(this->at(i)) )
                return i;
    
        return this->_size;
    }

    //  index_of_first

    template <typename transform>
    int index_of_first( read_only_T b, transform t ) const
        //  post:   result == size()    //  not found
        //      ||  (   t(at(result)) == t(b)
        //          &&  for all i in set {0 ... result - 1} &
        //                  !(t(at(i)) == t(b)) )
    {
        for ( int i = 0; i != this->_size; ++i )
            if ( t(this->at(i)) == t(b) )
                return i;
    
        return this->_size;
    }

    int index_of_first( read_only_T b ) const
        //  post:   result == size()    //  not found
        //      ||  (   at(result) == b
        //          &&  for all i in set {0 ... result - 1} &
        //                  !(at(i) == b) )
    {
        return this->index_of_first(b, ali::identity);
    }

    template <typename U, typename transform>
    int index_of_first( U const& b, transform t ) const
        //  post:   result == size()    //  not found
        //      ||  (   t(at(result)) == t(b)
        //          &&  for all i in set {0 ... result - 1} &
        //                  !(t(at(i)) == t(b)) )
    {
        for ( int i = 0; i != this->_size; ++i )
            if ( t(this->at(i)) == t(b) )
                return i;
    
        return this->_size;
    }

    template <typename U, typename transform>
    int index_of_first_n( array_const_ref<U> b, transform t ) const
        //  post:   result == size()    //  not found
        //      ||  (   ref(result, b.size()).is_equal_to(b, t)
        //          &&  for all i in set {0 ... result - 1} &
        //                  !ref(i, b.size()).is_equal_to(b, t) )
    {
        if ( b.is_empty() )
            return 0;

        if ( this->_size < b._size )
            return this->_size;
    
        int const i_max = this->_size - b._size;

        int i = 0;

        for ( int j = b._size; j != 0; )
            if ( --j, t(this->at(i + j)) != t(b.at(j)) )
            {
                if ( i == i_max )
                    return this->_size;

                ++i;

                j = b._size;
            }
    
        return i;
    }

    template <typename transform>
    int index_of_first_n( array_const_ref<T> b, transform t ) const
    {
        return this->index_of_first_n<T, transform>(b, t);
    }

    template <typename U>
    int index_of_first( U const& b ) const
        //  post:   result == size()    //  not found
        //      ||  (   at(result) == b
        //          &&  for all i in set {0 ... result - 1} &
        //                  !(at(i) == b) )
    {
        return this->index_of_first(b, ali::identity);
    }

    template <typename U>
    int index_of_first_n( array_const_ref<U> b ) const
        //  post:   result == size()    //  not found
        //      ||  (   ref(result, b.size()).is_equal_to(b)
        //          &&  for all i in set {0 ... result - 1} &
        //                  !ref(i, b.size()).is_equal_to(b) )
    {
        return this->index_of_first_n(b, ali::identity);
    }

    int index_of_first_n( array_const_ref<T> b ) const
    {
        return this->index_of_first_n<T>(b);
    }

    //  index_of_first_of

    template <typename U, typename transform>
    int index_of_first_of( array_const_ref<U> set, transform t ) const
        //  post:   result == size()    //  not found
        //      ||  (   set.contains(at(result), t)
        //          &&  for all i in set {0 ... result - 1} &
        //                  !set.contains(at(i), t) )
    {
        if ( !set.is_empty() )
            for ( int i = 0; i != this->_size; ++i )
                for ( int j = set._size; j != 0; )
                    if ( --j, t(this->at(i)) == t(set.at(j)) )
                        return i;
        
        return this->_size;
    }

    template <typename transform>
    int index_of_first_of( array_const_ref<T> set, transform t ) const
    {
        return this->index_of_first_of<T, transform>(set, t);
    }

    template <typename U>
    int index_of_first_of( array_const_ref<U> set ) const
        //  post:   result == size()    //  not found
        //      ||  (   set.contains(at(result))
        //          &&  for all i in set {0 ... result - 1} &
        //                  !set.contains(at(i)) )
    {
        return this->index_of_first_of(set, ali::identity);
    }    

    int index_of_first_of( array_const_ref<T> set ) const
    {
        return this->index_of_first_of<T>(set);
    }

    //  prioritized_index_of_first_of
    //
    //  Treats *this and set as two ordered lists of options with the most
    //  desirable option first. Searches for a common option such that the
    //  SUM of the option indexes in the two lists IS MINIMUM.
    //  In case there are more such options the option that appears first
    //  in *this list is selected.

    template <typename U, typename transform>
    int prioritized_index_of_first_of( array_const_ref<U> set, transform t ) const
    {
        int const index_of_our_preferred_option
            = this->index_of_first_of(set, t);
        
        if ( index_of_our_preferred_option == this->_size )
            return this->_size;
                //  No common option.

        int const index_of_their_preferred_option
            = set.index_of_first_of(this->ref(), t);

        int const index_of_our_preferred_option_in_their_list
            = set.index_of_first(this->at(index_of_our_preferred_option), t);

        int const index_of_their_preferred_option_in_our_list
            = this->index_of_first(set.at(index_of_their_preferred_option), t);

        int const penalty_of_our_option
            = index_of_our_preferred_option
                + index_of_our_preferred_option_in_their_list;

        int const penalty_of_their_option
            = index_of_their_preferred_option
                + index_of_their_preferred_option_in_our_list;

        return  penalty_of_their_option < penalty_of_our_option
            ?   index_of_their_preferred_option_in_our_list
                    //  Prefer their option.
            :   index_of_our_preferred_option;
                    //  Prefer our option.
    }

    template <typename transform>
    int prioritized_index_of_first_of( array_const_ref<T> set, transform t ) const
    {
        return this->prioritized_index_of_first_of<T, transform>(set, t);
    }

    template <typename U>
    int prioritized_index_of_first_of( array_const_ref<U> set ) const
    {
        return this->prioritized_index_of_first_of(set, ali::identity);
    }    

    int prioritized_index_of_first_of( array_const_ref<T> set ) const
    {
        return this->prioritized_index_of_first_of<T>(set);
    }

    //  index_of_first_not_of

    template <typename U, typename transform>
    int index_of_first_not_of( array_const_ref<U> set, transform t ) const
        //  post:   result == size()    //  not found
        //      ||  (   !set.contains(at(result), t)
        //          &&  for all i in set {0 ... result - 1} &
        //                  set.contains(at(i), t) )
    {
        //if ( set.is_empty() )
        //    return 0;
                //  Any element is not a member of the empty set.
                //  (The later code handles this case correctly.)
    
        if ( this->is_empty() )
            return 0;

        int const i_max = this->_size - 1;

        int i = 0;

        for ( int j = set._size; j != 0; )
            if ( --j, t(this->at(i)) == t(set.at(j)) )
            {
                if ( i == i_max )
                    return this->_size;

                ++i;

                j = set._size;
            }
    
        return i;
    }

    template <typename transform>
    int index_of_first_not_of( array_const_ref<T> set, transform t ) const
    {
        return this->index_of_first_not_of<T, transform>(set, t);
    }

    template <typename U>
    int index_of_first_not_of( array_const_ref<U> set ) const
        //  post:   result == size()    //  not found
        //      ||  (   !set.contains(at(result))
        //          &&  for all i in set {0 ... result - 1} &
        //                  set.contains(at(i)) )
    {
        return this->index_of_first_not_of(set, ali::identity);
    }

    int index_of_first_not_of( array_const_ref<T> set ) const
    {
        return this->index_of_first_not_of<T>(set);
    }

    //  index_of_last_if

    template <typename predicate>
    int index_of_last_if( predicate p ) const
        //  post:   result == size()    //  not found
        //      ||  (   p(at(result))
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  !p(at(i)) )
    {
        for ( int i = this->_size; i != 0; )
            if ( p(this->at(--i)) )
                return i;
    
        return this->_size;
    }

    //  index_of_last_if_not

    template <typename predicate>
    int index_of_last_if_not( predicate p ) const
        //  post:   result == size()    //  not found
        //      ||  (   !p(at(result))
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  p(at(i)) )
    {
        for ( int i = this->_size; i != 0; )
            if ( !p(this->at(--i)) )
                return i;
    
        return this->_size;
    }

    //  index_of_last

    template <typename transform>
    int index_of_last( read_only_T b, transform t ) const
        //  post:   result == size()    //  not found
        //      ||  (   t(at(result)) == t(b)
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  !(t(at(i)) == t(b)) )
    {
        for ( int i = this->_size; i != 0; )
            if ( --i, t(this->at(i)) == t(b) )
                return i;
    
        return this->_size;
    }

    int index_of_last( read_only_T b ) const
        //  post:   result == size()    //  not found
        //      ||  (   at(result) == b
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  !(at(i) == b) )
    {
        return this->index_of_last(b, ali::identity);
    }

    template <typename U, typename transform>
    int index_of_last( U const& b, transform t ) const
        //  post:   result == size()    //  not found
        //      ||  (   t(at(result)) == t(b)
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  !(t(at(i)) == t(b)) )
    {
        for ( int i = this->_size; i != 0; )
            if ( --i, t(this->at(i)) == t(b) )
                return i;
    
        return this->_size;
    }

    template <typename U, typename transform>
    int index_of_last_n( array_const_ref<U> b, transform t ) const
        //  post:   result == size()    //  not found
        //      ||  (   ref(result, b.size()).is_equal_to(b, t)
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  !ref(i, b.size()).is_equal_to(b, t) )
    {
        if ( b.is_empty() )
            return ali::maxi(1, this->_size) - 1;
            
        if ( this->_size < b._size )
            return this->_size;
    
        int i = this->_size - b._size;

        for ( int j = b._size; j != 0; )
            if ( --j, t(this->at(i + j)) != t(b.at(j)) )
            {
                if ( i == 0 )
                    return this->_size;

                --i;

                j = b._size;
            }
    
        return i;
    }

    template <typename transform>
    int index_of_last_n( array_const_ref<T> b, transform t ) const
    {
        return this->index_of_last_n<T, transform>(b, t);
    }

    template <typename U>
    int index_of_last( U const& b ) const
        //  post:   result == size()    //  not found
        //      ||  (   at(result) == b
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  !(at(i) == b) )
    {
        return this->index_of_last(b, ali::identity);
    }

    template <typename U>
    int index_of_last_n( array_const_ref<U> b ) const
        //  post:   result == size()    //  not found
        //      ||  (   ref(result, b.size()).is_equal_to(b)
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  !ref(i, b.size()).is_equal_to(b) )
    {
        return this->index_of_last_n(b, ali::identity);
    }

    int index_of_last_n( array_const_ref<T> b ) const
    {
        return this->index_of_last_n<T>(b);
    }

    //  index_of_last_of

    template <typename U, typename transform>
    int index_of_last_of( array_const_ref<U> set, transform t ) const
        //  post:   result == size()    //  not found
        //      ||  (   set.contains(at(result), t)
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  !set.contains(at(i), t) )
    {
        if ( !set.is_empty() )
            for ( int i = this->_size; i != 0; )
            {
                --i;
                for ( int j = set._size; j != 0; )
                    if ( --j, t(this->at(i)) == t(set.at(j)) )
                        return i;
            }

        return this->_size;
    }

    template <typename transform>
    int index_of_last_of( array_const_ref<T> set, transform t ) const
    {
        return this->index_of_last_of<T, transform>(set, t);
    }

    template <typename U>
    int index_of_last_of( array_const_ref<U> set ) const
        //  post:   result == size()    //  not found
        //      ||  (   set.contains(at(result))
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  !set.contains(at(i)) )
    {
        return this->index_of_last_of(set, ali::identity);
    }

    int index_of_last_of( array_const_ref<T> set ) const
    {
        return this->index_of_last_of<T>(set);
    }

    //  index_of_last_not_of

    template <typename U, typename transform>
    int index_of_last_not_of( array_const_ref<U> set, transform t ) const
        //  post:   result == size()    //  not found
        //      ||  (   !set.contains(at(result), t)
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  set.contains(at(i), t) )
    {
        //if ( set.is_empty() )
        //    return this->_size - 1;
                //  Any element is not a member of the empty set.
                //  (The later code handles this case correctly.)
    
        if ( this->is_empty() )
            return this->_size;

        int i = this->_size - 1;

        for ( int j = set._size; j != 0; )
            if ( --j, t(this->at(i)) == t(set.at(j)) )
            {
                if ( i == 0 )
                    return this->_size;

                --i;

                j = set._size;
            }
    
        return i;
    }

    template <typename transform>
    int index_of_last_not_of( array_const_ref<T> set, transform t ) const
    {
        return this->index_of_last_not_of<T, transform>(set, t);
    }

    template <typename U>
    int index_of_last_not_of( array_const_ref<U> set ) const
        //  post:   result == size()    //  not found
        //      ||  (   !set.contains(at(result))
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  set.contains(at(i)) )
    {
        return this->index_of_last_not_of(set, ali::identity);
    }

    int index_of_last_not_of( array_const_ref<T> set ) const
    {
        return this->index_of_last_not_of<T>(set);
    }

    //  index_of_min

    template <typename comparator>
    int index_of_min( comparator compare ) const
    {
        int result = this->_size;

        if ( result != 0 )
            for ( int i = --result; i != 0; )
                if ( compare(this->at(--i), this->at(result)) <= 0 )
                    result = i;

        return result;
    }

    int index_of_min( void ) const
    {
        return this->index_of_min(
            [] ( read_only_T a, read_only_T b )
            {
                using ali::compare;

                return compare(a, b);
            });
    }

    //  index_of_max

    template <typename comparator>
    int index_of_max( comparator compare ) const
    {
        int result = this->_size;

        if ( result != 0 )
            for ( int i = --result; i != 0; )
                if ( compare(this->at(--i), this->at(result)) > 0 )
                    result = i;

        return result;
    }

    int index_of_max( void ) const
    {
        return this->index_of_max(
            [] ( read_only_T a, read_only_T b )
            {
                using ali::compare;

                return compare(a, b);
            });
    }

    //  index_of_min_max

    template <typename comparator>
    pair<int, int> index_of_min_max( comparator compare ) const
        //  Note: This is more efficient than calling
        //  index_of_min and index_of_max separately.
    {
        using ali::swap;

        int idx = this->_size;

        if ( idx <= 1 )
            return make_pair(0, 0);

        int i_max = --idx;
        int i_min = --idx;

        if ( compare(this->at(i_min), this->at(i_max)) > 0 )
            swap(i_min, i_max);

        ali_assert(
                compare(this->at(i_min), this->at(i_max)) < 0
            ||  (   compare(this->at(i_min), this->at(i_max)) == 0
                &&  i_min < i_max));

        if ( idx % 2 == 1 )
        {
            --idx;

            if ( compare(this->at(idx), this->at(i_min)) <= 0 )
                i_min = idx;

            if ( compare(this->at(idx), this->at(i_max)) > 0 )
                i_max = idx;
        }

        while ( idx != 0 )
        {
            ali_assert(idx % 2 == 0);

            int potential_max = --idx;
            int potential_min = --idx;

            if ( compare(this->at(potential_min), this->at(potential_max)) > 0 )
                swap(potential_min, potential_max);

            if ( compare(this->at(potential_min), this->at(i_min)) <= 0 )
                i_min = potential_min;

            if ( compare(this->at(potential_max), this->at(i_max)) > 0 )
                i_max = potential_max;
        }

        return make_pair(i_min, i_max);
    }

    pair<int, int> index_of_min_max( void ) const
    {
        return this->index_of_min_max(
            [] ( read_only_T a, read_only_T b )
            {
                using ali::compare;

                return compare(a, b);
            });
    }

    //  all
    
    template <typename predicate>
    bool all( predicate p ) const
        //  Returns is_empty() || (p(at(0)) && ... && p(at(size() - 1)))
    {
        return this->index_of_last_if_not(p) == this->_size;
    }

    //  any

    template <typename predicate>
    bool any( predicate p ) const
        //  Returns !is_empty() && (p(at(0)) || ... || p(at(size() - 1)))
    {
        return this->index_of_last_if(p) != this->_size;
    }

    //  none

    template <typename predicate>
    bool none( predicate p ) const
        //  Returns is_empty() || (!p(at(0)) && ... && !p(at(size() - 1)))
    {
        return this->index_of_last_if(p) == this->_size;
    }

    //  contains

    template <typename transform>
    bool contains( read_only_T b, transform t ) const
        //  Returns index_of(b, t) != size()
    {
        return this->index_of_last(b, t) != this->_size;
    }

    bool contains( read_only_T b ) const
        //  Returns index_of(b) != size()
    {
        return this->contains(b, ali::identity);
    }

    template <typename U, typename transform>
    bool contains( U const& b, transform t ) const
        //  Returns index_of(b, t) != size()
    {
        return this->index_of_last(b, t) != this->_size;
    }

    template <typename U, typename transform>
    bool contains_n( array_const_ref<U> b, transform t ) const
        //  Returns index_of_n(b, t) != size()
    {
        return this->index_of_last_n(b, t) != this->_size;
    }

    template <typename transform>
    bool contains_n( array_const_ref<T> b, transform t ) const
    {
        return this->contains_n<T, transform>(b, t);
    }

    template <typename U>
    bool contains( U const& b ) const
        //  Returns index_of(b) != size()
    {
        return this->contains(b, ali::identity);
    }

    template <typename U>
    bool contains_n( array_const_ref<U> b ) const
        //  Returns index_of_n(b) != size()
    {
        return this->contains_n(b, ali::identity);
    }

    bool contains_n( array_const_ref<T> b ) const
    {
        return this->contains_n<T>(b);
    }

    //  contains_pair

    template <typename predicate>
    bool contains_pair( predicate p ) const
        //  Note: This method examines all the possible size * (size - 1) / 2 pairs.
    {
        for ( int i{this->_size}; i > 1; )
        {
            --i;
            ali_assert(i > 0);
            
            for ( int j{i}; j != 0; )
                if ( p(this->at(i), this->at(--j)) )
                    return true;
        }
        return false;
    }

    //  contains_duplicates

    bool contains_duplicates( void ) const
    {
        return contains_pair(
            [] ( T const& a, T const& b )
            { return a == b; });
    }

    //  trim_left_if

    template <typename predicate>
    array_const_ref<T> trim_left_if( predicate p ) const;
        //  post:   result.is_empty()
        //      ||  !predicate(result.front())

    //  trim_left

    template <typename U, typename transform>
    array_const_ref<T> trim_left( array_const_ref<U> set, transform t ) const;
        //  post:   result.is_empty()
        //      ||  !set.contains(result.front(), t)

    template <typename transform>
    array_const_ref<T> trim_left( array_const_ref<T> set, transform t ) const;

    template <typename U>
    array_const_ref<T> trim_left( array_const_ref<U> set ) const;
        //  post:   result.is_empty()
        //      ||  !set.contains(result.front())

    array_const_ref<T> trim_left( array_const_ref<T> set ) const;

    //  trim_right_if
    
    template <typename predicate>
    array_const_ref<T> trim_right_if( predicate p ) const;
        //  post:   result.is_empty()
        //      ||  !predicate(result.back())

    //  trim_right

    template <typename U, typename transform>
    array_const_ref<T> trim_right( array_const_ref<U> set, transform t ) const;
        //  post:   result.is_empty()
        //      ||  !set.contains(result.back(), t)

    template <typename transform>
    array_const_ref<T> trim_right( array_const_ref<T> set, transform t ) const;

    template <typename U>
    array_const_ref<T> trim_right( array_const_ref<U> set ) const;
        //  post:   result.is_empty()
        //      ||  !set.contains(result.back())

    array_const_ref<T> trim_right( array_const_ref<T> set ) const;

    //  trim_if

    template <typename predicate>
    array_const_ref<T> trim_if( predicate p ) const;
        //  post:   result.is_empty()
        //      ||  (   !p(result.front())
        //          &&  !p(result.back()))

    //  trim

    template <typename U, typename transform>
    array_const_ref<T> trim( array_const_ref<U> set, transform t ) const;
        //  post:   result.is_empty()
        //      ||  (   !set.contains(result.front(), t)
        //          &&  !set.contains(result.back(), t))

    template <typename transform>
    array_const_ref<T> trim( array_const_ref<T> set, transform t ) const;
    
    template <typename U>
    array_const_ref<T> trim( array_const_ref<U> set ) const;
        //  post:   result.is_empty()
        //      ||  (   !set.contains(result.front())
        //          &&  !set.contains(result.back()))

    array_const_ref<T> trim( array_const_ref<T> set ) const;

    //  count

    template <typename transform>
    int count( read_only_T b, transform t ) const
    {
        int n = 0;

        for ( int i = this->_size; i != 0; )
            if ( --i, t(this->at(i)) == t(b) )
                ++n;
    
        return n;
    }

    int count( read_only_T b ) const
    {
        return count(b, ali::identity);
    }

    template <typename U, typename transform>
    int count( U const& b, transform t ) const
        //  Returns the number of elements that compare equal to b (after transform t).
    {
        int n = 0;

        for ( int i = this->_size; i != 0; )
            if ( --i, t(this->at(i)) == t(b) )
                ++n;
    
        return n;
    }

    template <typename U, typename transform>
    int count_n( array_const_ref<U> b, transform t ) const;
        //  Returns the number of non-overlapping occurrences of b inside this array.

    template <typename transform>
    int count_n( array_const_ref<T> b, transform t ) const
    {
        return this->count_n<T, transform>(b, t);
    }

    template <typename U>
    int count( U const& b ) const
        //  Returns the number of elements that compare equal to b.
    {
        return this->count(b, ali::identity);
    }

    template <typename U>
    int count_n( array_const_ref<U> b ) const
        //  Returns the number of non-overlapping occurrences of b inside this array.
    {
        return this->count_n(b, ali::identity);
    }

    int count_n( array_const_ref<T> b ) const
    {
        return this->count_n<T>(b);
    }

    //  count_if

    template <typename predicate>
    int count_if( predicate p ) const
    {
        int n = 0;

        for ( int i = this->_size; i != 0; )
            if ( --i, p(this->at(i)) )
                ++n;
    
        return n;
    }

    //  is_sorted

    template <typename comparator>
    bool is_sorted( comparator compare ) const
    {
        if ( this->_size < 2 )
            ;   //  NOOP
        else for ( int i = this->_size - 1; i != 0; )
        {
            --i;
                
            int const result
                = compare(
                    this->at(i),
                    this->at(i + 1));

            if ( result > 0 )
                return false;
        }

        return true;
    }

    bool is_sorted( void ) const
    {
        return this->is_sorted(
            [] ( read_only_T a, read_only_T b )
            {
                using ali::compare;

                return compare(a, b);
            });
    }

    //  is_heap

    template <typename comparator>
    bool is_heap( comparator compare ) const
    {
        if ( this->_size < 2 )
            ;   //  NOOP
        else for ( int i = this->_size; i != 1; )
        {
            --i;

            int const result
                = compare(
                    this->at(i),            //  Child
                    this->at((i - 1) / 2)); //  Parent

            if ( result > 0 )
                return false;
        }

        return true;
    }
    
    bool is_heap( void ) const
    {
        return this->is_heap(
            [] ( read_only_T a, read_only_T b )
            {
                using ali::compare;

                return compare(a, b);
            });
    }

    array_const_ref<ali::uint8> as_blob( void ) const;

    is_internal_element_result
        is_internal_element( T const& a ) const
            //  post:   (0 <= result && result < this->size())
            //      ||  result < 0
    {
        bool const is_internal
            =   this->_begin <= &a
            &&  &a < this->_begin + this->_size;

        return {is_internal ? static_cast<int>(&a - this->_begin) : -1};
    }

    is_internal_pointer_result
        is_internal_pointer( T const* a ) const
            //  post:   (0 <= result && result <= this->size())
            //      ||  result < 0
            //  Note: Pointer to one past the last
            //  element is also internal pointer.
    {
        bool const is_internal
            =   this->_begin <= a
            &&  a <= this->_begin + this->_size;

        return {is_internal ? static_cast<int>(a - this->_begin) : -1};
    }

    is_internal_pointer_result
        is_internal_pointer( array_const_ref<T> a ) const;

    is_internal_pointer_result
        is_internal_pointer( array_const_ptr<T> a ) const;
    
private:    //  Methods
    array_const_ref_common& operator=(
        array_const_ref_common const& );

    template <typename U>
    bool _is_equal_to(
        array_const_ref<U> b,
        meta::define_bool_result<true> /* use mem_cmp */ ) const
    {
        return  this->_size == b._size
            &&  (   this->_size == 0
                ||  platform::memcmp(
                        this->_begin,
                        b._begin,
                        this->_size * sizeof(T)) == 0);
    }

    template <typename U>
    bool _is_equal_to(
        array_const_ref<U> b,
        meta::define_bool_result<false> /* use mem_cmp */ ) const
    {
        return this->is_equal_to<
            U, functor_types::identity>(
                b, ali::identity);
    }

private:    //  Data members
    T const*    _begin;
    int         _size;

    template <typename U>
    friend class array_const_ref_common;
    template <typename U>
    friend class array_const_ref;
    friend class c_string_const_ref;
    friend class c_wstring_const_ref;
    template <typename U>
    friend class array_ref_common;
    template <typename U>
    friend class array_ref;
    template <typename U>
    friend class array_const_ptr;
    friend class c_string_const_ptr;
    friend class c_wstring_const_ptr;
    template <typename U>
    friend class array_ptr;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class array_const_ref : public array_const_ref_common<T>
// ******************************************************************
{
public:
    array_const_ref( void ) {}

    //  Default copy ctor is OK - a reference can
    //  be initialized from another reference.

    array_const_ref( T const* begin, int size )
    :   array_const_ref_common<T>(begin, size)
    {}

    template <int n>
    array_const_ref( T const (&begin)[n] )
    :   array_const_ref_common<T>(begin, n)
    {}
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <>
class array_const_ref<ali::uint8>
// ******************************************************************
    : public array_const_ref_common<ali::uint8>
{
public:
    array_const_ref( void ) {}

    array_const_ref( ali::uint8 const* begin, int size )
    :   array_const_ref_common<ali::uint8>(begin, size)
    {}

    array_const_ref( ali::uint16 const* begin, int size )
    :   array_const_ref_common<ali::uint8>(
            reinterpret_cast<ali::uint8 const*>(begin),
            2 * size)
    {}

    array_const_ref( ali::uint32 const* begin, int size )
    :   array_const_ref_common<ali::uint8>(
            reinterpret_cast<ali::uint8 const*>(begin),
            4 * size)
    {}

    template <int n>
    array_const_ref( ali::uint8 const (&begin)[n] )
    :   array_const_ref_common<ali::uint8>(begin, n)
    {}

    template <int n>
    explicit array_const_ref( ali::uint16 const (&begin)[n] )
    :   array_const_ref_common<ali::uint8>(
            reinterpret_cast<ali::uint8 const*>(begin),
            2 * n)
    {}

    template <int n>
    explicit array_const_ref( ali::uint32 const (&begin)[n] )
    :   array_const_ref_common<ali::uint8>(
            reinterpret_cast<ali::uint8 const*>(begin),
            4 * n)
    {}

    //  int

    template <typename integer_type>
    integer_type int_be_at_( int pos, int byte_count ) const;
        // pre: byte_count in set {1 ... sizeof(integer_type)}

    template <typename integer_type>
    integer_type int_le_at_( int pos, int byte_count ) const;
        // pre: byte_count in set {1 ... sizeof(integer_type)}

    ali::uint32 int_be_at( int pos, int byte_count ) const;
        // pre: byte_count in set {1, 2, 3, 4}

    ali::uint32 int_le_at( int pos, int byte_count ) const;
        // pre: byte_count in set {1, 2, 3, 4}

    ali::uint8 int8_at( int pos ) const
    {
        return this->at(pos);
    }
    
    ali::uint16 int16_be_at( int pos ) const
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= this->_size - 2);
    
        return  static_cast<ali::uint16>(
                (static_cast<ali::uint32>(this->at(pos    )) <<  8)
            |   (static_cast<ali::uint32>(this->at(pos + 1))      ));
    }

    ali::uint16 int16_le_at( int pos ) const
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= this->_size - 2);
    
        return  static_cast<ali::uint16>(
                (static_cast<ali::uint32>(this->at(pos + 1)) <<  8)
            |   (static_cast<ali::uint32>(this->at(pos    ))      ));
    }

    ali::uint16 int16_ne_at( int pos ) const
        //  Native endian.
    {
        return to_native_from_little_endian(int16_le_at(pos));
    }
    
    ali::uint32 int32_be_at( int pos ) const
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= this->_size - 4);
    
        return  (static_cast<ali::uint32>(this->at(pos    )) << 24)
            |   (static_cast<ali::uint32>(this->at(pos + 1)) << 16)
            |   (static_cast<ali::uint32>(this->at(pos + 2)) <<  8)
            |   (static_cast<ali::uint32>(this->at(pos + 3))      );
    }

    ali::uint32 int32_le_at( int pos ) const
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= this->_size - 4);
    
        return  (static_cast<ali::uint32>(this->at(pos + 3)) << 24)
            |   (static_cast<ali::uint32>(this->at(pos + 2)) << 16)
            |   (static_cast<ali::uint32>(this->at(pos + 1)) <<  8)
            |   (static_cast<ali::uint32>(this->at(pos    ))      );
    }
    
    ali::uint32 int32_ne_at( int pos ) const
        //  Native endian.
    {
        return to_native_from_little_endian(int32_le_at(pos));
    }

    ali::uint64 int64_be_at( int pos ) const
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= this->_size - 8);
    
        return  (static_cast<ali::uint64>(this->int32_be_at(pos    )) << 32)
            |   (static_cast<ali::uint64>(this->int32_be_at(pos + 4))      );
    }

    ali::uint64 int64_le_at( int pos ) const
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= this->_size - 8);
    
        return  (static_cast<ali::uint64>(this->int32_le_at(pos + 4)) << 32)
            |   (static_cast<ali::uint64>(this->int32_le_at(pos    ))      );
    }
    
    ali::uint64 int64_ne_at( int pos ) const
        //  Native endian.
    {
        return to_native_from_little_endian(int64_le_at(pos));
    }

    array_const_ref<char> as_string( void ) const;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <>
class array_const_ref<char>
// ******************************************************************
    : public array_const_ref_common<char>
{
public:
    array_const_ref( void ) {}

    array_const_ref( char const* begin )
    :   array_const_ref_common<char>(
            begin, begin != nullptr ? str_len(begin) : 0)
    {}

    array_const_ref( char const* begin, int size )
    :   array_const_ref_common<char>(begin, size)
    {}

    bool nocase_is_equal_to( array_const_ref b ) const
    {
        return this->is_equal_to(b, ali::to_lower);
    }

    int nocase_compare( array_const_ref b ) const
    {
        return this->compare(b, ali::to_lower);
    }

    bool nocase_begins_with( char b ) const
    {
        return this->begins_with(b, ali::to_lower);
    }

    bool nocase_begins_with_n( array_const_ref b ) const
    {
        return this->begins_with_n(b, ali::to_lower);
    }

    bool nocase_ends_with( char b ) const
    {
        return this->ends_with(b, ali::to_lower);
    }

    bool nocase_ends_with_n( array_const_ref b ) const
    {
        return this->ends_with_n(b, ali::to_lower);
    }

    int nocase_index_of_first( char b ) const
    {
        return this->index_of_first(b, ali::to_lower);
    }

    int nocase_index_of_first_n( array_const_ref b ) const
    {
        return this->index_of_first_n(b, ali::to_lower);
    }

    int nocase_index_of_first_of( array_const_ref set ) const
    {
        return this->index_of_first_of(set, ali::to_lower);
    }

    int nocase_index_of_first_not_of( array_const_ref set ) const
    {
        return this->index_of_first_not_of(set, ali::to_lower);
    }

    //  index_of_first_unquoted

    template <typename transform>
    int index_of_first_unquoted( char b, transform t ) const
        //  Returns index of the first b outside of quotation marks.
        //  post:   result == size()    //  not found
        //      ||  t(at(result)) = t(b)
    {
        b = t(b);

        bool quoted = false;
    
        for ( int i = 0; i != this->_size; ++i )
        {
            if ( !quoted && t(this->at(i)) == b )
                return i;
        
            bool const quote
                =   this->at(i) == '\"'
                &&  (!quoted || this->at(i - 1) != '\\');
                        //  quoted implies i > 0 and i - 1 >= 0
            
            //  if ( quote )
            //      quoted = !quoted;
            //
            //  pre(quoted) | quote | post(quoted)
            //      F       |   F   |   F
            //      F       |   T   |   T
            //      T       |   F   |   T
            //      T       |   T   |   F
            //
            //  XOR

            quoted = quoted != quote;
        }
    
        return this->_size;
    }

    int index_of_first_unquoted( char b ) const
        //  Returns index of the first b outside of quotation marks.
        //  post:   result == size()    //  not found
        //      ||  at(result) = b
    {
        return this->index_of_first_unquoted(b, ali::identity);
    }

    int nocase_index_of_first_unquoted( char b ) const
        //  Returns index of the first b outside of quotation marks.
        //  post:   result == size()    //  not found
        //      ||  to_lower(at(result)) = to_lower(b)
    {
        return this->index_of_first_unquoted(b, ali::to_lower);
    }

    int nocase_index_of_last( char b ) const
    {
        return this->index_of_last(b, ali::to_lower);
    }

    int nocase_index_of_last_n( array_const_ref b ) const
    {
        return this->index_of_last_n(b, ali::to_lower);
    }

    int nocase_index_of_last_of( array_const_ref set ) const
    {
        return this->index_of_last_of(set, ali::to_lower);
    }

    int nocase_index_of_last_not_of( array_const_ref set ) const
    {
        return this->index_of_last_not_of(set, ali::to_lower);
    }

    bool nocase_contains( char b ) const
    {
        return this->contains(b, ali::to_lower);
    }

    bool nocase_contains_n( array_const_ref b ) const
    {
        return this->contains_n(b, ali::to_lower);
    }

    int nocase_count( char b ) const
    {
        return this->count(b, ali::to_lower);
    }

    int nocase_count_n( array_const_ref b ) const
    {
        return this->count_n(b, ali::to_lower);
    }

    //  trim_left

    using array_const_ref_common<char>::trim_left;
        
    array_const_ref trim_left( void ) const;
        //  post:   result.is_empty()
        //      ||  !is_space(result.front())

    array_const_ref nocase_trim_left( array_const_ref set ) const;
        //  post:   result.is_empty()
        //      ||  !charset.nocase_contains(result.front())

    //  trim_right

    using array_const_ref_common<char>::trim_right;
        
    array_const_ref trim_right( void ) const;
        //  post:   result.is_empty()
        //      ||  !is_space(result.back())

    array_const_ref nocase_trim_right( array_const_ref set ) const;
        //  post:   result.is_empty()
        //      ||  !set.nocase_contains(result.back())

    //  trim

    using array_const_ref_common<char>::trim;

    array_const_ref trim( void ) const;
        //  post:   result.is_empty()
        //      ||  (   !is_space(result.front())
        //          &&  !is_space(result.back()))

    array_const_ref nocase_trim( array_const_ref set ) const;
        //  post:   result.is_empty()
        //      ||  (   !set.nocase_contains(result.front())
        //          &&  !set.nocase_contains(result.back()))

    //  is_internal_pointer

    using array_const_ref_common<char>::is_internal_pointer;

    is_internal_pointer_result
        is_internal_pointer( c_string_const_ptr a ) const;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <>
class array_const_ref<ali::wchar>
// ******************************************************************
    : public array_const_ref_common<ali::wchar>
{
public:
    array_const_ref( void ) {}

    array_const_ref( ali::wchar const* begin )
    :   array_const_ref_common<ali::wchar>(
            begin, begin != nullptr ? str_len(begin) : 0)
    {}

    array_const_ref( ali::wchar const* begin, int size )
    :   array_const_ref_common<ali::wchar>(begin, size)
    {}

    bool nocase_is_equal_to( array_const_ref b ) const
    {
        return this->is_equal_to(b, ali::to_lower);
    }

    int nocase_compare( array_const_ref b ) const
    {
        return this->compare(b, ali::to_lower);
    }

    bool nocase_begins_with( ali::wchar b ) const
    {
        return this->begins_with(b, ali::to_lower);
    }

    bool nocase_begins_with_n( array_const_ref b ) const
    {
        return this->begins_with_n(b, ali::to_lower);
    }

    bool nocase_ends_with( ali::wchar b ) const
    {
        return this->ends_with(b, ali::to_lower);
    }

    bool nocase_ends_with_n( array_const_ref b ) const
    {
        return this->ends_with_n(b, ali::to_lower);
    }

    int nocase_index_of_first( ali::wchar b ) const
    {
        return this->index_of_first(b, ali::to_lower);
    }

    int nocase_index_of_first_n( array_const_ref b ) const
    {
        return this->index_of_first_n(b, ali::to_lower);
    }

    int nocase_index_of_first_of( array_const_ref set ) const
    {
        return this->index_of_first_of(set, ali::to_lower);
    }

    int nocase_index_of_first_not_of( array_const_ref set ) const
    {
        return this->index_of_first_not_of(set, ali::to_lower);
    }

    //  index_of_first_unquoted

    template <typename transform>
    int index_of_first_unquoted( ali::wchar b, transform t ) const
        //  Returns index of the first b outside of quotation marks.
        //  post:   result == size()    //  not found
        //      ||  t(at(result)) = t(b)
    {
        b = t(b);

        bool quoted = false;
    
        for ( int i = 0; i != this->_size; ++i )
        {
            if ( !quoted && t(this->at(i)) == b )
                return i;
        
            bool const quote
                =   this->at(i) == L'\"'
                &&  (!quoted || this->at(i - 1) != L'\\');
                        //  quoted implies i > 0 and i - 1 >= 0
            
            //  if ( quote )
            //      quoted = !quoted;
            //
            //  pre(quoted) | quote | post(quoted)
            //      F       |   F   |   F
            //      F       |   T   |   T
            //      T       |   F   |   T
            //      T       |   T   |   F
            //
            //  XOR

            quoted = quoted != quote;
        }
    
        return this->_size;
    }

    int index_of_first_unquoted( ali::wchar b ) const
        //  Returns index of the first b outside of quotation marks.
        //  post:   result == size()    //  not found
        //      ||  at(result) = b
    {
        return this->index_of_first_unquoted(b, ali::identity);
    }

    int nocase_index_of_first_unquoted( ali::wchar b ) const
        //  Returns index of the first b outside of quotation marks.
        //  post:   result == size()    //  not found
        //      ||  to_lower(at(result)) = to_lower(b)
    {
        return this->index_of_first_unquoted(b, ali::to_lower);
    }

    int nocase_index_of_last( ali::wchar b ) const
    {
        return this->index_of_last(b, ali::to_lower);
    }

    int nocase_index_of_last_n( array_const_ref b ) const
    {
        return this->index_of_last_n(b, ali::to_lower);
    }

    int nocase_index_of_last_of( array_const_ref set ) const
    {
        return this->index_of_last_of(set, ali::to_lower);
    }

    int nocase_index_of_last_not_of( array_const_ref set ) const
    {
        return this->index_of_last_not_of(set, ali::to_lower);
    }

    bool nocase_contains( ali::wchar b ) const
    {
        return this->contains(b, ali::to_lower);
    }

    bool nocase_contains_n( array_const_ref b ) const
    {
        return this->contains_n(b, ali::to_lower);
    }

    int nocase_count( ali::wchar b ) const
    {
        return this->count(b, ali::to_lower);
    }

    int nocase_count_n( array_const_ref b ) const
    {
        return this->count_n(b, ali::to_lower);
    }

    //  trim_left

    using array_const_ref_common<ali::wchar>::trim_left;
        
    array_const_ref trim_left( void ) const;
        //  post:   result.is_empty()
        //      ||  !is_space(result.front())

    array_const_ref nocase_trim_left( array_const_ref set ) const;
        //  post:   result.is_empty()
        //      ||  !charset.nocase_contains(result.front())

    //  trim_right

    using array_const_ref_common<ali::wchar>::trim_right;
        
    array_const_ref trim_right( void ) const;
        //  post:   result.is_empty()
        //      ||  !is_space(result.back())

    array_const_ref nocase_trim_right( array_const_ref set ) const;
        //  post:   result.is_empty()
        //      ||  !set.nocase_contains(result.back())

    //  trim

    using array_const_ref_common<ali::wchar>::trim;

    array_const_ref trim( void ) const;
        //  post:   result.is_empty()
        //      ||  (   !is_space(result.front())
        //          &&  !is_space(result.back()))

    array_const_ref nocase_trim( array_const_ref set ) const;
        //  post:   result.is_empty()
        //      ||  (   !set.nocase_contains(result.front())
        //          &&  !set.nocase_contains(result.back()))

    //  is_internal_pointer

    using array_const_ref_common<ali::wchar>::is_internal_pointer;

    is_internal_pointer_result
        is_internal_pointer( c_wstring_const_ptr a ) const;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <> template <>
int array_const_ref_common<unsigned char>::compare(
    array_const_ref<unsigned char> b,
    functor_types::identity ) const;
// ******************************************************************
//  Can use memcmp.
// ******************************************************************

//
//  BEWARE:
//  While we would like to use memcmp for the following two
//  specializations as well, it unfortunately gives incorrect
//  results when the arrays contain negative values.
//  It's because the memcmp function interprets the data
//  as unsigned chars.
//
//  Example: (signed char)-1 < 0, but (unsigned char)-1 = 255 > 0
//
//  We cannot use strncmp either, because it stops at the first
//  zero it encounters.
//
//  Having said that, we are going to use memcmp for chars
//  as there is little harm in doing so, but not for the
//  signed chars.
//  

// ******************************************************************
template <> template <>
int array_const_ref_common<char>::compare(
    array_const_ref<char> b,
    functor_types::identity ) const;
template <> template <>
int array_const_ref_common<unsigned char>::compare(
    array_const_ref<char> b,
    functor_types::identity ) const;
template <> template <>
int array_const_ref_common<char>::compare(
    array_const_ref<unsigned char> b,
    functor_types::identity ) const;
// ******************************************************************
//  Using memcmp does little harm.
// ******************************************************************

// ******************************************************************
//template <> template <>
//int array_const_ref_common<signed char>::compare(
//    array_const_ref<signed char> b,
//    functor_types::identity ) const;
// ******************************************************************
//  Using memcmp would give INCORRECT results!
// ******************************************************************

// ******************************************************************
template <> template <>
int array_const_ref_common<ali::wchar>::compare(
    array_const_ref<ali::wchar> b,
    functor_types::identity ) const;
// ******************************************************************
//  Can use wmemcmp.
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class array_ref_common : public array_const_ref<T>
// ******************************************************************
{
    typedef typename array_const_ref<T>::read_only_T read_only_T;

public:
    array_ref_common( void ) {}

    array_ref_common( T* begin, int size )
    :   array_const_ref<T>(begin, size)
    {}

    //array_ref_common& operator=( array_ref_common const& b )
    //{
    //    const_cast<array_ref_common const*>(this)->operator=(b);
    //        //  We are ADDING const here.
    //    return *this;
    //}

    array_ref_common const& operator=( array_ref_common const& b ) const
        //  pre:    size() == b.size()
    {
        return this->operator=(static_cast<array_const_ref<T> const&>(b));
    }

    array_ref_common const& operator=( array_const_ref<T> b ) const
        //  pre:    size() == b.size()
    {
        ali_assert(this->_size == b._size);
        
        if ( this->_size == b._size )
            this->copy_front(b);

        return *this;
    }

    T& front( void ) const
        //  pre:    !is_empty()
    {
        ali_assert(!this->is_empty());
        
        return this->data()[0];
    }

    T& back( void ) const
        //  pre:    !is_empty()
    {
        ali_assert(!this->is_empty());
        
        return this->data()[this->_size - 1];
    }

    T& back( int i ) const
        //  pre:    0 <= i && i < size()
    {
        ali_assert(0 <= i);
        ali_assert(i < this->_size);
        
        return this->data()[this->_size - 1 - i];
    }

    T& at( int i ) const
    {
        ali_assert(0 <= i);
        ali_assert(i < this->_size);

        return this->data()[i];
    }

    T& operator[]( int i ) const
    {
        return this->at(i);
    }

    T* data( void ) const
    {
        return const_cast<T*>(this->_begin);
            //  Legal, we started with non-const array.
    }

    //  mutable_ref

    array_ref<T> mutable_ref( int pos, int n ) const;
        //  pre:    0 <= pos && pos <= size()
        //      &&  0 <= n && n <= size() - pos
    array_ref<T> mutable_ref( void ) const;
    array_ref<T> mutable_ref_left( int pos ) const;
        //  pre:    0 <= pos && pos <= size()
    array_ref<T> mutable_ref_right( int pos ) const;
        //  pre:    0 <= pos && pos <= size()
    array_ref<T> mutable_ref_front( int n ) const;
        //  pre:    0 <= n && n <= size()
    array_ref<T> mutable_ref_back( int n ) const;
        //  pre:    0 <= n && n <= size()
    array_ref<T> mutable_ref_not_front( int n ) const;
        //  pre:    0 <= n && n <= size()
    array_ref<T> mutable_ref_not_back( int n ) const;
        //  pre:    0 <= n && n <= size()

    //  mutable_pointer

    array_ptr<T> mutable_pointer( int pos, int n ) const;
        //  pre:    0 <= pos && pos <= size()
        //      &&  0 <= n && n <= size() - pos
    array_ptr<T> mutable_pointer( void ) const;
    array_ptr<T> mutable_pointer_left( int pos ) const;
        //  pre:    0 <= pos && pos <= size()
    array_ptr<T> mutable_pointer_right( int pos ) const;
        //  pre:    0 <= pos && pos <= size()
    array_ptr<T> mutable_pointer_front( int n ) const;
        //  pre:    0 <= n && n <= size()
    array_ptr<T> mutable_pointer_back( int n ) const;
        //  pre:    0 <= n && n <= size()
    array_ptr<T> mutable_pointer_not_front( int n ) const;
        //  pre:    0 <= n && n <= size()
    array_ptr<T> mutable_pointer_not_back( int n ) const;
        //  pre:    0 <= n && n <= size()

    //  fill
    array_ref<T> const& fill( void ) const
        //  Fill with default constructed element.
    {
        return this->fill(T());
    }

    array_ref<T> const& fill( read_only_T b ) const
    {
        hidden::fill(this->data(), b, this->_size);

        return this->self();
    }

    //  copy

    template <typename U>
    array_ref<T> const& copy( int pos, array_const_ref<U> b ) const;

    array_ref<T> const& copy( int pos, array_const_ref<T> b ) const
    {
        return this->copy<T>(pos, b);
    }

    //  copy_front

    template <typename U>
    array_ref<T> const& copy_front( array_const_ref<U> b ) const
    {
        hidden::copy(
            this->data(), b._begin,
            ali::mini(this->_size, b._size));

        return this->self();
    }

    array_ref<T> const& copy_front( array_const_ref<T> b ) const
    {
        return this->copy_front<T>(b);
    }

    //  copy_back

    template <typename U>
    array_ref<T> const& copy_back( array_const_ref<U> b ) const
    {
        int const n = ali::mini(this->_size, b._size);

        hidden::copy(
            this->data() + (this->_size - n),
            b._begin + (b._size - n),
            n);

        return this->self();
    }

    array_ref<T> const& copy_back( array_const_ref<T> b ) const
    {
        return this->copy_back<T>(b);
    }

    //  move

    template <typename U>
    array_ref<T> const& move( int pos, array_ref<U> b ) const;

    array_ref<T> const& move( int pos, array_ref<T> b ) const
    {
        return this->move<T>(pos, b);
    }

    //  move_front

    template <typename U>
    array_ref<T> const& move_front( array_ref<U> b ) const
    {
        hidden::move(
            this->data(), b.data(),
            ali::mini(this->_size, b._size));

        return this->self();
    }

    array_ref<T> const& move_front( array_ref<T> b ) const
    {
        return this->move_front<T>(b);
    }

    //  move_back

    template <typename U>
    array_ref<T> const& move_back( array_ref<U> b ) const
    {
        int const n = ali::mini(this->_size, b._size);

        hidden::move(
            this->data() + (this->_size - n),
            b.data() + (b._size - n),
            n);

        return this->self();
    }

    array_ref<T> const& move_back( array_ref<T> b ) const
    {
        return this->move_back<T>(b);
    }

    //  move_if_noexcept

    template <typename U>
    array_ref<T> const& move_if_noexcept( int pos, array_ref<U> b ) const;

    array_ref<T> const& move_if_noexcept( int pos, array_ref<T> b ) const
    {
        return this->move_if_noexcept<T>(pos, b);
    }

    //  move_front_if_noexcept

    template <typename U>
    array_ref<T> const& move_front_if_noexcept( array_ref<U> b ) const
    {
        hidden::move_if_noexcept(
            this->data(), b.data(),
            ali::mini(this->_size, b._size));

        return this->self();
    }

    array_ref<T> const& move_front_if_noexcept( array_ref<T> b ) const
    {
        return this->move_front_if_noexcept<T>(b);
    }

    //  move_back_if_noexcept

    template <typename U>
    array_ref<T> const& move_back_if_noexcept( array_ref<U> b ) const
    {
        int const n = ali::mini(this->_size, b._size);

        hidden::move_if_noexcept(
            this->data() + (this->_size - n),
            b.data() + (b._size - n),
            n);

        return this->self();
    }

    array_ref<T> const& move_back_if_noexcept( array_ref<T> b ) const
    {
        return this->move_back_if_noexcept<T>(b);
    }

    //  for_each

    template <typename body>
    array_ref<T> const& for_each( body b ) const
    {
        for ( int i = this->_size; i != 0; )
            b(this->at(--i));
        
        return this->self();
    }

    //  combine

    template <typename operation>
    array_ref<T> const& combine( read_only_T b, operation op ) const
    {
        for ( int i = this->_size; i != 0; )
            op(this->at(--i), b);
        
        return this->self();
    }

    template <typename operation>
    array_ref<T> const& combine( int pos, array_const_ref<T> b, operation op ) const;

    //  combine_front

    template <typename operation>
    array_ref<T> const& combine_front( array_const_ref<T> b, operation op ) const
    {
        for ( int i = ali::mini(this->_size, b._size); i != 0; )
            --i, op(this->at(i), b.at(i));
        
        return this->self();
    }

    //  combine_back

    template <typename operation>
    array_ref<T> const& combine_back( array_const_ref<T> b, operation op ) const;

    //  bitwise_and

    array_ref<T> const& operator&=( read_only_T b ) const
    {
        return this->bitwise_and(b);
    }

    array_ref<T> const& operator&=( array_const_ref<T> b ) const
    {
        ali_assert(this->_size == b._size);

        if ( this->_size == b._size )
            this->bitwise_and_front(b);

        return this->self();
    }

    array_ref<T> const& bitwise_and( read_only_T b ) const
    {
        return this->combine(b, []( T& a, read_only_T b ) { a &= b; });
    }

    array_ref<T> const& bitwise_and( int pos, array_const_ref<T> b ) const
    {
        return this->combine(pos, b, []( T& a, read_only_T b ) { a &= b; });
    }

    //  bitwise_and_front

    array_ref<T> const& bitwise_and_front( array_const_ref<T> b ) const
    {
        return this->combine_front(b, []( T& a, read_only_T b ) { a &= b; });
    }

    //  bitwise_and_back

    array_ref<T> const& bitwise_and_back( array_const_ref<T> b ) const
    {
        return this->combine_back(b, []( T& a, read_only_T b ) { a &= b; });
    }

    //  bitwise_or

    array_ref<T> const& operator|=( read_only_T b ) const
    {
        return this->bitwise_or(b);
    }

    array_ref<T> const& operator|=( array_const_ref<T> b ) const
    {
        ali_assert(this->_size == b._size);

        if ( this->_size == b._size )
            this->bitwise_or_front(b);

        return this->self();
    }

    array_ref<T> const& bitwise_or( read_only_T b ) const
    {
        return this->combine(b, []( T& a, read_only_T b ) { a |= b; });
    }

    array_ref<T> const& bitwise_or( int pos, array_const_ref<T> b ) const
    {
        return this->combine(pos, b, []( T& a, read_only_T b ) { a |= b; });
    }

    //  bitwise_or_front

    array_ref<T> const& bitwise_or_front( array_const_ref<T> b ) const
    {
        return this->combine_front(b, []( T& a, read_only_T b ) { a |= b; });
    }

    //  bitwise_or_back

    array_ref<T> const& bitwise_or_back( array_const_ref<T> b ) const
    {
        return this->combine_back(b, []( T& a, read_only_T b ) { a |= b; });
    }

    //  bitwise_xor

    array_ref<T> const& operator^=( read_only_T b ) const
    {
        return this->bitwise_xor(b);
    }

    array_ref<T> const& operator^=( array_const_ref<T> b ) const
    {
        ali_assert(this->_size == b._size);

        if ( this->_size == b._size )
            this->bitwise_xor_front(b);

        return this->self();
    }

    array_ref<T> const& bitwise_xor( read_only_T b ) const
    {
        return this->combine(b, []( T& a, read_only_T b ) { a ^= b; });
    }

    array_ref<T> const& bitwise_xor( int pos, array_const_ref<T> b ) const
    {
        return this->combine(pos, b, []( T& a, read_only_T b ) { a ^= b; });
    }

    //  bitwise_xor_front

    array_ref<T> const& bitwise_xor_front( array_const_ref<T> b ) const
    {
        return this->combine_front(b, []( T& a, read_only_T b ) { a ^= b; });
    }

    //  bitwise_xor_back

    array_ref<T> const& bitwise_xor_back( array_const_ref<T> b ) const
    {
        return this->combine_back(b, []( T& a, read_only_T b ) { a ^= b; });
    }

    //  reverse
    
    array_ref<T> const& reverse( void ) const
    {
        using ali::swap;

        if ( this->_size > 1 )
            for ( int left = 0, right = this->_size - 1;
                left < right; ++left, --right )
                    swap(this->at(left), this->at(right));

        return this->self();
    }

    //  swap_block

    array_ref<T> const& swap_block( int pos, array_ref<T> b ) const;

    //  swap_block_front

    array_ref<T> const& swap_block_front( array_ref<T> b ) const
    {
        using ali::swap;

        for ( int i = ali::mini(this->_size, b._size); i != 0; )
            --i, swap(this->at(i), b.at(i));

        return this->self();
    }

    //  swap_block_back

    array_ref<T> const& swap_block_back( array_ref<T> b ) const;

    //  rotate

    array_ref<T> const& rotate_left( int n ) const
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_size);

        if ( n != 0 && n != this->_size )
        {
            int const pos = n;

            int n_left = n;

            int n_right = this->_size - n_left;

            while ( n_left != n_right )
            {
                if ( n_left > n_right )
                {
                    this->swap_block(
                        pos - n_left,
                        this->mutable_ref(
                            pos, n_right));

                    n_left -= n_right;
                }
                else
                {
                    this->swap_block(
                        pos - n_left,
                        this->mutable_ref(
                            (pos - n_left) + n_right,
                            n_left));

                    n_right -= n_left;
                }
            }

            this->swap_block(
                pos - n_left,
                this->mutable_ref(
                    pos, n_left));
        }

        return this->self();
    }

    array_ref<T> const& rotate_right( int n ) const
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_size);

        return this->rotate_left(this->_size - n);
    }

    //  next_permutation

    template <typename comparator>
	bool next_permutation( comparator compare ) const
	{
        int next{this->_size};

        if ( next == 0 || --next == 0 )
		    return false;

	    for ( ;; )
		{	
            int const next1{next};

		    if ( compare(this->at(--next), this->at(next1)) < 0 )
            {
			    int mid{this->_size};

                while ( compare(this->at(next), this->at(--mid)) >= 0 )
				    ;   //  Noop

                using ali::swap;
                swap(this->at(next), this->at(mid));

                this->mutable_ref_right(next1).reverse();

                return true;
            }

		    if ( next == 0 )
            {
			    this->reverse();

                return false;
            }
		}
	}

    bool next_permutation( void ) const
    {
        return this->next_permutation(
            [] ( read_only_T a, read_only_T b )
            {
                using ali::compare;

                return compare(a, b);
            });
    }


    //  stable_partition

    template <typename predicate>
    int stable_partition( predicate p ) const
        //  Rearranges the array into left and right parts.
        //  Elements in the left part satisfy the given predicate
        //  and are guaranteed to have the same relative order
        //  as in the original array.
        //  Elements in the right part DON'T satisfy the given
        //  predicate and are guaranteed to have the same relative
        //  order as in the original array.
        //  Returns size of the resulting left part (or, equivalently,
        //  index of the first element of the right part).
    {
        if ( this->_size == 0 )
            return 0;

        if ( this->_size == 1 )
            return static_cast<int>(p(this->front()));

        int const m{this->_size / 2};

        int const begin{this->mutable_ref_left(m).stable_partition(p)};
        
        int const end{m + this->mutable_ref_right(m).stable_partition(p)};
        
        this->mutable_ref(begin, end - begin).rotate_left(m - begin);
        
        return end - (m - begin);
    }

    //  stable_partition_left_only

    template <typename predicate>
    int stable_partition_left_only( predicate p ) const
        //  Rearranges the array into left and right parts.
        //  Elements in the left part satisfy the given predicate
        //  and are guaranteed to have the same relative order
        //  as in the original array.
        //  The content of the right part is UNDEFINED.
        //  Returns size of the resulting left part.
    {
        int iw = 0;
        
        for ( int ir = 0; ir != this->_size; ++ir )
            if ( p(this->at(ir)) )
                this->at(iw++) = ali::move(this->at(ir));

        return iw;
    }

    //  insertion_sort

    template <typename comparator>
    array_ref<T> const& insertion_sort( comparator compare ) const
    {
        using ali::swap;

        if ( this->_size < 2 )
            ;   //  NOOP
        else for ( int i = 1; i != this->_size; ++i )
        {
            for ( int j = i; j != 0 && compare(this->at(j - 1), this->at(j)) > 0; --j )
                swap(this->at(j - 1), this->at(j));
        }

        return this->self();
    }

    array_ref<T> const& insertion_sort( void ) const
    {
        return this->insertion_sort(
            [] ( read_only_T a, read_only_T b )
            {
                using ali::compare;

                return compare(a, b);
            });
    }

    //  quick_sort

    template <typename comparator>
    int unstable_partition_around_pivot(
        int index_of_pivot, comparator compare ) const
    {
        ali_assert(0 <= index_of_pivot);
        ali_assert(index_of_pivot < this->_size);

        using ali::swap;

        if ( index_of_pivot != this->_size - 1 )
        {
            //  Move pivot to the end.

            swap(this->at(index_of_pivot), this->at(this->_size - 1));
            
            index_of_pivot = this->_size - 1;
        }

        T& pivot = this->at(index_of_pivot);
        
        for ( int i = this->_size - 1; i != 0; )
        {
            int const result = compare(this->at(--i), pivot);

            if ( result > 0 && i != --index_of_pivot )
                swap(this->at(i), this->at(index_of_pivot));
        }

        if ( index_of_pivot != this->_size - 1 )
            swap(this->at(index_of_pivot), pivot);
                //  Move pivot back to its proper place.

        return index_of_pivot;
    }

    int unstable_partition_around_pivot( int index_of_pivot ) const
    {
        return this->unstable_partition_around_pivot(
            index_of_pivot,
            [] ( read_only_T a, read_only_T b )
            {
                using ali::compare;

                return compare(a, b);
            });
    }

    template <typename comparator>
    array_ref<T> const& quick_sort( comparator compare ) const;

    array_ref<T> const& quick_sort( void ) const
    {
        return this->quick_sort(
            [] ( read_only_T a, read_only_T b )
            {
                using ali::compare;

                return compare(a, b);
            });
    }

    //  heap_sort

    //  The heap invariant:
    //  for all i in set {1 .. size() - 1} &
    //      compare(
    //          at(i),              //  Child
    //          at((i - 1) / 2))    //  Parent
    //              <= 0

    template <typename comparator>
    array_ref<T> const& up_heap( int i, comparator compare ) const
        //  Assumes the heap property satisfied everywhere except
        //  between element at i and its parent at (i - 1) / 2.
        //  Ensures the heap property is satisfied everywhere.
    {
        ali_assert(0 <= i);
        ali_assert(i < this->_size);

        for ( ; i != 0 && compare(this->at(i), this->at((i - 1) / 2)) > 0;
                i = (i - 1) / 2 )
        {
            using ali::swap;

            swap(this->at(i), this->at((i - 1) / 2));
        }

        return this->self();
    }

    template <typename comparator>
    array_ref<T> const& down_heap( int i, comparator compare ) const
        //  Assumes the heap property satisfied everywhere except
        //  between element at i and its children at 2i+1 and 2i+2.
        //  Ensures the heap property is satisfied everywhere.
    {
        ali_assert(0 <= i);
        ali_assert(i < this->_size);

        //  The first element without any descendant (leaf) is at _size / 2.
        

        for ( int const leaf = this->_size / 2; i < leaf; )
        {
            int const left = 2 * i + 1;

            ali_assert(left < this->_size);
                //  left = 2 * i + 1 is always < _size
        
            int const right = left + 1;

            ali_assert(right <= this->_size);
        
            int largest = compare(this->at(left), this->at(i)) > 0 ? left : i;
        
            if ( right != this->_size && compare(this->at(right), this->at(largest)) > 0 )
                largest = right;
            
            if ( largest == i )
                break;
            
            using ali::swap;

            swap(this->at(i), this->at(largest));
            
            i = largest;
        }

        return this->self();
    }

    template <typename comparator>
    array_ref<T> const& make_heap( comparator compare ) const
    {
        for ( int i = this->_size / 2; i != 0; )
            this->down_heap(--i, compare);
        
        return this->self();
    }

    template <typename comparator>
    array_ref<T> const& heap_sort( comparator compare ) const;

    array_ref<T> const& heap_sort( void ) const
    {
        return this->heap_sort(
            [] ( read_only_T a, read_only_T b )
            {
                using ali::compare;

                return compare(a, b);
            });
    }

    //  sort

    template <typename comparator>
    array_ref<T> const& sort( comparator compare ) const
    {
        return this->heap_sort(compare);
    }

    array_ref<T> const& sort( void ) const
    {
        return this->heap_sort();
    }

    array_ref<ali::uint8> as_mutable_blob( void ) const;

protected:  //  Methods
    array_ref<T> const& self( void ) const
    {
        return static_cast<array_ref<T> const&>(*this);
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class array_ref : public array_ref_common<T>
// ******************************************************************
{
public:
    array_ref( void ) {}

    array_ref( T* begin, int size )
    :   array_ref_common<T>(begin, size)
    {}

    template <int n>
    array_ref( T (&begin)[n] )
    :   array_ref_common<T>(begin, n)
    {}

    //array_ref& operator=( array_ref const& b )
    //    //  pre:    size() == b.size()
    //{
    //    array_ref_common<T>::operator=(
    //        static_cast<array_ref_common<T> const&>(b));
    //    
    //    return *this;
    //}

    array_ref const& operator=( array_ref const& b ) const
        //  pre:    size() == b.size()
    {
        array_ref_common<T>::operator=(
            static_cast<array_ref_common<T> const&>(b));
        
        return *this;
    }

    array_ref const& operator=( array_const_ref<T> b ) const
        //  pre:    size() == b.size()
    {
        array_ref_common<T>::operator=(b);
        
        return *this;
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <>
class array_ref<ali::uint8> : public array_ref_common<ali::uint8>
// ******************************************************************
{
public:
    array_ref( void ) {}

    array_ref( ali::uint8* begin, int size )
    :   array_ref_common<ali::uint8>(begin, size)
    {}

    array_ref( ali::uint16* begin, int size )
    :   array_ref_common<ali::uint8>(
            reinterpret_cast<ali::uint8*>(begin),
            2 * size)
    {}

    array_ref( ali::uint32* begin, int size )
    :   array_ref_common<ali::uint8>(
            reinterpret_cast<ali::uint8*>(begin),
            4 * size)
    {}

    template <int n>
    array_ref( ali::uint8 (&begin)[n] )
    :   array_ref_common<ali::uint8>(begin, n)
    {}

    template <int n>
    explicit array_ref( ali::uint16 (&begin)[n] )
    :   array_ref_common<ali::uint8>(
            reinterpret_cast<ali::uint8*>(begin),
            2 * n)
    {}

    template <int n>
    explicit array_ref( ali::uint32 (&begin)[n] )
    :   array_ref_common<ali::uint8>(
            reinterpret_cast<ali::uint8*>(begin),
            4 * n)
    {}

    //array_ref& operator=( array_ref const& b )
    //    //  pre:    size() == b.size()
    //{
    //    array_ref_common<ali::uint8>::operator=(
    //        static_cast<array_ref_common<ali::uint8> const&>(b));
    //    
    //    return *this;
    //}

    array_ref const& operator=( array_ref const& b ) const
        //  pre:    size() == b.size()
    {
        array_ref_common<ali::uint8>::operator=(
            static_cast<array_ref_common<ali::uint8> const&>(b));
        
        return *this;
    }

    array_ref const& operator=( array_const_ref<ali::uint8> b ) const
        //  pre:    size() == b.size()
    {
        array_ref_common<ali::uint8>::operator=(b);
        
        return *this;
    }

    //  int

    // pre: byte_count in set {1, 2, 3, 4}
    array_ref const& set_int_be_at(
        int pos, ali::uint32 value, int byte_count ) const;
        
    // pre: byte_count in set {1, 2, 3, 4}
    array_ref const& set_int_le_at(
        int pos, ali::uint32 value, int byte_count ) const;

    // pre: byte_count in set {1, 2, 3, 4, 5, 6, 7, 8}
    array_ref const& set_long_be_at(
        int pos, ali::uint64 value, int byte_count ) const;
        
    // pre: byte_count in set {1, 2, 3, 4, 5, 6, 7, 8}
    array_ref const& set_long_le_at(
        int pos, ali::uint64 value, int byte_count ) const;

    array_ref const& set_int8_at( int pos, ali::uint8 value ) const
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= this->_size - 1);
        
        this->data()[pos] = value;
        
        return *this;
    }

    array_ref const& set_int16_be_at( int pos, ali::uint16 value ) const
    {
        return this->set_int_be_at(pos, value, 2);
    }

    array_ref const& set_int16_le_at( int pos, ali::uint16 value ) const
    {
        return this->set_int_le_at(pos, value, 2);
    }
    
    array_ref const& set_int16_ne_at( int pos, ali::uint16 value ) const
        //  Native endian.
    {
        return this->set_int_le_at(pos, to_little_endian(value), 2);
    }

    array_ref const& set_int32_be_at( int pos, ali::uint32 value ) const
    {
        return this->set_int_be_at(pos, value, 4);
    }

    array_ref const& set_int32_le_at( int pos, ali::uint32 value ) const
    {
        return this->set_int_le_at(pos, value, 4);
    }
    
    array_ref const& set_int32_ne_at( int pos, ali::uint32 value ) const
        //  Native endian.
    {
        return this->set_int_le_at(pos, to_little_endian(value), 4);
    }

    array_ref const& set_int64_be_at( int pos, ali::uint64 value ) const
    {
        return this->set_long_be_at(pos, value, 8);
    }
    
    array_ref const& set_int64_le_at( int pos, ali::uint64 value ) const
    {
        return this->set_long_le_at(pos, value, 8);
    }
    
    array_ref const& set_int64_ne_at( int pos, ali::uint64 value ) const
        //  Native endian.
    {
        return this->set_long_le_at(pos, to_little_endian(value), 8);
    }

    //  shift

    array_ref const& logical_shift_right( int n ) const;
    array_ref const& logical_shift_left( int n ) const;

    array_ref const& operator>>=( int n ) const
    {
        return this->logical_shift_right(n);
    }

    array_ref const& operator<<=( int n ) const
    {
        return this->logical_shift_left(n);
    }

    //  increment

    int increment_be( void ) const;
        //  Increments this blob as a big endian large integer.
        //  Returns carry.

    int increment_le( void ) const;
        //  Increments this blob as a little endian large integer.
        //  Returns carry.

    array_ref<char> as_mutable_string( void ) const;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <>
class array_ref<char> : public array_ref_common<char>
// ******************************************************************
{
public:
    array_ref( void ) {}

    array_ref( char* begin, int size )
    :   array_ref_common<char>(begin, size)
    {}

    //array_ref& operator=( array_ref const& b )
    //    //  pre:    size() == b.size()
    //{
    //    array_ref_common<char>::operator=(
    //        static_cast<array_ref_common<char> const&>(b));
    //    
    //    return *this;
    //}

    array_ref const& operator=( array_ref const& b ) const
        //  pre:    size() == b.size()
    {
        array_ref_common<char>::operator=(
            static_cast<array_ref_common<char> const&>(b));
        
        return *this;
    }

    array_ref const& operator=( array_const_ref<char> b ) const
        //  pre:    size() == b.size()
    {
        array_ref_common<char>::operator=(b);
        
        return *this;
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <>
class array_ref<ali::wchar> : public array_ref_common<ali::wchar>
// ******************************************************************
{
public:
    array_ref( void ) {}

    array_ref( ali::wchar* begin, int size )
    :   array_ref_common<ali::wchar>(begin, size)
    {}

    //array_ref& operator=( array_ref const& b )
    //    //  pre:    size() == b.size()
    //{
    //    array_ref_common<ali::wchar>::operator=(
    //        static_cast<array_ref_common<ali::wchar> const&>(b));
    //    
    //    return *this;
    //}

    array_ref const& operator=( array_ref const& b ) const
        //  pre:    size() == b.size()
    {
        array_ref_common<ali::wchar>::operator=(
            static_cast<array_ref_common<ali::wchar> const&>(b));
        
        return *this;
    }

    array_ref const& operator=( array_const_ref<ali::wchar> b ) const
        //  pre:    size() == b.size()
    {
        array_ref_common<ali::wchar>::operator=(b);
        
        return *this;
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class c_string_const_ref : public string_const_ref
// ******************************************************************
//  Guarantees that the string is actually
//  a null-terminated C-string.
// ******************************************************************
{
public:
    c_string_const_ref( void )
    :   string_const_ref("", 0)
    {}

    //  Default copy ctor is OK.
    
    c_string_const_ref( char const* str )
    :   string_const_ref(str)
    {
        ali_assert(str != nullptr);
    }

    c_string_const_ref( char const* str, int size )
    :   string_const_ref(str, size)
    {
        ali_assert(str != nullptr);
        ali_assert(str[size] == '\0');
    }

    c_string_const_ref(
        c_string_const_ref b,
        int pos )
    :   string_const_ref(
            b.data() + pos,
            b.size() - pos)
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= b.size());
    }

    //  reference

    string_const_ref ref( int pos, int n ) const;
        //  pre:    0 <= pos && pos <= size()
        //      &&  0 <= n && n <= size() - pos
    c_string_const_ref ref( void ) const;
    c_string_const_ref ref_right( int pos ) const;
        //  pre:    0 <= pos && pos <= size()
    c_string_const_ref ref_back( int n ) const;
        //  pre:    0 <= n && n <= size()
    c_string_const_ref ref_not_front( int n ) const;
        //  pre:    0 <= n && n <= size()

    //  pointer

    array_const_ptr<char> pointer( int pos, int n ) const;
        //  pre:    0 <= pos && pos <= size()
        //      &&  0 <= n && n <= size() - pos
    c_string_const_ptr pointer( void ) const;
    c_string_const_ptr pointer_right( int pos ) const;
        //  pre:    0 <= pos && pos <= size()
    c_string_const_ptr pointer_back( int n ) const;
        //  pre:    0 <= n && n <= size()
    c_string_const_ptr pointer_not_front( int n ) const;
        //  pre:    0 <= n && n <= size()

    //  trim_left_if

    template <typename predicate>
    c_string_const_ref trim_left_if( predicate p ) const;
        //  post:   result.is_empty()
        //      ||  !predicate(result.front())

    //  trim_left

    template <typename transform>
    c_string_const_ref trim_left( string_const_ref set, transform t ) const;
        //  post:   result.is_empty()
        //      ||  !set.contains(result.front(), t)
    c_string_const_ref trim_left( void ) const;
        //  post:   result.is_empty()
        //      ||  !is_space(result.front())
    c_string_const_ref trim_left( string_const_ref set ) const;
        //  post:   result.is_empty()
        //      ||  !set.contains(result.front())
    c_string_const_ref nocase_trim_left( string_const_ref set ) const;
        //  post:   result.is_empty()
        //      ||  !set.nocase_contains(result.front())

private:    //  Methods
    c_string_const_ref& operator=(
        c_string_const_ref const& );

    static c_string_const_ref c_cast(
        string_const_ref ref );

    static c_string_const_ptr c_cast(
        array_const_ptr<char> ptr );
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class string_literal : public c_string_const_ref
// ******************************************************************
{
public:
    template <int n>
    string_literal( char const (&str)[n] )
    :   c_string_const_ref(str, n - 1 /* NULL */)
    {}
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class c_wstring_const_ref : public wstring_const_ref
// ******************************************************************
//  Guarantees that the wstring is actually
//  a null-terminated C-wstring.
// ******************************************************************
{
public:
    c_wstring_const_ref( void )
    :   wstring_const_ref(L"", 0)
    {}

    //  Default copy ctor is OK.

    c_wstring_const_ref( ali::wchar const* str )
    :   wstring_const_ref(str)
    {
        ali_assert(str != nullptr);
    }

    c_wstring_const_ref( ali::wchar const* str, int size )
    :   wstring_const_ref(str, size)
    {
        ali_assert(str != nullptr);
        ali_assert(str[size] == L'\0');
    }

    c_wstring_const_ref(
        c_wstring_const_ref b,
        int pos )
    :   wstring_const_ref(
            b.data() + pos,
            b.size() - pos)
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= b.size());
    }

    //  reference

    wstring_const_ref ref( int pos, int n ) const;
        //  pre:    0 <= pos && pos <= size()
        //      &&  0 <= n && n <= size() - pos
    c_wstring_const_ref ref( void ) const;
    c_wstring_const_ref ref_right( int pos ) const;
        //  pre:    0 <= pos && pos <= size()
    c_wstring_const_ref ref_back( int n ) const;
        //  pre:    0 <= n && n <= size()
    c_wstring_const_ref ref_not_front( int n ) const;
        //  pre:    0 <= n && n <= size()

    //  pointer

    array_const_ptr<ali::wchar> pointer( int pos, int n ) const;
        //  pre:    0 <= pos && pos <= size()
        //      &&  0 <= n && n <= size() - pos
    c_wstring_const_ptr pointer( void ) const;
    c_wstring_const_ptr pointer_right( int pos ) const;
        //  pre:    0 <= pos && pos <= size()
    c_wstring_const_ptr pointer_back( int n ) const;
        //  pre:    0 <= n && n <= size()
    c_wstring_const_ptr pointer_not_front( int n ) const;
        //  pre:    0 <= n && n <= size()

    //  trim_left_if

    template <typename predicate>
    c_wstring_const_ref trim_left_if( predicate p ) const;
        //  post:   result.is_empty()
        //      ||  !predicate(result.front())

    //  trim_left

    template <typename transform>
    c_wstring_const_ref trim_left( wstring_const_ref set, transform t ) const;
        //  post:   result.is_empty()
        //      ||  !set.contains(result.front(), t)
    c_wstring_const_ref trim_left( void ) const;
        //  post:   result.is_empty()
        //      ||  !is_space(result.front())
    c_wstring_const_ref trim_left( wstring_const_ref set ) const;
        //  post:   result.is_empty()
        //      ||  !set.contains(result.front())
    c_wstring_const_ref nocase_trim_left( wstring_const_ref set ) const;
        //  post:   result.is_empty()
        //      ||  !set.nocase_contains(result.front())

private:    //  Methods
    c_wstring_const_ref& operator=(
        c_wstring_const_ref const& );

    static c_wstring_const_ref c_cast(
        wstring_const_ref ref );

    static c_wstring_const_ptr c_cast(
        array_const_ptr<ali::wchar> ptr );
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class wstring_literal : public c_wstring_const_ref
// ******************************************************************
{
public:
    template <int n>
    wstring_literal( ali::wchar const (&str)[n] )
    :   c_wstring_const_ref(str, n - 1 /* NULL */)
    {}
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class array_ptr
// ******************************************************************
{
public:
    array_ptr( ali::nullptr_type = nullptr ) {}

    //  Default copy ctor is OK.

    array_ptr( T* begin, int size )
    :   _ref(begin, size)
    {}

    template <int n>
    array_ptr( T (&begin)[n] )
    :   _ref(begin)
    {}

    array_ptr& operator=( array_ptr const& b )
    {
        this->_ref._size = b._ref._size;
        
        this->_ref._begin = b._ref._begin;
        
        return *this;
    }

    array_ref<T> const* operator->( void ) const
    {
        return &this->_ref;
    }

    array_ref<T> const& operator*( void ) const
    {
        return this->_ref;
    }

    bool is_null( void ) const
    {
        return this->_ref._begin == nullptr;
    }

    bool operator ==( ali::nullptr_type const& ) const
    {
        return is_null();
    }

    bool operator !=( ali::nullptr_type const& ) const
    {
        return !is_null();
    }

    array_ptr& operator++( void )
    {
        ali_assert(!this->_ref.is_empty());
        
        --this->_ref._size;
        
        ++this->_ref._begin;
        
        return *this;
    }

    array_ptr operator++( int )
    {
        array_ptr const result(*this);
        
        ++(*this);
        
        return result;
    }

    array_ptr& operator+=( int n )
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_ref._size);
        
        this->_ref._size -= n;
        
        this->_ref._begin += n;
        
        return *this;
    }

    friend array_ptr operator+( array_ptr a, int n )
    {
        a += n;
        
        return a;
    }

    void swap( array_ptr& b )
    {
        using ali::swap;
        
        swap(this->_ref._begin, b._ref._begin);
        
        swap(this->_ref._size, b._ref._size);
    }

    friend void swap( array_ptr& a, array_ptr& b )
    {
        a.swap(b);
    }

private:    //  Data members
    array_ref<T>    _ref;
    
    template <typename U>
    friend class array_const_ptr;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class array_const_ptr
// ******************************************************************
{
public:     //  Methods
    array_const_ptr( ali::nullptr_type = nullptr ) {}

    //  Default copy ctor is OK.

    array_const_ptr( T const* begin, int size )
    :   _ref(begin, size)
    {}

    template <int n>
    array_const_ptr( T const (&begin)[n] )
    :   _ref(begin)
    {}

    array_const_ptr( array_ptr<T> b )
    :   _ref(b._ref)
    {}

    array_const_ptr& operator=( array_ptr<T> b )
    {
        this->_ref._size = b._ref._size;
        
        this->_ref._begin = b._ref._begin;
        
        return *this;
    }

    array_const_ptr& operator=( array_const_ptr const& b )
    {
        this->_ref._size = b._ref._size;
        
        this->_ref._begin = b._ref._begin;
        
        return *this;
    }

    array_const_ref<T> const* operator->( void ) const
    {
        return &this->_ref;
    }

    array_const_ref<T> const& operator*( void ) const
    {
        return this->_ref;
    }

    bool is_null( void ) const
    {
        return this->_ref._begin == nullptr;
    }

    bool operator ==( ali::nullptr_type const& ) const
    {
        return is_null();
    }

    bool operator !=( ali::nullptr_type const& ) const
    {
        return !is_null();
    }

    array_const_ptr& operator++( void )
    {
        ali_assert(!this->_ref.is_empty());

        --this->_ref._size;
        
        ++this->_ref._begin;
        
        return *this;
    }

    array_const_ptr operator++( int )
    {
        array_const_ptr const result(*this);
        
        ++(*this);
        
        return result;
    }

    array_const_ptr& operator+=( int n )
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_ref._size);
        
        this->_ref._size -= n;
        
        this->_ref._begin += n;
        
        return *this;
    }

    friend array_const_ptr operator+(
        array_const_ptr a, int n )
    {
        a += n;
        
        return a;
    }

    void swap( array_const_ptr& b )
    {
        using ali::swap;
        
        swap(this->_ref._begin, b._ref._begin);
        
        swap(this->_ref._size, b._ref._size);
    }

    friend void swap( array_const_ptr& a, array_const_ptr& b )
    {
        a.swap(b);
    }

private:    //  Data members
    array_const_ref<T>  _ref;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class c_string_const_ptr
// ******************************************************************
{
public:     //  Methods
    c_string_const_ptr( void )
    :   _ref("", 0)
    {}

    //  Default copy ctor is OK.

    c_string_const_ptr( char const* str )
    :   _ref(str)
    {}

    c_string_const_ptr( char const* str, int size )
    :   _ref(str, size)
    {}

    c_string_const_ptr& operator=( c_string_const_ptr const& b )
    {
        this->_ref._size = b._ref._size;
        
        this->_ref._begin = b._ref._begin;
        
        return *this;
    }

    c_string_const_ref const* operator->( void ) const
    {
        return &this->_ref;
    }

    c_string_const_ref const& operator*( void ) const
    {
        return this->_ref;
    }

    bool is_null( void ) const
    {
        return this->_ref._begin == nullptr;
    }

    bool operator ==( ali::nullptr_type const& ) const
    {
        return is_null();
    }

    bool operator !=( ali::nullptr_type const& ) const
    {
        return !is_null();
    }

    c_string_const_ptr& operator++( void )
    {
        ali_assert(!_ref.is_empty());

        --this->_ref._size;
        
        ++this->_ref._begin;
        
        return *this;
    }

    c_string_const_ptr operator++( int )
    {
        c_string_const_ptr const result(*this);
        
        ++(*this);
        
        return result;
    }

    c_string_const_ptr& operator+=( int n )
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_ref._size);

        this->_ref._size -= n;
        
        this->_ref._begin += n;
        
        return *this;
    }

    friend c_string_const_ptr operator+(
        c_string_const_ptr a, int n )
    {
        a += n;
        
        return a;
    }

    

    void swap( c_string_const_ptr& b )
    {
        using ali::swap;
        
        swap(this->_ref._begin, b._ref._begin);
        
        swap(this->_ref._size, b._ref._size);
    }

    friend void swap( c_string_const_ptr& a, c_string_const_ptr& b )
    {
        a.swap(b);
    }

private:    //  Data members
    c_string_const_ref  _ref;

    template <typename U>
    friend class array_const_ptr;
};


// ******************************************************************
// ******************************************************************

// ******************************************************************
template <>
class array_const_ptr<char>
// ******************************************************************
{
public:     //  Methods
    array_const_ptr( ali::nullptr_type = nullptr ) {}

    //  Default copy ctor is OK.

    array_const_ptr( char const* str )
    :   _ref(str)
    {}

    array_const_ptr( char const* str, int size )
    :   _ref(str, size)
    {}

    array_const_ptr( array_ptr<char> b )
    :   _ref(b._ref)
    {}

    array_const_ptr( c_string_const_ptr b )
    :   _ref(b._ref)
    {}

    array_const_ptr& operator=( array_ptr<char> b )
    {
        this->_ref._size = b._ref._size;
        
        this->_ref._begin = b._ref._begin;
        
        return *this;
    }

    array_const_ptr& operator=( c_string_const_ptr b )
    {
        this->_ref._size = b._ref._size;
        
        this->_ref._begin = b._ref._begin;
        
        return *this;
    }

    array_const_ptr& operator=( array_const_ptr const& b )
    {
        this->_ref._size = b._ref._size;
        
        this->_ref._begin = b._ref._begin;
        
        return *this;
    }

    array_const_ptr& operator=( char const* b )
    {
        return this->operator=(c_string_const_ptr(b));
    }

    array_const_ref<char> const* operator->( void ) const
    {
        return &this->_ref;
    }

    array_const_ref<char> const& operator*( void ) const
    {
        return this->_ref;
    }

    array_const_ref<char> operator|( array_const_ref<char> other ) const
    {
        return this->is_null() ? other : this->_ref;
    }

    array_const_ptr operator|( array_const_ptr other ) const
    {
        return this->is_null() ? other : *this;
    }

    bool is_null( void ) const
    {
        return this->_ref._begin == nullptr;
    }

    bool operator ==( ali::nullptr_type const& ) const
    {
        return is_null();
    }

    bool operator !=( ali::nullptr_type const& ) const
    {
        return !is_null();
    }

    array_const_ptr& operator++( void )
    {
        ali_assert(!this->_ref.is_empty());

        --this->_ref._size;
        
        ++this->_ref._begin;
        
        return *this;
    }

    array_const_ptr operator++( int )
    {
        array_const_ptr const result(*this);
        
        ++(*this);
        
        return result;
    }

    array_const_ptr& operator+=( int n )
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_ref._size);
        
        this->_ref._size -= n;
        
        this->_ref._begin += n;
        
        return *this;
    }

    friend array_const_ptr operator+(
        array_const_ptr a, int n )
    {
        a += n;
        
        return a;
    }

    void swap( array_const_ptr& b )
    {
        using ali::swap;
        
        swap(this->_ref._begin, b._ref._begin);
        
        swap(this->_ref._size, b._ref._size);
    }

    friend void swap( array_const_ptr& a, array_const_ptr& b )
    {
        a.swap(b);
    }


private:    //  Data members
    array_const_ref<char>   _ref;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class c_wstring_const_ptr
// ******************************************************************
{
public:     //  Methods
    c_wstring_const_ptr( void ) {}

    //  Default copy ctor is OK.

    c_wstring_const_ptr( ali::wchar const* str )
    :   _ref(str)
    {}

    c_wstring_const_ptr( ali::wchar const* str, int size )
    :   _ref(str, size)
    {}

    c_wstring_const_ptr& operator=( c_wstring_const_ptr const& b )
    {
        this->_ref._size = b._ref._size;
        
        this->_ref._begin = b._ref._begin;
        
        return *this;
    }

    c_wstring_const_ref const* operator->( void ) const
    {
        return &this->_ref;
    }

    c_wstring_const_ref const& operator*( void ) const
    {
        return this->_ref;
    }

    bool is_null( void ) const
    {
        return this->_ref._begin == nullptr;
    }

    bool operator ==( ali::nullptr_type const& ) const
    {
        return is_null();
    }

    bool operator !=( ali::nullptr_type const& ) const
    {
        return !is_null();
    }

    c_wstring_const_ptr& operator++( void )
    {
        ali_assert(!this->_ref.is_empty());

        --this->_ref._size;
        
        ++this->_ref._begin;
        
        return *this;
    }

    c_wstring_const_ptr operator++( int )
    {
        c_wstring_const_ptr const result(*this);
        
        ++(*this);
        
        return result;
    }

    c_wstring_const_ptr& operator+=( int n )
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_ref._size);
        
        this->_ref._size -= n;
        
        this->_ref._begin += n;
        
        return *this;
    }

    friend c_wstring_const_ptr operator+(
        c_wstring_const_ptr a, int n )
    {
        a += n;
        
        return a;
    }

    void swap( c_wstring_const_ptr& b )
    {
        using ali::swap;
        
        swap(this->_ref._begin, b._ref._begin);
        
        swap(this->_ref._size, b._ref._size);
    }

    friend void swap( c_wstring_const_ptr& a, c_wstring_const_ptr& b )
    {
        a.swap(b);
    }

private:    //  Data members
    c_wstring_const_ref  _ref;

    template <typename U>
    friend class array_const_ptr;
};


// ******************************************************************
// ******************************************************************

// ******************************************************************
template <>
class array_const_ptr<ali::wchar>
// ******************************************************************
{
public:     //  Methods
    array_const_ptr( ali::nullptr_type = nullptr ) {}

    //  Default copy ctor is OK.

    array_const_ptr( ali::wchar const* str )
    :   _ref(str)
    {}

    array_const_ptr( ali::wchar const* str, int size )
    :   _ref(str, size)
    {}

    array_const_ptr( array_ptr<ali::wchar> b )
    :   _ref(b._ref)
    {}

    array_const_ptr( c_wstring_const_ptr b )
    :   _ref(b._ref)
    {}

    array_const_ptr& operator=( array_ptr<ali::wchar> b )
    {
        this->_ref._size = b._ref._size;
        
        this->_ref._begin = b._ref._begin;
        
        return *this;
    }

    array_const_ptr& operator=( c_wstring_const_ptr b )
    {
        this->_ref._size = b._ref._size;
        
        this->_ref._begin = b._ref._begin;
        
        return *this;
    }

    array_const_ptr& operator=( array_const_ptr const& b )
    {
        this->_ref._size = b._ref._size;
        
        this->_ref._begin = b._ref._begin;
        
        return *this;
    }

    array_const_ptr& operator=( ali::wchar const* b )
    {
        return this->operator=(c_wstring_const_ptr(b));
    }

    array_const_ref<ali::wchar> const* operator->( void ) const
    {
        return &this->_ref;
    }

    array_const_ref<ali::wchar> const& operator*( void ) const
    {
        return this->_ref;
    }

    array_const_ref<ali::wchar> operator|( array_const_ref<ali::wchar> other ) const
    {
        return this->is_null() ? other : this->_ref;
    }

    array_const_ptr operator|( array_const_ptr other ) const
    {
        return this->is_null() ? other : *this;
    }

    bool is_null( void ) const
    {
        return this->_ref._begin == nullptr;
    }

    bool operator ==( ali::nullptr_type const& ) const
    {
        return is_null();
    }

    bool operator !=( ali::nullptr_type const& ) const
    {
        return !is_null();
    }

    array_const_ptr& operator++( void )
    {
        ali_assert(!this->_ref.is_empty());

        --this->_ref._size;
        
        ++this->_ref._begin;
        
        return *this;
    }

    array_const_ptr operator++( int )
    {
        array_const_ptr const result(*this);
        
        ++(*this);
        
        return result;
    }

    array_const_ptr& operator+=( int n )
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_ref._size);
        
        this->_ref._size -= n;
        
        this->_ref._begin += n;
        
        return *this;
    }

    friend array_const_ptr operator+(
        array_const_ptr a, int n )
    {
        a += n;
        
        return a;
    }

    void swap( array_const_ptr& b )
    {
        using ali::swap;
        
        swap(this->_ref._begin, b._ref._begin);
        
        swap(this->_ref._size, b._ref._size);
    }

    friend void swap( array_const_ptr& a, array_const_ptr& b )
    {
        a.swap(b);
    }

private:    //  Data members
    array_const_ref<ali::wchar> _ref;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
//  INLINES
// ******************************************************************

// ******************************************************************
template <typename T>
inline array_const_ref<T>
    array_const_ref_common<T>::ref( int pos, int n ) const
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
//      &&  0 <= n && n <= size() - pos
// ******************************************************************
{
    ali_assert(0 <= pos);
    ali_assert(pos <= this->_size);
    ali_assert(0 <= n);
    ali_assert(n <= this->_size - pos);

    return array_const_ref<T>(this->_begin + pos, n);
}

// ******************************************************************
template <typename T>
inline array_const_ref<T>
    array_const_ref_common<T>::ref( void ) const
// ******************************************************************
{
    return array_const_ref<T>(this->_begin, this->_size);
}

// ******************************************************************
template <typename T>
inline array_const_ref<T>
    array_const_ref_common<T>::ref_left( int pos ) const
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
// ******************************************************************
{
    ali_assert(0 <= pos);
    ali_assert(pos <= this->_size);

    return array_const_ref<T>(this->_begin, pos);
}

// ******************************************************************
template <typename T>
inline array_const_ref<T>
    array_const_ref_common<T>::ref_right( int pos ) const
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
// ******************************************************************
{
    ali_assert(0 <= pos);
    ali_assert(pos <= this->_size);

    return array_const_ref<T>(this->_begin + pos, this->_size - pos);
}

// ******************************************************************
template <typename T>
inline array_const_ref<T>
    array_const_ref_common<T>::ref_front( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    ali_assert(0 <= n);
    ali_assert(n <= this->_size);

    return array_const_ref<T>(this->_begin, n);
}

// ******************************************************************
template <typename T>
inline array_const_ref<T>
    array_const_ref_common<T>::ref_back( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    ali_assert(0 <= n);
    ali_assert(n <= this->_size);

    return array_const_ref<T>(this->_begin + (this->_size - n), n);
}

// ******************************************************************
template <typename T>
inline array_const_ref<T>
    array_const_ref_common<T>::ref_not_front( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    ali_assert(0 <= n);
    ali_assert(n <= this->_size);

    return array_const_ref<T>(this->_begin + n, this->_size - n);
}

// ******************************************************************
template <typename T>
inline array_const_ref<T>
    array_const_ref_common<T>::ref_not_back( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    ali_assert(0 <= n);
    ali_assert(n <= this->_size);

    return array_const_ref<T>(this->_begin, this->_size - n);
}

//  pointer

// ******************************************************************
template <typename T>
inline array_const_ptr<T>
    array_const_ref_common<T>::pointer( int pos, int n ) const
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
//      &&  0 <= n && n <= size() - pos
// ******************************************************************
{
    return this->ref(pos, n).pointer();
}

// ******************************************************************
template <typename T>
inline array_const_ptr<T>
    array_const_ref_common<T>::pointer( void ) const
// ******************************************************************
{
    return array_const_ptr<T>(this->_begin, this->_size);
}

// ******************************************************************
template <typename T>
inline array_const_ptr<T>
    array_const_ref_common<T>::pointer_left( int pos ) const
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
// ******************************************************************
{
    return this->ref_left(pos).pointer();
}

// ******************************************************************
template <typename T>
inline array_const_ptr<T>
    array_const_ref_common<T>::pointer_right( int pos ) const
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
// ******************************************************************
{
    return this->ref_right(pos).pointer();
}

// ******************************************************************
template <typename T>
inline array_const_ptr<T>
    array_const_ref_common<T>::pointer_front( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    return this->ref_front(n).pointer();
}

// ******************************************************************
template <typename T>
inline array_const_ptr<T>
    array_const_ref_common<T>::pointer_back( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    return this->ref_back(n).pointer();
}

// ******************************************************************
template <typename T>
inline array_const_ptr<T>
    array_const_ref_common<T>::pointer_not_front( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    return this->ref_not_front(n).pointer();
}

// ******************************************************************
template <typename T>
inline array_const_ptr<T>
    array_const_ref_common<T>::pointer_not_back( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    return this->ref_not_back(n).pointer();
}

// ******************************************************************
template <typename T>
template <typename body>
inline array_const_ref<T>
    array_const_ref_common<T>::for_each( body b ) const
// ******************************************************************
{
    for ( int i = this->_size; i != 0; )
        b(this->at(--i));
        
    return this->ref();
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
template <typename U, typename transform>
inline bool array_const_ref_common<T>::begins_with_n(
    array_const_ref<U> b, transform t ) const
// ******************************************************************
{
    return  this->_size >= b._size
        &&  ref_front(b._size).is_equal_to(b, t);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
template <typename U, typename transform>
inline bool array_const_ref_common<T>::ends_with_n(
    array_const_ref<U> b, transform t ) const
// ******************************************************************
{
    return  this->_size >= b._size
        &&  ref_back(b._size).is_equal_to(b, t);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
template <typename predicate>
inline array_const_ref<T> array_const_ref_common<T>::trim_left_if(
    predicate p ) const
// ******************************************************************
{
    return this->ref_right(this->index_of_first_if_not(p));
}

// ******************************************************************
template <typename T>
template <typename U, typename transform>
inline array_const_ref<T> array_const_ref_common<T>::trim_left(
    array_const_ref<U> set, transform t ) const
// ******************************************************************
{
    return this->ref_right(this->index_of_first_not_of(set, t));
}

// ******************************************************************
template <typename T>
template <typename transform>
inline array_const_ref<T> array_const_ref_common<T>::trim_left(
    array_const_ref<T> set, transform t ) const
// ******************************************************************
{
    return this->trim_left<T, transform>(set, t);
}

// ******************************************************************
template <typename T>
template <typename U>
inline array_const_ref<T> array_const_ref_common<T>::trim_left(
    array_const_ref<U> set ) const
// ******************************************************************
{
    return this->trim_left(set, ali::identity);
}

// ******************************************************************
template <typename T>
inline array_const_ref<T> array_const_ref_common<T>::trim_left(
    array_const_ref<T> set ) const
// ******************************************************************
{
    return this->trim_left<T>(set);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
template <typename predicate>
array_const_ref<T> array_const_ref_common<T>::trim_right_if(
    predicate p ) const
// ******************************************************************
{
    int trimmed_size = this->_size;
    
    while ( trimmed_size != 0 && p(this->at(trimmed_size - 1)) )
        --trimmed_size;
    
    return this->ref_front(trimmed_size);
}

// ******************************************************************
template <typename T>
template <typename U, typename transform>
array_const_ref<T> array_const_ref_common<T>::trim_right(
    array_const_ref<U> set, transform t ) const
// ******************************************************************
{
    int trimmed_size = this->_size;
    
    if ( !set.is_empty() )
        while ( trimmed_size != 0 && set.contains(this->at(trimmed_size - 1), t) )
            --trimmed_size;
    
    return this->ref_front(trimmed_size);
}

// ******************************************************************
template <typename T>
template <typename transform>
inline array_const_ref<T> array_const_ref_common<T>::trim_right(
    array_const_ref<T> set, transform t ) const
// ******************************************************************
{
    return this->trim_right<T, transform>(set, t);
}

// ******************************************************************
template <typename T>
template <typename U>
inline array_const_ref<T> array_const_ref_common<T>::trim_right(
    array_const_ref<U> set ) const
// ******************************************************************
{
    return this->trim_right(set, ali::identity);
}

// ******************************************************************
template <typename T>
inline array_const_ref<T> array_const_ref_common<T>::trim_right(
    array_const_ref<T> set ) const
// ******************************************************************
{
    return this->trim_right<T>(set);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
template <typename predicate>
inline array_const_ref<T> array_const_ref_common<T>::trim_if(
    predicate p ) const
// ******************************************************************
{
    return this->trim_left_if(p).trim_right_if(p);
}

// ******************************************************************
template <typename T>
template <typename U, typename transform>
inline array_const_ref<T> array_const_ref_common<T>::trim(
    array_const_ref<U> set, transform t ) const
// ******************************************************************
{
    return this->trim_left(set, t).trim_right(set, t);
}

// ******************************************************************
template <typename T>
template <typename transform>
inline array_const_ref<T> array_const_ref_common<T>::trim(
    array_const_ref<T> set, transform t ) const
// ******************************************************************
{
    return this->trim<T, transform>(set, t);
}

// ******************************************************************
template <typename T>
template <typename U>
inline array_const_ref<T> array_const_ref_common<T>::trim(
    array_const_ref<U> set ) const
// ******************************************************************
{
    return this->trim(set, ali::identity);
}

// ******************************************************************
template <typename T>
inline array_const_ref<T> array_const_ref_common<T>::trim(
    array_const_ref<T> set ) const
// ******************************************************************
{
    return this->trim<T>(set);
}

// ******************************************************************
template <typename T>
template <typename U, typename transform>
int array_const_ref_common<T>::count_n(
    array_const_ref<U> b, transform t ) const
// ******************************************************************
//  Returns the number of non-overlapping
//  occurrences of b inside this array.
// ******************************************************************
{
    int n = 0;

	array_const_ptr<T> ptr = this->pointer();

	while ( !(ptr += ptr->index_of_first_n(b, t))->is_empty() )
		++n, ptr += b.size();

	return n;
}

// ******************************************************************
template <typename T>
inline array_const_ref<ali::uint8>
    array_const_ref_common<T>::as_blob( void ) const
// ******************************************************************
{
    return array_const_ref<ali::uint8>(
        reinterpret_cast<ali::uint8 const*>(this->_begin),
        this->_size * sizeof(T));
}

// ******************************************************************
template <>
inline array_const_ref<ali::uint8>
    array_const_ref_common<ali::uint8>::as_blob( void ) const
// ******************************************************************
{
    return this->ref();
}

// ******************************************************************
template <typename T>
inline is_internal_pointer_result array_const_ref_common<T>
    ::is_internal_pointer( array_const_ref<T> a ) const
// ******************************************************************
{
    return this->is_internal_pointer(a.data());
}

// ******************************************************************
template <typename T>
inline is_internal_pointer_result array_const_ref_common<T>
    ::is_internal_pointer( array_const_ptr<T> a ) const
// ******************************************************************
{
    return this->is_internal_pointer(a->data());
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <>
ali::uint32 array_const_ref<ali::uint8>
    ::int_be_at_<ali::uint32>( int pos, int byte_count ) const;
// ******************************************************************
// pre: byte_count in set {1, 2, 3, 4}
// ******************************************************************

// ******************************************************************
template <>
ali::uint32 array_const_ref<ali::uint8>
    ::int_le_at_<ali::uint32>( int pos, int byte_count ) const;
// ******************************************************************
// pre: byte_count in set {1, 2, 3, 4}
// ******************************************************************

// ******************************************************************
template <>
ali::uint64 array_const_ref<ali::uint8>
    ::int_be_at_<ali::uint64>( int pos, int byte_count ) const;
// ******************************************************************
// pre: byte_count in set {1, 2, 3, 4, 5, 6, 7, 8}
// ******************************************************************

// ******************************************************************
template <>
ali::uint64 array_const_ref<ali::uint8>
    ::int_le_at_<ali::uint64>( int pos, int byte_count ) const;
// ******************************************************************
// pre: byte_count in set {1, 2, 3, 4, 5, 6, 7, 8}
// ******************************************************************

// ******************************************************************
inline ali::uint32 array_const_ref<ali::uint8>::int_be_at(
    int pos, int byte_count ) const
// ******************************************************************
// pre: byte_count in set {1, 2, 3, 4}
// ******************************************************************
{
    return this->int_be_at_<ali::uint32>(pos, byte_count);
}

// ******************************************************************
inline ali::uint32 array_const_ref<ali::uint8>::int_le_at(
    int pos, int byte_count ) const
// ******************************************************************
// pre: byte_count in set {1, 2, 3, 4}
// ******************************************************************
{
    return this->int_le_at_<ali::uint32>(pos, byte_count);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
inline array_const_ref<char>
    array_const_ref<ali::uint8>::as_string( void ) const
// ******************************************************************
{
    return array_const_ref<char>(
        reinterpret_cast<char const*>(_begin), _size);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
inline array_const_ref<char> array_const_ref<char>
    ::trim_left( void ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  !is_space(result.front())
// ******************************************************************
{
    return this->trim_left_if(ali::is_space);
}

// ******************************************************************
inline array_const_ref<char> array_const_ref<char>
    ::nocase_trim_left( array_const_ref set ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  !charset.nocase_contains(result.front())
// ******************************************************************
{
    return this->trim_left(set, ali::to_lower);
}

// ******************************************************************
inline array_const_ref<char> array_const_ref<char>
    ::trim_right( void ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  !is_space(result.back())
// ******************************************************************
{
    return this->trim_right_if(ali::is_space);
}

// ******************************************************************
inline array_const_ref<char> array_const_ref<char>
    ::nocase_trim_right( array_const_ref set ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  !set.nocase_contains(result.back())
// ******************************************************************
{
    return this->trim_right(set, ali::to_lower);
}

// ******************************************************************
inline array_const_ref<char> array_const_ref<char>::trim( void ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  (   !is_space(result.front())
//          &&  !is_space(result.back()))
// ******************************************************************
{
    return this->trim_if(ali::is_space);
}

// ******************************************************************
inline array_const_ref<char> array_const_ref<char>::nocase_trim(
    array_const_ref set ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  (   !set.nocase_contains(result.front())
//          &&  !set.nocase_contains(result.back()))
// ******************************************************************
{
    return this->trim(set, ali::to_lower);
}

// ******************************************************************
inline is_internal_pointer_result array_const_ref<char>
    ::is_internal_pointer( c_string_const_ptr a ) const
// ******************************************************************
{
    return this->is_internal_pointer(a->data());
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
inline array_const_ref<ali::wchar> array_const_ref<ali::wchar>
    ::trim_left( void ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  !is_space(result.front())
// ******************************************************************
{
    return this->trim_left_if(ali::is_space);
}

// ******************************************************************
inline array_const_ref<ali::wchar> array_const_ref<ali::wchar>
    ::nocase_trim_left( array_const_ref set ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  !charset.nocase_contains(result.front())
// ******************************************************************
{
    return this->trim_left(set, ali::to_lower);
}

// ******************************************************************
inline array_const_ref<ali::wchar> array_const_ref<ali::wchar>
    ::trim_right( void ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  !is_space(result.back())
// ******************************************************************
{
    return this->trim_right_if(ali::is_space);
}

// ******************************************************************
inline array_const_ref<ali::wchar> array_const_ref<ali::wchar>
    ::nocase_trim_right( array_const_ref set ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  !set.nocase_contains(result.back())
// ******************************************************************
{
    return this->trim_right(set, ali::to_lower);
}

// ******************************************************************
inline array_const_ref<ali::wchar> array_const_ref<ali::wchar>
    ::trim( void ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  (   !is_space(result.front())
//          &&  !is_space(result.back()))
// ******************************************************************
{
    return this->trim_if(ali::is_space);
}

// ******************************************************************
inline array_const_ref<ali::wchar> array_const_ref<ali::wchar>
    ::nocase_trim( array_const_ref set ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  (   !set.nocase_contains(result.front())
//          &&  !set.nocase_contains(result.back()))
// ******************************************************************
{
    return this->trim(set, ali::to_lower);
}

// ******************************************************************
inline is_internal_pointer_result array_const_ref<ali::wchar>
    ::is_internal_pointer( c_wstring_const_ptr a ) const
// ******************************************************************
{
    return this->is_internal_pointer(a->data());
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline array_ref<T> array_ref_common<T>
    ::mutable_ref( int pos, int n ) const
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
//      &&  0 <= n && n <= size() - pos
// ******************************************************************
{
    ali_assert(0 <= pos);
    ali_assert(pos <= this->_size);
    ali_assert(0 <= n);
    ali_assert(n <= this->_size - pos);

    return array_ref<T>(this->data() + pos, n);
}

// ******************************************************************
template <typename T>
inline array_ref<T> array_ref_common<T>::mutable_ref( void ) const
// ******************************************************************
{
    return array_ref<T>(this->data(), this->_size);
}

// ******************************************************************
template <typename T>
inline array_ref<T>
    array_ref_common<T>::mutable_ref_left( int pos ) const
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
// ******************************************************************
{
    ali_assert(0 <= pos);
    ali_assert(pos <= this->_size);

    return array_ref<T>(this->data(), pos);
}

// ******************************************************************
template <typename T>
inline array_ref<T>
    array_ref_common<T>::mutable_ref_right( int pos ) const
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
// ******************************************************************
{
    ali_assert(0 <= pos);
    ali_assert(pos <= this->_size);

    return array_ref<T>(this->data() + pos, this->_size - pos);
}

// ******************************************************************
template <typename T>
inline array_ref<T>
    array_ref_common<T>::mutable_ref_front( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    ali_assert(0 <= n);
    ali_assert(n <= this->_size);

    return array_ref<T>(this->data(), n);
}

// ******************************************************************
template <typename T>
inline array_ref<T>
    array_ref_common<T>::mutable_ref_back( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    ali_assert(0 <= n);
    ali_assert(n <= this->_size);

    return array_ref<T>(this->data() + (this->_size - n), n);
}

// ******************************************************************
template <typename T>
inline array_ref<T>
    array_ref_common<T>::mutable_ref_not_front( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    ali_assert(0 <= n);
    ali_assert(n <= this->_size);

    return array_ref<T>(this->data() + n, this->_size - n);
}

// ******************************************************************
template <typename T>
inline array_ref<T>
    array_ref_common<T>::mutable_ref_not_back( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    ali_assert(0 <= n);
    ali_assert(n <= this->_size);

    return array_ref<T>(this->data(), this->_size - n);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline array_ptr<T>
    array_ref_common<T>::mutable_pointer( int pos, int n ) const
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
//      &&  0 <= n && n <= size() - pos
// ******************************************************************
{
    return this->mutable_ref(pos, n).mutable_pointer();
}

// ******************************************************************
template <typename T>
inline array_ptr<T>
    array_ref_common<T>::mutable_pointer( void ) const
// ******************************************************************
{
    return array_ptr<T>(this->data(), this->_size);
}

// ******************************************************************
template <typename T>
inline array_ptr<T>
    array_ref_common<T>::mutable_pointer_left( int pos ) const
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
// ******************************************************************
{
    return this->mutable_ref_left(pos).mutable_pointer();
}

// ******************************************************************
template <typename T>
inline array_ptr<T>
    array_ref_common<T>::mutable_pointer_right( int pos ) const
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
// ******************************************************************
{
    return this->mutable_ref_right(pos).mutable_pointer();
}

// ******************************************************************
template <typename T>
inline array_ptr<T>
    array_ref_common<T>::mutable_pointer_front( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    return this->mutable_ref_front(n).mutable_pointer();
}

// ******************************************************************
template <typename T>
inline array_ptr<T>
    array_ref_common<T>::mutable_pointer_back( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    return this->mutable_ref_back(n).mutable_pointer();
}

// ******************************************************************
template <typename T>
inline array_ptr<T>
    array_ref_common<T>::mutable_pointer_not_front( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    return this->mutable_ref_not_front(n).mutable_pointer();
}

// ******************************************************************
template <typename T>
inline array_ptr<T>
    array_ref_common<T>::mutable_pointer_not_back( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    return this->mutable_ref_not_back(n).mutable_pointer();
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
template <typename U>
inline array_ref<T> const& array_ref_common<T>::copy(
    int pos, array_const_ref<U> b ) const
// ******************************************************************
{
    this->mutable_ref_right(pos).copy_front(b);

    return this->self();
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
template <typename U>
inline array_ref<T> const& array_ref_common<T>::move(
    int pos, array_ref<U> b ) const
// ******************************************************************
{
    this->mutable_ref_right(pos).move_front(b);

    return this->self();
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
template <typename U>
inline array_ref<T> const& array_ref_common<T>::move_if_noexcept(
    int pos, array_ref<U> b ) const
// ******************************************************************
{
    this->mutable_ref_right(pos).move_front_if_noexcept(b);

    return this->self();
}

// ******************************************************************
template <typename T>
template <typename operation>
inline array_ref<T> const& array_ref_common<T>::combine(
    int pos, array_const_ref<T> b, operation op ) const
// ******************************************************************
{
    this->mutable_ref_right(pos).combine_front(b, op);

    return this->self();
}

// ******************************************************************
template <typename T>
template <typename operation>
inline array_ref<T> const& array_ref_common<T>::combine_back(
    array_const_ref<T> b, operation op ) const
// ******************************************************************
{
    int const n = ali::mini(this->_size, b._size);

    this->mutable_ref_back(n)
        .combine_front(
            b.ref_back(n), op);

    return this->self();
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline array_ref<T> const& array_ref_common<T>::swap_block(
    int pos, array_ref<T> b ) const
// ******************************************************************
{
    this->mutable_ref_right(pos).swap_block_front(b);

    return this->self();
}

// ******************************************************************
template <typename T>
inline array_ref<T> const& array_ref_common<T>::swap_block_back(
    array_ref<T> b ) const
// ******************************************************************
{
    int const n = ali::mini(this->_size, b._size);

    this->mutable_ref_back(n)
        .swap_block_front(
            b.mutable_ref_back(n));

    return this->self();
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
template <typename comparator>
array_ref<T> const& array_ref_common<T>::quick_sort(
    comparator compare ) const
// ******************************************************************
{
    int const threshold = 8;

    array_ptr<T> ptr = this->mutable_pointer();

    while ( ptr->size() > threshold )
    {
        int const i = ptr->unstable_partition_around_pivot(
            ptr->size() / 2, compare);

        array_ptr<T>
            left = ptr->mutable_pointer_left(i),
            right = ptr->mutable_pointer_right(i + 1);

        //  Recurse to the smaller partition.

        if ( left->size() < right->size() )
        {
            left->quick_sort(compare);
            
            ptr = right;
        }
        else
        {
            right->quick_sort(compare);

            ptr = left;
        }
    }

    ptr->insertion_sort(compare);

    return this->self();
}

// ******************************************************************
template <typename T>
template <typename comparator>
array_ref<T> const& array_ref_common<T>::heap_sort(
    comparator compare ) const
// ******************************************************************
{
    if ( this->_size < 2 )
        ;   //  NOOP
    else
    {
        using ali::swap;

        this->make_heap(compare);

        int right = this->_size;

        swap(this->front(), this->at(--right));
        
        while ( right > 1 )
        {
            this->mutable_ref_left(right)
                .down_heap(0, compare);
            
            swap(this->front(), this->at(--right));
        }
    }

    return this->self();
}

// ******************************************************************
template <typename T>
inline array_ref<ali::uint8>
    array_ref_common<T>::as_mutable_blob( void ) const
// ******************************************************************
{
    return array_ref<ali::uint8>(
        reinterpret_cast<ali::uint8*>(this->data()),
        this->_size * sizeof(T));
}

// ******************************************************************
template <>
inline array_ref<ali::uint8>
    array_ref_common<ali::uint8>::as_mutable_blob( void ) const
// ******************************************************************
{
    return this->mutable_ref();
}


// ******************************************************************
// ******************************************************************

// ******************************************************************
inline string_const_ref c_string_const_ref
    ::ref( int pos, int n ) const
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
//      &&  0 <= n && n <= size() - pos
// ******************************************************************
{
    return string_const_ref::ref(pos, n);
}

// ******************************************************************
inline c_string_const_ref c_string_const_ref::ref( void ) const
// ******************************************************************
{
    return c_cast(string_const_ref::ref());
}

// ******************************************************************
inline c_string_const_ref c_string_const_ref
    ::ref_right( int pos ) const
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
// ******************************************************************
{
    return c_cast(string_const_ref::ref_right(pos));
}

// ******************************************************************
inline c_string_const_ref c_string_const_ref
    ::ref_back( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    return c_cast(string_const_ref::ref_back(n));
}

// ******************************************************************
inline c_string_const_ref c_string_const_ref
    ::ref_not_front( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    return c_cast(string_const_ref::ref_not_front(n));
}

// ******************************************************************
inline array_const_ptr<char> c_string_const_ref
    ::pointer( int pos, int n ) const
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
//      &&  0 <= n && n <= size() - pos
// ******************************************************************
{
    return string_const_ref::pointer(pos, n);
}

// ******************************************************************
inline c_string_const_ptr c_string_const_ref::pointer( void ) const
// ******************************************************************
{
    return c_cast(string_const_ref::pointer());
}

// ******************************************************************
inline c_string_const_ptr c_string_const_ref
    ::pointer_right( int pos ) const
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
// ******************************************************************
{
    return c_cast(string_const_ref::pointer_right(pos));
}

// ******************************************************************
inline c_string_const_ptr c_string_const_ref
    ::pointer_back( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    return c_cast(string_const_ref::pointer_back(n));
}

// ******************************************************************
inline c_string_const_ptr c_string_const_ref
    ::pointer_not_front( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    return c_cast(string_const_ref::pointer_not_front(n));
}

// ******************************************************************
template <typename predicate>
inline c_string_const_ref c_string_const_ref
    ::trim_left_if( predicate p ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  !predicate(result.front())
// ******************************************************************
{
    return c_cast(string_const_ref::trim_left_if(p));
}

// ******************************************************************
template <typename transform>
inline c_string_const_ref c_string_const_ref::trim_left(
    string_const_ref set, transform t ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  !set.contains(result.front(), t)
// ******************************************************************
{
    return c_cast(string_const_ref::trim_left(set, t));
}

// ******************************************************************
inline c_string_const_ref c_string_const_ref
    ::trim_left( void ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  !is_space(result.front())
// ******************************************************************
{
    return c_cast(string_const_ref::trim_left());
}

// ******************************************************************
inline c_string_const_ref c_string_const_ref::trim_left(
    string_const_ref set ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  !set.contains(result.front())
// ******************************************************************
{
    return c_cast(string_const_ref::trim_left(set));
}

// ******************************************************************
inline c_string_const_ref c_string_const_ref::nocase_trim_left(
    string_const_ref set ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  !set.nocase_contains(result.front())
// ******************************************************************
{
    return c_cast(string_const_ref::nocase_trim_left(set));
}

// ******************************************************************
inline c_string_const_ref c_string_const_ref::c_cast(
    string_const_ref ref )
// ******************************************************************
{
    return c_string_const_ref(ref._begin, ref._size);
}

// ******************************************************************
inline c_string_const_ptr c_string_const_ref::c_cast(
    array_const_ptr<char> ptr )
// ******************************************************************
{
    return c_string_const_ptr(ptr->_begin, ptr->_size);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
inline wstring_const_ref c_wstring_const_ref
    ::ref( int pos, int n ) const
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
//      &&  0 <= n && n <= size() - pos
// ******************************************************************
{
    return wstring_const_ref::ref(pos, n);
}

// ******************************************************************
inline c_wstring_const_ref c_wstring_const_ref::ref( void ) const
// ******************************************************************
{
    return c_cast(wstring_const_ref::ref());
}

// ******************************************************************
inline c_wstring_const_ref c_wstring_const_ref
    ::ref_right( int pos ) const
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
// ******************************************************************
{
    return c_cast(wstring_const_ref::ref_right(pos));
}

// ******************************************************************
inline c_wstring_const_ref c_wstring_const_ref
    ::ref_back( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    return c_cast(wstring_const_ref::ref_back(n));
}

// ******************************************************************
inline c_wstring_const_ref c_wstring_const_ref
    ::ref_not_front( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    return c_cast(wstring_const_ref::ref_not_front(n));
}

// ******************************************************************
inline array_const_ptr<ali::wchar> c_wstring_const_ref
    ::pointer( int pos, int n ) const
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
//      &&  0 <= n && n <= size() - pos
// ******************************************************************
{
    return wstring_const_ref::pointer(pos, n);
}

// ******************************************************************
inline c_wstring_const_ptr c_wstring_const_ref::pointer( void ) const
// ******************************************************************
{
    return c_cast(wstring_const_ref::pointer());
}

// ******************************************************************
inline c_wstring_const_ptr c_wstring_const_ref
    ::pointer_right( int pos ) const
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
// ******************************************************************
{
    return c_cast(wstring_const_ref::pointer_right(pos));
}

// ******************************************************************
inline c_wstring_const_ptr c_wstring_const_ref
    ::pointer_back( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    return c_cast(wstring_const_ref::pointer_back(n));
}

// ******************************************************************
inline c_wstring_const_ptr c_wstring_const_ref
    ::pointer_not_front( int n ) const
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    return c_cast(wstring_const_ref::pointer_not_front(n));
}

// ******************************************************************
template <typename predicate>
inline c_wstring_const_ref c_wstring_const_ref
    ::trim_left_if( predicate p ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  !predicate(result.front())
// ******************************************************************
{
    return c_cast(wstring_const_ref::trim_left_if(p));
}

// ******************************************************************
template <typename transform>
inline c_wstring_const_ref c_wstring_const_ref::trim_left(
    wstring_const_ref set, transform t ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  !set.contains(result.front(), t)
// ******************************************************************
{
    return c_cast(wstring_const_ref::trim_left(set, t));
}

// ******************************************************************
inline c_wstring_const_ref c_wstring_const_ref
    ::trim_left( void ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  !is_space(result.front())
// ******************************************************************
{
    return c_cast(wstring_const_ref::trim_left());
}

// ******************************************************************
inline c_wstring_const_ref c_wstring_const_ref::trim_left(
    wstring_const_ref set ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  !set.contains(result.front())
// ******************************************************************
{
    return c_cast(wstring_const_ref::trim_left(set));
}

// ******************************************************************
inline c_wstring_const_ref c_wstring_const_ref::nocase_trim_left(
    wstring_const_ref set ) const
// ******************************************************************
//  post:   result.is_empty()
//      ||  !set.nocase_contains(result.front())
// ******************************************************************
{
    return c_cast(wstring_const_ref::nocase_trim_left(set));
}

// ******************************************************************
inline c_wstring_const_ref c_wstring_const_ref::c_cast(
    wstring_const_ref ref )
// ******************************************************************
{
    return c_wstring_const_ref(ref._begin, ref._size);
}

// ******************************************************************
inline c_wstring_const_ptr c_wstring_const_ref::c_cast(
    array_const_ptr<ali::wchar> ptr )
// ******************************************************************
{
    return c_wstring_const_ptr(ptr->_begin, ptr->_size);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
inline array_ref<char>
    array_ref<ali::uint8>::as_mutable_string( void ) const
// ******************************************************************
{
    return array_ref<char>(
        reinterpret_cast<char*>(
            this->data()), this->_size);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline int operator-(
    array_const_ptr<T> a,
    array_const_ptr<T> b )
// ******************************************************************
//  pre: b->is_internal_pointer(a).distance_or_negative >= 0
// ******************************************************************
{
    auto const distance = b->is_internal_pointer(
        a).distance_or_negative;

    ali_assert(distance >= 0);

    return distance;
}

// ******************************************************************
template <typename T>
inline bool operator==(
    array_const_ref<T> a,
    array_const_ref<T> b )
// ******************************************************************
{
    return a.is_equal_to(b);
}

// ******************************************************************
template <typename T>
inline bool operator!=(
    array_const_ref<T> a,
    array_const_ref<T> b )
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
template <typename T>
inline int compare(
    array_const_ref<T> a,
    array_const_ref<T> b )
// ******************************************************************
{
    return a.compare(b);
}

// ******************************************************************
template <typename T>
inline bool operator<(
    array_const_ref<T> a,
    array_const_ref<T> b )
// ******************************************************************
{
    return compare(a, b) < 0;
}

// ******************************************************************
template <typename T>
inline bool operator<=(
    array_const_ref<T> a,
    array_const_ref<T> b )
// ******************************************************************
{
    return compare(a, b) <= 0;
}

// ******************************************************************
template <typename T>
inline bool operator>(
    array_const_ref<T> a,
    array_const_ref<T> b )
// ******************************************************************
{
    return compare(a, b) > 0;
}

// ******************************************************************
template <typename T>
inline bool operator>=(
    array_const_ref<T> a,
    array_const_ref<T> b )
// ******************************************************************
{
    return compare(a, b) >= 0;
}

// ******************************************************************
// ******************************************************************

#define ALI_ARRAY_CONST_REF_EXPLICIT_OPERATORS(T)   \
inline int operator-(                               \
    array_const_ptr<T> a, array_const_ptr<T> b )    \
    { return operator-<T>(a, b); }                  \
inline bool operator==(                             \
    array_const_ref<T> a, array_const_ref<T> b )    \
    { return operator==<T>(a, b); }                 \
inline bool operator!=(                             \
    array_const_ref<T> a, array_const_ref<T> b )    \
    { return operator!=<T>(a, b); }                 \
inline int compare(                                 \
    array_const_ref<T> a, array_const_ref<T> b )    \
    { return compare<T>(a, b); }                    \
inline bool operator<(                              \
    array_const_ref<T> a, array_const_ref<T> b )    \
    { return operator< <T>(a, b); }                 \
inline bool operator<=(                             \
    array_const_ref<T> a, array_const_ref<T> b )    \
    { return operator<=<T>(a, b); }                 \
inline bool operator>(                              \
    array_const_ref<T> a, array_const_ref<T> b )    \
    { return operator> <T>(a, b); }                 \
inline bool operator>=(                             \
    array_const_ref<T> a, array_const_ref<T> b )    \
    { return operator>=<T>(a, b); }


ALI_ARRAY_CONST_REF_EXPLICIT_OPERATORS(char)
ALI_ARRAY_CONST_REF_EXPLICIT_OPERATORS(ali::wchar)
ALI_ARRAY_CONST_REF_EXPLICIT_OPERATORS(ali::uint8)
    //  We need these to enable implicit conversions
    //  from ali::string, string literal and ali::blob.

#undef  ALI_ARRAY_CONST_REF_EXPLICIT_OPERATORS

// ******************************************************************
template <typename T>
inline array_const_ref<T> make_ref( T const* arr, int size )
// ******************************************************************
{
    return array_const_ref<T>{arr, size};
}

// ******************************************************************
template <typename T, int size>
inline array_const_ref<T> make_ref( T const (&arr)[size] )
// ******************************************************************
{
    return array_const_ref<T>{arr, size};
}

// ******************************************************************
template <typename T>
inline array_const_ref<T> make_ref( ali::initializer_list<T> arr )
// ******************************************************************
{
    return array_const_ref<T>{arr.begin(), static_cast<int>(arr.size())};
}

// ******************************************************************
template <typename T>
inline array_ref<T> make_mutable_ref( T* arr, int size )
// ******************************************************************
{
    return array_ref<T>{arr, size};
}

// ******************************************************************
template <typename T, int size>
inline array_ref<T> make_mutable_ref( T (&arr)[size] )
// ******************************************************************
{
    return array_ref<T>{arr, size};
}

// ******************************************************************
inline c_string_const_ref operator"" _s(
    char const* str, size_t size )
// ******************************************************************
{
    return c_string_const_ref{str, static_cast<int>(size)};
}

// ******************************************************************
inline c_wstring_const_ref operator"" _s(
    ali::wchar const* str, size_t size )
// ******************************************************************
{
    return c_wstring_const_ref{str, static_cast<int>(size)};
}

// ******************************************************************
inline c_string_const_ptr operator"" _sp(
    char const* str, size_t size )
// ******************************************************************
{
    return c_string_const_ptr{str, static_cast<int>(size)};
}

// ******************************************************************
inline c_wstring_const_ptr operator"" _sp(
    ali::wchar const* str, size_t size )
// ******************************************************************
{
    return c_wstring_const_ptr{str, static_cast<int>(size)};
}

// ******************************************************************
array<bool> test_array_utils( void );
// ******************************************************************

// ******************************************************************
//  The following functions are DEPRECATED.
// ******************************************************************

#ifndef ALI_EXCLUDE_DEPRECATED

// ******************************************************************
inline void mem_copy( void* dst, void const* src, int n )
// ******************************************************************
{
    platform::memmove(dst, src, n);
}


// ******************************************************************
inline void mem_move( void* first1, void const* first2, int n )
// ******************************************************************
{
    platform::memmove(first1, first2, n);
}

// ******************************************************************
inline void mem_fill( void* dst, unsigned char val, int n )
// ******************************************************************
{
    platform::memset(dst, val, n);
}

// ******************************************************************
inline int mem_cmp( void const* a, void const* b, int n )
// ******************************************************************
{
    return platform::memcmp(a, b, n);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
//  Copies elements from [src; src+n) to [dst; dst+n).
//  The arrays may overlap.
template <typename T, typename U>
inline void copy( T* dst, U const* src, int n )
// ******************************************************************
{
    hidden::copy(dst, src, n);
}

// ******************************************************************
//  Copies all elements from src to dst.
template <typename T, typename U, int n>
inline void copy( T (&dst)[n], U const (&src)[n] )
// ******************************************************************
{
    hidden::copy(dst, src, n);
}

// ******************************************************************
// ******************************************************************

namespace detail
{

// ******************************************************************
//  Returns true if the two arrays overlap.
template <typename T>
inline bool overlap( T const* a, T const* b, int n )
// ******************************************************************
{
    return a + n > b
        && b + n > a;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
//  Copies arrays of the same types in reverse order.
template <typename T>
void reverse_copy( T* dst, T const* src, int n )
// ******************************************************************
{
    if ( overlap(dst, src, n) )
    {
        copy(dst, src, n);
        reverse(dst, n);
    }
    else while ( n != 0 )
    {
        *dst++ = src[--n];
    }
}

// ******************************************************************
//  Copies arrays of different types in reverse order.
//  Assumption: dst and src don't overlap.
template <typename T, typename U>
void reverse_copy( T* dst, U const* src, int n )
// ******************************************************************
{
    while ( n != 0 )
        *dst++ = src[--n];
}

}   //  namespace detail

// ******************************************************************
//  Copies elements from [src; src+n) to [dst; dst+n) in reverse order.
//  The arrays may overlap.
template <typename T, typename U>
inline void reverse_copy( T* dst, U const* src, int n )
// ******************************************************************
{
    detail::reverse_copy(dst, src, n);
}

// ******************************************************************
//  Copies all elements from src to dst in reverse order.
//  The arrays may overlap.
template <typename T, typename U, int n>
inline void reverse_copy( T (&dst)[n], U const (&src)[n] )
// ******************************************************************
{
    detail::reverse_copy(dst, src, n);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline void fill( T* arr, T value, int n )
// ******************************************************************
{
    hidden::fill(arr, value, n);
}

// ******************************************************************
template <typename T, typename U>
inline void fill( T* arr, U value, int n )
// ******************************************************************
{
    hidden::fill(arr, static_cast<T>(value), n);
}

// ******************************************************************
template <typename T>
inline void fill( T* begin, T* end, T value )
// ******************************************************************
{
    hidden::fill(begin, value, end - begin);
}

// ******************************************************************
template <typename T, typename U>
inline void fill( T* begin, T* end, U value )
// ******************************************************************
{
    hidden::fill(begin, static_cast<T>(value), end - begin);
}

// ******************************************************************
template <typename T, int n>
inline void fill( T (&arr)[n], T value )
// ******************************************************************
{
    hidden::fill(arr, value, n);
}

// ******************************************************************
template <typename T, typename U, int n>
inline void fill( T (&arr)[n], U value )
// ******************************************************************
{
    hidden::fill(arr, static_cast<T>(value), n);
}

#endif  //  !ALI_EXCLUDE_DEPRECATED


// From ali_location.h

// ******************************************************************
inline c_string_const_ref location::file( void ) const
// ******************************************************************
{
    return file_from_path(path());
}

// ******************************************************************
inline c_string_const_ref location::path( void ) const
// ******************************************************************
{
    return c_string_const_ref(_path, _pathLen);
}

}   //  namespace ali
