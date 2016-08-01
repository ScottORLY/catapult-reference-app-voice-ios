/*
 *  ali_assoc_array.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_array.h"
#include "ali_deprecated.h"
#include "ali_exception.h"
#include "ali_utility.h"

namespace ali
{

// ******************************************************************
template <typename K, typename T, typename CompareT = ali::less>
class assoc_array
// ******************************************************************
{
public:     // Typedefs
    typedef ali::pair<K, T>                 value_type;
    typedef value_type const*               const_iterator;

public:
    assoc_array( void ) {}

    assoc_array( assoc_array const& b )
    :   _arr(b._arr),
        _comp(b._comp)
    {}

    assoc_array( ali::initializer_list<value_type> const& pairs )
    {
        set(pairs);
    }

    assoc_array& operator=( assoc_array const& b )
    {
        if ( this != &b )
        {
            _arr = b._arr;
            _comp = b._comp;
        }
        return *this;
    }

    template <typename KK>
    assoc_array& set( KK const& key, T const& value )
    {
        int const idx = index_of_lower_bound(key);

        if ( idx != _arr.size() && are_keys_equal(_arr[idx].first, key) )
            _arr[idx].second = value;
        else
            _arr.insert(idx, value_type(key, value));

        return *this;
    }

    assoc_array& set( ali::initializer_list<value_type> const& pairs )
    {
        for (auto const& value : pairs)
            set(value.first, value.second);
        return *this;
    }

#ifdef  ALI_FEATURE_RVALUE_REF

    assoc_array( assoc_array&& b )
    :   _arr(ali::move(b._arr)),
        _comp(ali::move(b._comp))
    {}

    assoc_array& operator=( assoc_array&& b )
    {
        if ( this != &b )
        {
            _arr = ali::move(b._arr);
            _comp = ali::move(b._comp);
        }
        return *this;
    }

    template <typename KK>
    assoc_array& set( KK const& key, T&& value )
    {
        int const idx = index_of_lower_bound(key);

        if ( idx != _arr.size() && are_keys_equal(_arr[idx].first, key) )
            _arr[idx].second = ali::move(value);
        else
            _arr.insert(idx, value_type(key, ali::move(value)));

        return *this;
    }

#endif  //  ALI_FEATURE_RVALUE_REF

    template <typename KK>
    int index_of_lower_bound( KK const& key ) const
        //  post:   0 <= result <= this->size()
        //      &&  this->at(result - 1).first < key <= this->at(result).first
        //              //  Where the index is not out of range.
    {
        int count = _arr.size();
	    int first = 0;

	    for ( ; count > 0; )
		{
		    int count2 = count / 2;
		    int mid = first + count2;

		    if ( _comp(_arr[mid].first, key) )
			    first = ++mid, count -= ++count2;
		    else
			    count = count2;
		}
	    
	    return first;
    }

    template <typename KK>
    int index_of( KK const& key ) const
        //  post:   result == this->size()
        //      ||  are_keys_equal(this->at(result).first, key)
    {
        int const idx = index_of_lower_bound(key);
        
        return idx != _arr.size() && are_keys_equal(
            _arr[idx].first, key) ? idx : _arr.size();
    }

ALI_DEPRECATED(
    template <typename KK>
    int find_index( KK const& key ) const
        //  Deprecated in favor of index_of.
    {
        return index_of(key);
    }
)

    template <typename KK>
    T const* find( KK const& key ) const
    {
        int const idx = index_of(key);
        
        return idx != _arr.size() ? &_arr[idx].second : nullptr;
    }
    
    template <typename KK>
    T* find( KK const& key )
    {
        int const idx = index_of(key);
        
        return idx != _arr.size() ? &_arr[idx].second : nullptr;
    }

    template <typename KK>
    T const& get( KK const& key ) const
    {
        T const* ret = find(key);
        ali_assert(ret != nullptr);
        return *ret;
    }

    template <typename KK>
    T& get( KK const& key )
    {
        T* ret = find(key);
        ali_assert(ret != nullptr);
        return *ret;
    }

    template <typename KK>
    bool contains( KK const& key ) const
    {
        return index_of(key) != _arr.size();
    }

    template <typename KK>
    bool erase( KK const& key )
    {
        int const idx = index_of(key);
        
        if ( idx == _arr.size() )
            return false;
        
        _arr.erase(idx);
        
        return true;
    }

    template <typename predicate>
    int erase_if( predicate p )
    {
        return _arr.erase_if(p);
    }
    
    template <typename KK>
    T& operator[]( KK const& key )
    {
        int const idx = index_of_lower_bound(key);
        
        if ( idx == _arr.size() || !are_keys_equal(_arr[idx].first, key) )
            _arr.insert(idx, value_type(key, T()));

        return _arr[idx].second;
    }
    
    int size( void ) const
    {
        return _arr.size();
    }
    
    bool is_empty( void ) const
    {
        return _arr.is_empty();
    }

ALI_DEPRECATED(
    bool empty( void ) const
        //  Deprecated in favor of is_empty.
    {
        return is_empty();
    }
)

    value_type const& at( int i ) const
    {
        return _arr[i];
    }

/* Dangerous as key is not constant; use value_at
    value_type& at( int i )
    {
        return _arr[i];
    }
*/

    T& value_at( int i )
    {
        return _arr[i].second;
    }

    T const& value_at( int i ) const
    {
        return _arr[i].second;
    }
    
    value_type const& front( void ) const
    {
        return _arr.front();
    }

/* Dangerous as key is not constant; use front_value
    value_type& front( void )
    {
        return _arr.front();
    }
*/

    T& front_value( void )
    {
        return _arr.front();
    }

    T const& front_value( void ) const
    {
        return _arr.front();
    }
    
    value_type const& back( void ) const
    {
        return _arr.back();
    }

/* Dangerous as key is not constant; use back_value
    value_type& back( void )
    {
        return _arr.back();
    }
*/

    T& back_value( void )
    {
        return _arr.front();
    }

    T const& back_value( void ) const
    {
        return _arr.front();
    }

    const_iterator begin( void ) const
    {
        return _arr.begin();
    }

    friend const_iterator begin( assoc_array const& arr )
    {
        return arr.begin();
    }

    const_iterator end( void ) const
    {
        return _arr.end();
    }

    friend const_iterator end( assoc_array const& arr )
    {
        return arr.end();
    }
    
    assoc_array& erase_at( int i )
    {
        _arr.erase(i);
        return *this;
    }

    assoc_array& erase( void )
    {
        _arr.erase();
        return *this;
    }

ALI_DEPRECATED(
    void clear( void )
        //  Deprecated in favor of erase.
    {
        erase();
    }
)

    friend bool operator==(
        assoc_array const& a,
        assoc_array const& b )
    {
        if ( a._arr.size() != b._arr.size() )
            return false;
        
        for ( int i = a._arr.size(); i != 0; )
        {
            --i;
            
            bool const are_pairs_equal
                =   a.are_keys_equal(
                        a._arr[i].first,
                        b._arr[i].first)
                            //  Same keys.
                &&  a._arr[i].second == b._arr[i].second;
                            //  Same values.

            if ( !are_pairs_equal )
                return false;
        }

        return true;
    }

    friend bool operator!=(
        assoc_array const& a,
        assoc_array const& b )
    {
        return !(a == b);
    }
    
    void swap( assoc_array& b )
    {
        using ali::swap;
        swap(_arr, b._arr);
        swap(_comp, b._comp);
    }

    friend void swap( assoc_array& a, assoc_array& b )
    {
        a.swap(b);
    }

    template <typename K1, typename K2>
    bool are_keys_equal( K1 const& k1, K2 const& k2 ) const
    {
        return !_comp(k1, k2) && !_comp(k2, k1);
    }

    array_const_ref<value_type> ref( void ) const
    {
        return _arr.ref();
    }
    
private:    // Data members
    ali::array<value_type>  _arr;
    CompareT                _comp;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename K, typename T, typename CompareT = ali::less>
class assoc_auto_ptr_array
// ******************************************************************
{
public:     // Typedefs
    typedef ali::pair<K, T*>        value_type;

public:
    assoc_auto_ptr_array( void ) {}

    assoc_auto_ptr_array(
        assoc_auto_ptr_array const& b )
    {
        assoc_auto_ptr_array temp;

        temp._arr.reserve(b._arr.size());

        for ( int i = 0; i != b._arr.size(); ++i )
        {
            ali::auto_ptr<T> value;
            
            if ( b._arr[i].second != nullptr )
                value.reset(
                    ali::out_of_memory_if_null(
                        new T(*b._arr[i].second),
                        ALI_HERE));

            temp._arr.push_back(
                value_type(
                    b._arr[i].first,
                    value.get()));

            value.release();
        }

        swap(temp);
    }

    ~assoc_auto_ptr_array( void )
    {
        erase();
    }

    assoc_auto_ptr_array& operator=(
        assoc_auto_ptr_array const& b )
    {
        if ( this != &b )
            assoc_auto_ptr_array(b).swap(*this);
        return *this;
    }

    template <typename KK>
    void set( KK const& key, ali::auto_ptr<T> value )
    {
        int const idx = index_of_lower_bound(key);
        
        if ( idx != _arr.size() && are_keys_equal(_arr[idx].first, key) )
        {
            delete _arr[idx].second;
            
            _arr[idx].second = value.release();
        }
        else
        {
            _arr.insert(idx, value_type(key, value.get()));
            
            value.release();
        }
    };
    
    template <typename KK>
    int index_of_lower_bound( KK const& key ) const
        //  post:   0 <= result <= this->size()
        //      &&  this->at(result - 1).first < key <= this->at(result).first
        //              //  Where the index is not out of range.
    {
        int count = _arr.size();
	    int first = 0;

	    for ( ; count > 0; )
		{
		    int count2 = count / 2;
		    int mid = first + count2;

		    if ( _comp(_arr[mid].first, key) )
			    first = ++mid, count -= ++count2;
		    else
			    count = count2;
		}
	    
	    return first;
    }
    
    template <typename KK>
    int index_of( KK const& key ) const
        //  post:   result == this->size()
        //      ||  are_keys_equal(this->at(result).first, key)
    {
        int const idx = index_of_lower_bound(key);
        
        return idx != _arr.size() && are_keys_equal(
            _arr[idx].first, key) ? idx : _arr.size();
    }

ALI_DEPRECATED(
    template <typename KK>
    int find_index( KK const& key ) const
        //  Deprecated in favor of index_of.
    {
        return index_of(key);
    }
)

    template <typename KK>
    T const* find( KK const& key ) const
    {
        int const idx = index_of(key);
        
        return idx != _arr.size() ? _arr[idx].second : nullptr;
    }
    
    template <typename KK>
    T* find( KK const& key )
    {
        int const idx = index_of(key);
        
        return idx != _arr.size() ? _arr[idx].second : nullptr;
    }

    template <typename KK>
    T const& get( KK const& key ) const
    {
        T const* ret = find(key);
        ali_assert(ret != nullptr);
        return *ret;
    }

    template <typename KK>
    T& get( KK const& key )
    {
        T* ret = find(key);
        ali_assert(ret != nullptr);
        return *ret;
    }

    template <typename KK>
    ali::auto_ptr<T> erase( KK const& key )
    {
        ali::auto_ptr<T> t;
        
        int const idx = index_of(key);
        
        if ( idx != _arr.size() )
        {
            t.reset(_arr[idx].second);

            _arr.erase(idx);
        }

        return t;
    }
    
    template <typename KK>
    ali::auto_ptr<T> release_no_erase( KK const& key )
    {
        ali::auto_ptr<T> t;
        
        int const idx = index_of(key);
        
        if ( idx != _arr.size() )
        {
            t.reset(_arr[idx].second);

            _arr[idx].second = nullptr;
        }
        
        return t;
    }
    
    bool is_empty( void ) const
    {
        return _arr.is_empty();
    }

ALI_DEPRECATED(
    bool empty( void ) const
        //  Deprecated in favor of is_empty.
    {
        return is_empty();
    }
)

    int size( void ) const
    {
        return _arr.size();
    }
    
    value_type const& at( int i ) const
    {
        return _arr[i];
    }
    
    value_type& at( int i )
    {
        return _arr[i];
    }
    
    ali::auto_ptr<T> erase_at( int i )
    {
        ali::auto_ptr<T> t(_arr[i].second);
        
        _arr.erase(i);
        
        return t;
    }
    
    assoc_auto_ptr_array& erase( void )
    {
        for ( ; !_arr.is_empty(); _arr.erase_back() )
            delete _arr.back().second;
        
        return *this;
    }

    template <typename predicate>
    int erase_if( predicate p )
    {
        return _arr.erase_if(p);
    }

ALI_DEPRECATED(
    void clear( void )
        //  Deprecated in favor of erase.
    {
        erase();
    }
)

    friend bool operator==(
        assoc_auto_ptr_array const& a,
        assoc_auto_ptr_array const& b )
    {
        if ( a._arr.size() != b._arr.size() )
            return false;

        for ( int i = a._arr.size(); i != 0; )
        {
            --i;
            
            bool const are_pairs_equal
                =   a.are_keys_equal(
                        a._arr[i].first,
                        b._arr[i].first)
                            //  Same keys.
                &&  (a._arr[i].second == nullptr)
                        == (b._arr[i].second == nullptr)
                                //  Both null or both non-null.
                &&  (   a._arr[i].second == nullptr
                    ||  *a._arr[i].second == *b._arr[i].second);
                            //  Same values.

            if ( !are_pairs_equal )
                return false;
        }
        return true;
    }

    friend bool operator!=(
        assoc_auto_ptr_array const& a,
        assoc_auto_ptr_array const& b )
    {
        return !(a == b);
    }

    void swap( assoc_auto_ptr_array& b )
    {
        using ali::swap;
        swap(_arr, b._arr);
        swap(_comp, b._comp);
    }

    friend void swap(
        assoc_auto_ptr_array& a,
        assoc_auto_ptr_array& b )
    {
        a.swap(b);
    }

    template <typename K1, typename K2>
    bool are_keys_equal( K1 const& k1, K2 const& k2 ) const
    {
        return !_comp(k1, k2) && !_comp(k2, k1);
    }

    array_const_ref<value_type> ref( void ) const
    {
        return _arr.ref();
    }
        
private:    // Data members
    ali::array<value_type>  _arr;
    CompareT                _comp;
};

// ******************************************************************
// ******************************************************************

#if 0

// ******************************************************************
template <typename K, typename T, typename CompareT, typename U>
int find(
    ali::assoc_array<K, T, CompareT> const& arr, U const& u )
// ******************************************************************
{
    int i = 0;
    for ( ; i < arr.size(); ++i )
        if ( arr.at(i).second == u )
            break;
    return i;
}

// ******************************************************************
template <typename K, typename S, typename CompareT, typename T, typename U, typename V>
int find(
    ali::assoc_array<K, S, CompareT> const& arr, U T::*member, V const& v )
// ******************************************************************
{
    int i = 0;
    for ( ; i < arr.size(); ++i )
        if ( arr.at(i).second.*member == v )
            break;
    return i;
}

// ******************************************************************
template <typename K, typename T, typename CompareT>
int find_ptr(
    ali::assoc_auto_ptr_array<K, T, CompareT> const& arr, T const* t )
// ******************************************************************
{
    int i = 0;
    for ( ; i < arr.size(); ++i )
        if ( arr.at(i).second == t )
            break;
    return i;
}

// ******************************************************************
template <typename K, typename T, typename CompareT, typename U>
int find(
    ali::assoc_auto_ptr_array<K, T, CompareT> const& arr, U const& u )
// ******************************************************************
{
    int i = 0;
    for ( ; i < arr.size(); ++i )
        if ( *arr.at(i).second == u )
            break;
    return i;
}

// ******************************************************************
template <typename K, typename S, typename CompareT, typename T, typename U, typename V>
int find(
    ali::assoc_auto_ptr_array<K, S, CompareT> const& arr, U T::*member, V const& v )
// ******************************************************************
{
    int i = 0;
    for ( ; i < arr.size(); ++i )
        if ( arr.at(i).second->*member == v )
            break;
    return i;
}

#endif

}   //  namespace ali

