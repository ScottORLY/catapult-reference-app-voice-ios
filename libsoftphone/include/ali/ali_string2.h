/*
 *  ali_string2.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_array_utils.h"
#include "ali_deprecated.h"
#include "ali_meta.h"
#include "ali_str_utils.h"
#include "ali_utility.h"

// ******************************************************************

// Allow COW sharing of strings. Not thread safe.
#define ALI_STRING2_COPY_ON_WRITE

// Debug buffer overflow.
//#define ALI_STRING2_DEBUG_OVERFLOW

// Debug thread safety. It comes with a moderate cost and might be too paranoid.
#define ALI_STRING2_DEBUG_THREAD_SAFETY

/* Debug null dereference. It comes with quite a high cost because it backtraces
 * all string creations, assigning and destructions */
//#define ALI_STRING2_DEBUG_NULL_DEREFERENCE

// ******************************************************************

#if defined(ALI_STRING2_DEBUG_OVERFLOW)
#   define ALI_STRING2_IF_DEBUG_OVERFLOW(...) __VA_ARGS__
#else
#   define ALI_STRING2_IF_DEBUG_OVERFLOW(...)
#endif

// ******************************************************************

#if defined(ALI_STRING2_DEBUG_THREAD_SAFETY)
#   include "ali_thread_identifier.h"
#   define ALI_STRING2_IF_DEBUG_THREAD_SAFETY(...) __VA_ARGS__
#else
#   define ALI_STRING2_IF_DEBUG_THREAD_SAFETY(...)
#endif

// ******************************************************************

#if defined(ALI_STRING2_DEBUG_NULL_DEREFERENCE)
#   define ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(...) __VA_ARGS__
#else
#   define ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(...)
#endif

// ******************************************************************

namespace ali
{

/*
class string2data
{
public:     // Typedefs
    typedef char        value_type;
    typedef value_type  E;
    typedef char*       iterator;
    typedef char const* const_iterator;
    
public:
    string2data( void );
    string2data( string2data const& b, int pos, int n );
    string2data( E const* str, int n );
    
    bool is_empty( void ) const;
    int size( void ) const;
    int capacity( void ) const;
    
    iterator begin( void );
    const_iterator begin( void ) const;
    
    iterator end( void );
    const_iterator end( void ) const;
    
    E const& operator[]( int i ) const;
    E& operator[]( int i );
    
    E const* data( void ) const;
    E const* c_str( void ) const;

protected:  // Methods
    ~string2data( void );

    void assign( string2data_sso_rc const& b, int pos, int n );
    void assign( string_const_ref b );
    void assign( int n, E c );
    
    void reserve( int n );
    void resize( int n, E c );
    void erase( void );

    void swap( string2data& b );

    void ensure_unique( void );

    is_internal_pointer_result is_internal_pointer( E const* str, int n ) const;
    is_internal_pointer_result is_internal_pointer( string_const_ref str ) const;
    is_internal_pointer_result is_internal_pointer( string_const_ptr str ) const;
    
    void set( int pos, string_const_ref str );
    void set( int pos, int n, E c );
    void move( int first1, int first2, int n );

private:    //  Methods
    string2data& operator=( string2data const& b );
};
*/

// ******************************************************************
class string2data_sso_rc
// ******************************************************************
{
public:     // Typedefs
    typedef char        value_type;
    typedef value_type  E;
    typedef char*       iterator;
    typedef char const* const_iterator;
    
public:
    string2data_sso_rc( void );
    string2data_sso_rc(
        string2data_sso_rc const& b,
        int pos, int n );
    explicit string2data_sso_rc(
        string_const_ref str );

    bool is_empty( void ) const
    {
        return _len == 0;
    }

    int size( void ) const
    {
        return _len;
    }
    
    int capacity( void ) const
    {
        return _cap;
    }
    
    iterator begin( void )
    {
        ensure_unique();
        set_shareable(false);
        return buf();
    }

    friend iterator begin( string2data_sso_rc & str )
    {
        return str.begin();
    }
    
    const_iterator begin( void ) const
    {
        return buf();
    }

    friend const_iterator begin( string2data_sso_rc const& str )
    {
        return str.begin();
    }
    
    iterator end( void )
    {
        ensure_unique();
        set_shareable(false);
        return buf() + _len;
    }

    friend iterator end( string2data_sso_rc & str )
    {
        return str.end();
    }
    
    const_iterator end( void ) const
    {
        return buf() + _len;
    }

    friend const_iterator end( string2data_sso_rc const& str )
    {
        return str.end();
    }
    
    E const& operator[]( int i ) const
    {
        return *(buf() + i);
    }
    
    E& operator[]( int i )
    {
        ensure_unique();
        set_shareable(false);
        return *(buf() + i);
    }
    
    E const* data( void ) const
    {
        return buf();
    }
    
    E const* c_str( void ) const
    {
        return buf();
    }

    int get_reference_count() const;

protected:  // Methods
    ~string2data_sso_rc( void );

    void assign(
        string2data_sso_rc const& b,
        int pos, int n );
    void assign( string_const_ref b );
    void assign( int n, E c );
    
    void reserve( int n );
    void resize( int n, E c );
    void erase( void );

    void swap( string2data_sso_rc& b );

    void ensure_unique( void )
    {
        ensure_unique(_cap, _len);
    }

    is_internal_pointer_result is_internal_pointer( E const* str, int n ) const
    {
        ali_assert(0 <= n);

        bool const is_internal = buf() <= str && str <= buf() + size();
        
        ali_assert(!is_internal || n <= size() - (str - buf()));
        
        return is_internal_pointer_result{
            is_internal ? static_cast<int>(str - buf()) : -1};
    }

    is_internal_pointer_result is_internal_pointer( string_const_ref str ) const
    {
        return is_internal_pointer(str.data(), str.size());
    }

    is_internal_pointer_result is_internal_pointer( string_const_ptr str ) const
    {
        return is_internal_pointer(str->data(), str->size());
    }
    
    void set( int pos, string_const_ref str );
    void set( int pos, int n, E c );
    void move( int first1, int first2, int n );
    
public:     // Enums
    enum { SmallCapacity = 16 };
    ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(
        enum { BacktraceItems = 20 });
    
private:    // Struct
    struct large
    {
        E*                                  data;
        ALI_STRING2_IF_DEBUG_THREAD_SAFETY(
            ali::thread::identifier *       thread);
        mutable string2data_sso_rc const*   next;
        mutable string2data_sso_rc const*   prev;
    };
    
private:    // Methods
    string2data_sso_rc& operator=(
        string2data_sso_rc const& b );
    
    bool is_large( void ) const
    {
        return _cap > SmallCapacity;
    }
    
    bool is_shareable( void ) const
    {
        ali_assert(is_large());
        return _large.data[0] == ' ';
    }
    
    void set_shareable( bool val )
    {
        if ( !is_large() ) return;
        _large.data[0] = val ? ' ' : '!';
    }
    
    bool is_last( void ) const
    {
        ali_assert(is_large());
        return _large.prev == _large.next && _large.prev == this;
    }

    E* buf( void )
    {
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(check_for_null_dereference());
        ALI_STRING2_IF_DEBUG_OVERFLOW(check_for_overflow());
        return is_large() ? &_large.data[1] : _small;
    }
    
    E const* buf( void ) const
    {
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(check_for_null_dereference());
        ALI_STRING2_IF_DEBUG_OVERFLOW(check_for_overflow());
        return is_large() ? &_large.data[1] : _small;
    }
    
    int new_capacity( int cap ) const
    {
        return ali::maxi(((_cap << 1) + _cap) >> 1, cap);
    }
    
    void ensure_unique( int cap, int len );

ALI_STRING2_IF_DEBUG_OVERFLOW(
    void check_for_overflow( void ) const
    {
        E const* buf = is_large() ? &_large.data[1] : _small;
        ali_assert(make_ref(buf + _cap + 1, _guard_size) == make_ref(_guard));
        ali_assert(make_ref((E const*)_trailer, _guard_size) == make_ref(_guard));
        ali_assert(
                !is_large()
            ||  make_ref(_trailer + sizeof(_guard), sizeof(large))
                    == make_ref((ali::uint8 const*)&_large, sizeof(large)));
    }

    void save_large( void ) const
    {
        make_mutable_ref(_trailer + sizeof(_guard), sizeof(large))
            = make_ref((ali::uint8 const*)&_large, sizeof(large));
    }
);

ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(
    void check_for_null_dereference( void ) const;
);

ALI_STRING2_IF_DEBUG_THREAD_SAFETY(
    void check_thread_safety( void ) const
    {
        if (!is_large())
            return;
        ali::thread::identifier currentThread;
        if (is_last())
            // Allow moving between threads
            *_large.thread = currentThread;
        else
            ali_assert(*_large.thread == currentThread);
    }
);
    
    
    static void swap_ll(
        string2data_sso_rc& a,
        string2data_sso_rc& b );
    static void swap_ss(
        string2data_sso_rc& a,
        string2data_sso_rc& b );
    static void swap_ls(
        string2data_sso_rc& a,
        string2data_sso_rc& b );
    
private:    // Data members
    ALI_STRING2_IF_DEBUG_OVERFLOW(
        enum {          _guard_size = 4 };
        static E const  _guard[_guard_size];
    );
    ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(
        void*           _created[BacktraceItems];
        void*           _assigned[BacktraceItems];
        void*           _destroyed[BacktraceItems]);

    int                 _cap;
    int                 _len;
    union
    {
        E               _small[SmallCapacity + ALI_STRING2_IF_DEBUG_OVERFLOW(_guard_size +) 1];
        large           _large;
    };

ALI_STRING2_IF_DEBUG_OVERFLOW(
    mutable ali::uint8  _trailer[sizeof(_guard) + sizeof(large)]
);

};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class string2data_sso
// ******************************************************************
{
public:     // Typedefs
    typedef char                value_type;
    typedef value_type          E;
    typedef value_type*         iterator;
    typedef value_type const*   const_iterator;
    
public:
    string2data_sso( void )
    :   _begin(_small.begin),
        _size(0)
    {
        _begin[_size] = '\0';

        ali_assert(is_small());
    }

    string2data_sso(
        string2data_sso const& b,
        int pos, int n );
    
    explicit string2data_sso(
        string_const_ref str );
    
    bool is_empty( void ) const
    {
        return _size == 0;
    }

    int size( void ) const
    {
        return _size;
    }
    
    int capacity( void ) const
    {
        if (is_small())
            return _small_capacity;
        return _capacity;
    }
    
    iterator begin( void )
    {
        return _begin;
    }

    friend iterator begin( string2data_sso & str )
    {
        return str.begin();
    }
    
    const_iterator begin( void ) const
    {
        return _begin;
    }

    friend const_iterator begin( string2data_sso const& str )
    {
        return str.begin();
    }
    
    iterator end( void )
    {
        return _begin + _size;
    }

    friend iterator end( string2data_sso & str )
    {
        return str.end();
    }
    
    const_iterator end( void ) const
    {
        return _begin + _size;
    }

    friend const_iterator end( string2data_sso const& str )
    {
        return str.end();
    }

    E const& operator[]( int i ) const
    {
        ali_assert(0 <= i);
        ali_assert(i < _size);
        return _begin[i];
    }
    
    E& operator[]( int i )
    {
        ali_assert(0 <= i);
        ali_assert(i < _size);
        return _begin[i];
    }
    
    E const* data( void ) const
    {
        ali_assert(_begin[_size] == '\0');
        return _begin;
    }
    
    E const* c_str( void ) const
    {
        return data();
    }
    
protected:  // Methods
    ~string2data_sso( void )
    {
        release();
        _begin = nullptr;
        _size = 0;
        _capacity = 0;
    }

    void assign(
        string2data_sso const& b,
        int pos, int n )
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= b._size);
        ali_assert(0 <= n);
        ali_assert(n <= b._size - pos);

        assign(string_const_ref(b.data() + pos, n));
    }

    void assign( string_const_ref b );
    void assign( int n, E c );
    
    void reserve( int n )
    {
        ali_assert(0 <= n);
    
        private_reserve(n, _size);
    }
    
    void resize( int n, E c );

    void erase( void )
    {
        _size = 0;

        _begin[_size] = '\0';
    }

    void swap( string2data_sso& b );

    void ensure_unique( void ) {}

    is_internal_pointer_result is_internal_pointer( E const* str, int n ) const
    {
        ali_assert(0 <= n);

        bool const is_internal = _begin <= str && str <= _begin + _size;
        
        ali_assert(!is_internal || n <= _size - (str - _begin));
        
        return is_internal_pointer_result{
            is_internal ? static_cast<int>(str - _begin) : -1};
    }

    is_internal_pointer_result is_internal_pointer( string_const_ref str ) const
    {
        return is_internal_pointer(str.data(), str.size());
    }

    is_internal_pointer_result is_internal_pointer( string_const_ptr str ) const
    {
        return is_internal_pointer(str->data(), str->size());
    }
    
    void set( int pos, string_const_ref str )
    {
        string_ref(_begin, _size).copy(pos, str);
    }

    void set( int pos, int n, E c )
    {
        string_ref(_begin, _size).mutable_ref(pos, n).fill(c);
    }

    void move( int first1, int first2, int n )
    {
        string_ref self(_begin, _size);

        self.mutable_ref(first1, n) = self.ref(first2, n);
    }
    
private:    // Methods
    string2data_sso& operator=(
        string2data_sso const& b );

    bool is_small( void ) const
    {
        return _begin == _small.begin;
    }

    void release( void );

    void private_reserve(
        int min_capacity,
        int retained_size );
    
    static void swap_ll(
        string2data_sso& a,
        string2data_sso& b )
    {
        ali_assert(!a.is_small());
        ali_assert(!b.is_small());
    
        using ali::swap;
        swap(a._begin, b._begin);
        swap(a._size, b._size);
        swap(a._capacity, b._capacity);
    }
    
    static void swap_ss(
        string2data_sso& a,
        string2data_sso& b )
    {
        ali_assert(a.is_small());
        ali_assert(b.is_small());
    
        using ali::swap;
        swap(a._size, b._size);
        swap_by_copy(a._small, b._small);
    }

    static void swap_ls(
        string2data_sso& a,
        string2data_sso& b );

private:    // Data members
    static int const    _nice_object_size = 16;
    static int const    _min_object_size
                            = sizeof(E*) + sizeof(unsigned int) + meta::maxi<
                                sizeof(int), sizeof(E)>::result;
    static int const    _object_size
                            = meta::maxi<
                                _min_object_size,
                                _nice_object_size>::result;
    static int const    _small_capacity
                            = (_object_size - sizeof(E*) - sizeof(unsigned int))
                                / sizeof(E) - 1;

    struct small_data
    {
        E               begin[_small_capacity + 1];
    };

    union
    {
        int             _capacity;
        small_data      _small;
    };
    E*                  _begin;
    int                 _size;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class string2data_sso_rc2
// ******************************************************************
{
public:     // Typedefs
    typedef char                value_type;
    typedef value_type          E;
    typedef value_type*         iterator;
    typedef value_type const*   const_iterator;

public:     //  Struct
    struct copy_on_write_profile_data
    {
        long long number_of_non_copies_due_to_sharable_source;
        long long number_of_non_copied_bytes_due_to_sharable_source;
        long long number_of_copies_due_to_non_sharable_source;
        long long number_of_copied_bytes_due_to_non_sharable_source;
    };
    
public:
    string2data_sso_rc2( void )
    :   _begin(_small.begin),
        _size(0)
    {
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(init_backtraces());
        _begin[_size] = '\0';

        ali_assert(is_small());
    }

    string2data_sso_rc2(
        string2data_sso_rc2 const& b,
        int pos, int n );
    
    explicit string2data_sso_rc2(
        string_const_ref str );
    
    bool is_empty( void ) const
    {
        return _size == 0;
    }

    int size( void ) const
    {
        return _size;
    }
    
    int capacity( void ) const
    {
        if ( is_small() )
            return _small_capacity;
        return _large->capacity;
    }
    
    iterator begin( void )
    {
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(check_for_null_dereference());
        make_non_sharable();
        return _begin;
    }

    friend iterator begin( string2data_sso_rc2 & str )
    {
        return str.begin();
    }
    
    const_iterator begin( void ) const
    {
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(check_for_null_dereference());
        return _begin;
    }

    friend const_iterator begin( string2data_sso_rc2 const& str )
    {
        return str.begin();
    }
    
    iterator end( void )
    {
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(check_for_null_dereference());
        make_non_sharable();
        return _begin + _size;
    }

    friend iterator end( string2data_sso_rc2 & str )
    {
        return str.end();
    }
    
    const_iterator end( void ) const
    {
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(check_for_null_dereference());
        return _begin + _size;
    }

    friend const_iterator end( string2data_sso_rc2 const& str )
    {
        return str.end();
    }

    E const& operator[]( int i ) const
    {
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(check_for_null_dereference());
        ali_assert(0 <= i);
        ali_assert(i < _size);
        return _begin[i];
    }
    
    E& operator[]( int i )
    {
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(check_for_null_dereference());
        ali_assert(0 <= i);
        ali_assert(i < _size);
        make_non_sharable();
        return _begin[i];
    }
    
    E const* data( void ) const
    {
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(check_for_null_dereference());
        ali_assert(_begin[_size] == '\0');
        return _begin;
    }
    
    E const* c_str( void ) const
    {
        return data();
    }

    is_internal_pointer_result is_internal_pointer( E const* str, int n ) const
    {
        ali_assert(0 <= n);

        bool const is_internal = _begin <= str && str <= _begin + _size;
        
        ali_assert(!is_internal || n <= _size - (str - _begin));
        
        return is_internal_pointer_result{
            is_internal ? static_cast<int>(str - _begin) : -1};
    }

    is_internal_pointer_result is_internal_pointer( string_const_ref str ) const
    {
        return is_internal_pointer(str.data(), str.size());
    }

    is_internal_pointer_result is_internal_pointer( string_const_ptr str ) const
    {
        return is_internal_pointer(str->data(), str->size());
    }

    static copy_on_write_profile_data
        get_copy_on_write_profile_data( void )
    {
        return _copy_on_write_profile_data;
    }
    
protected:  // Methods
    ~string2data_sso_rc2( void )
    {
        release();
        _begin = nullptr;
        _size = 0;
    }

    void assign(
        string2data_sso_rc2 const& b,
        int pos, int n );
    void assign( string_const_ref b );
    void assign( int n, E c );
    
    void reserve( int n )
    {
        ali_assert(0 <= n);
    
        private_reserve(n, _size, false);

        make_sharable();
    }
    
    void resize( int n, E c );

    void erase( void );
    
    void swap( string2data_sso_rc2& b );

    void ensure_unique( void )
    {
        private_reserve(_size, _size, true);
    }

    void set( int pos, string_const_ref str )
    {
        ensure_unique();

        ali_assert(is_small() || _large->rc < 2);

        string_ref(_begin, _size).copy(pos, str);
    }

    void set( int pos, int n, E c )
    {
        ensure_unique();

        ali_assert(is_small() || _large->rc < 2);

        string_ref(_begin, _size).mutable_ref(pos, n).fill(c);
    }

    void move( int first1, int first2, int n )
    {
        ensure_unique();

        ali_assert(is_small() || _large->rc < 2);

        string_ref self(_begin, _size);

        self.mutable_ref(first1, n) = self.ref(first2, n);
    }

ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(
    void check_for_null_dereference( void ) const;
    void init_backtraces( void );
);

private:    //  Struct
    struct large_header
    {
        int rc;
        int capacity;
    };

    ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(
        enum { BacktraceItems = 20,
               BacktraceSize = BacktraceItems * sizeof(void*) });
    
private:    //  Methods
    string2data_sso_rc2& operator=(
        string2data_sso_rc2 const& b );

    bool is_small( void ) const
    {
        return _begin == _small.begin;
    }

    bool is_sharable( void ) const
    {
        ali_assert(!is_small());
        return _large->rc != 0;
    }

    void make_non_sharable( void )
    {
        ensure_unique();

        ali_assert(is_small() || _large->rc < 2);

        if ( !is_small() )
            _large->rc = 0;
    }
    
    void make_sharable( void )
    {
        if ( !is_small() && _large->rc == 0 )
            ++_large->rc;
    }

    void release( large_header* large );

    void release( void );

    void private_reserve(
        int min_capacity,
        int retained_size,
        bool ensure_unique );

    static void swap_ll(
        string2data_sso_rc2& a,
        string2data_sso_rc2& b )
    {
        ali_assert(!a.is_small());
        ali_assert(!b.is_small());
    
        using ali::swap;
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(
            ali::swap(a._created, b._created);
            ali::swap(a._assigned, b._assigned));
        swap(a._begin, b._begin);
        swap(a._size, b._size);
        swap(a._large, b._large);
    }
    
    static void swap_ss(
        string2data_sso_rc2& a,
        string2data_sso_rc2& b )
    {
        ali_assert(a.is_small());
        ali_assert(b.is_small());
    
        using ali::swap;
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(
            ali::swap(a._created, b._created);
            ali::swap(a._assigned, b._assigned));
        swap(a._size, b._size);
        swap_by_copy(a._small, b._small);
    }

    static void swap_ls(
        string2data_sso_rc2& a,
        string2data_sso_rc2& b );

private:    // Data members
    static copy_on_write_profile_data
                        _copy_on_write_profile_data;
    static int const    _nice_object_size = 16;
    static int const    _min_object_size
                            = sizeof(E*) + sizeof(unsigned int) + meta::maxi<
                                sizeof(large_header*), sizeof(E)>::result;
    static int const    _object_size
                            = meta::maxi<
                                _min_object_size,
                                _nice_object_size>::result;
    static int const    _small_capacity
                            = (_object_size - sizeof(E*) - sizeof(unsigned int))
                                / sizeof(E) - 1;

    struct small_data
    {
        E               begin[_small_capacity + 1];
    };

    union
    {
        large_header*   _large;
        small_data      _small;
    };
    E*                  _begin;
    int                 _size;
    ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(
        void*                           _created[BacktraceItems];
        void*                           _assigned[BacktraceItems];
        void*                           _destroyed[BacktraceItems]);
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class wstring2data_sso_rc
// ******************************************************************
{
public:     // Typedefs
    typedef wchar               value_type;
    typedef value_type          E;
    typedef wchar*              iterator;
    typedef wchar const*        const_iterator;
    
public:
    wstring2data_sso_rc( void );
    wstring2data_sso_rc(
        wstring2data_sso_rc const& b,
        int pos, int n );
    explicit wstring2data_sso_rc(
        wstring_const_ref str );
    
    bool is_empty( void ) const
    {
        return _len == 0;
    }

    int size( void ) const
    {
        return _len;
    }
    
    int capacity( void ) const
    {
        return _cap;
    }
    
    iterator begin( void )
    {
        ensure_unique();
        set_shareable(false);
        return buf();
    }

    friend iterator begin( wstring2data_sso_rc & str )
    {
        return str.begin();
    }
    
    const_iterator begin( void ) const
    {
        return buf();
    }

    friend const_iterator begin( wstring2data_sso_rc const& str )
    {
        return str.begin();
    }
    
    iterator end( void )
    {
        ensure_unique();
        set_shareable(false);
        return buf() + _len;
    }

    friend iterator end( wstring2data_sso_rc & str )
    {
        return str.end();
    }
    
    const_iterator end( void ) const
    {
        return buf() + _len;
    }

    friend const_iterator end( wstring2data_sso_rc const& str )
    {
        return str.end();
    }
    
    E const& operator[]( int i ) const
    {
        return *(buf() + i);
    }
    
    E& operator[]( int i )
    {
        ensure_unique();
        set_shareable(false);
        return *(buf() + i);
    }
    
    E const* data( void ) const
    {
        return buf();
    }
    
    E const* c_str( void ) const
    {
        return buf();
    }

    is_internal_pointer_result is_internal_pointer( E const* str, int n ) const
    {
        ali_assert(0 <= n);

        bool const is_internal = buf() <= str && str <= buf() + size();
        
        ali_assert(!is_internal || n <= size() - (str - buf()));
        
        return is_internal_pointer_result{
            is_internal ? static_cast<int>(str - buf()) : -1};
    }

    is_internal_pointer_result is_internal_pointer( wstring_const_ref str ) const
    {
        return is_internal_pointer(str.data(), str.size());
    }

    is_internal_pointer_result is_internal_pointer( wstring_const_ptr str ) const
    {
        return is_internal_pointer(str->data(), str->size());
    }

protected:  // Methods
    ~wstring2data_sso_rc( void );

    void assign(
        wstring2data_sso_rc const& b,
        int pos, int n );
    void assign( wstring_const_ref b );
    void assign( int n, E c );
    
    void reserve( int n );
    
    void resize( int n, E c );
    
    void erase( void );

    void swap( wstring2data_sso_rc& b );
    
    void ensure_unique( void )
    {
        ensure_unique(_cap, _len);
    }

    void set( int pos, wstring_const_ref str );
    void set( int pos, int n, E c );
    void move( int first1, int first2, int n );
    
public:     // Enums
    enum { SmallCapacity = 16 };
    
private:    // Struct
    struct large
    {
        E*                                  data;
        mutable wstring2data_sso_rc const*  next;
        mutable wstring2data_sso_rc const*  prev;
    };
    
private:    // Methods
    wstring2data_sso_rc& operator=( wstring2data_sso_rc const& b );
    
    bool is_large( void ) const
    {
        return _cap > SmallCapacity;
    }
    
    bool is_shareable( void ) const
    {
        ali_assert(is_large());
        return _large.data[0] == ' ';
    }
    
    void set_shareable( bool val )
    {
        if ( !is_large() ) return;
        _large.data[0] = val ? ' ' : '!';
    }
    
    bool is_last( void ) const
    {
        ali_assert(is_large());
        return _large.prev == _large.next && _large.prev == this;
    }

    E* buf( void )
    {
        return is_large() ? &_large.data[1] : _small;
    }
    
    E const* buf( void ) const
    {
        return is_large() ? &_large.data[1] : _small;
    }
    
    int new_capacity( int cap ) const
    {
        return ali::maxi(((_cap << 1) + _cap) >> 1, cap);
    }
    
    void ensure_unique( int cap, int len );
    
    static void swap_ll(
        wstring2data_sso_rc& a,
        wstring2data_sso_rc& b );
    static void swap_ss(
        wstring2data_sso_rc& a,
        wstring2data_sso_rc& b );
    static void swap_ls(
        wstring2data_sso_rc& a,
        wstring2data_sso_rc& b );
    
private:    // Data members
    int   _cap;
    int   _len;
    union
    {
        E       _small[SmallCapacity + 1];
        large   _large;
    };
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class wstring2data_sso
// ******************************************************************
{
public:     // Typedefs
    typedef wchar               value_type;
    typedef value_type          E;
    typedef value_type*         iterator;
    typedef value_type const*   const_iterator;
    
public:
    wstring2data_sso( void )
    :   _begin(_small.begin),
        _size(0)
    {
        _begin[_size] = L'\0';

        ali_assert(is_small());
    }

    wstring2data_sso(
        wstring2data_sso const& b,
        int pos, int n );
    
    explicit wstring2data_sso( wstring_const_ref str );
    
    bool is_empty( void ) const
    {
        return _size == 0;
    }

    int size( void ) const
    {
        return _size;
    }
    
    int capacity( void ) const
    {
        if ( is_small() )
            return _small_capacity;
        return _capacity;
    }
    
    iterator begin( void )
    {
        return _begin;
    }

    friend iterator begin( wstring2data_sso & str )
    {
        return str.begin();
    }
    
    const_iterator begin( void ) const
    {
        return _begin;
    }

    friend const_iterator begin( wstring2data_sso const& str )
    {
        return str.begin();
    }
    
    iterator end( void )
    {
        return _begin + _size;
    }

    friend iterator end( wstring2data_sso & str )
    {
        return str.end();
    }
    
    const_iterator end( void ) const
    {
        return _begin + _size;
    }

    friend const_iterator end( wstring2data_sso const& str )
    {
        return str.end();
    }

    E const& operator[]( int i ) const
    {
        ali_assert(0 <= i);
        ali_assert(i < _size);
        return _begin[i];
    }
    
    E& operator[]( int i )
    {
        ali_assert(0 <= i);
        ali_assert(i < _size);
        return _begin[i];
    }
    
    E const* data( void ) const
    {
        ali_assert(_begin[_size] == L'\0');
        return _begin;
    }
    
    E const* c_str( void ) const
    {
        return data();
    }
    
protected:  // Methods
    ~wstring2data_sso( void )
    {
        release();
        _begin = nullptr;
        _size = 0;
        _capacity = 0;
    }

    void assign(
        wstring2data_sso const& b,
        int pos, int n )
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= b._size);
        ali_assert(0 <= n);
        ali_assert(n <= b._size - pos);

        assign(wstring_const_ref(b.data() + pos, n));
    }
    void assign( wstring_const_ref b );
    void assign( int n, E c );
    
    void reserve( int n )
    {
        ali_assert(0 <= n);

        private_reserve(n, _size);
    }
    
    void resize( int n, E c );
    
    void erase( void )
    {
        _size = 0;

        _begin[_size] = L'\0';
    }

    void swap( wstring2data_sso& b );

    void ensure_unique( void ) {}

    is_internal_pointer_result is_internal_pointer( E const* str, int n ) const
    {
        ali_assert(0 <= n);

        bool const is_internal = _begin <= str && str <= _begin + _size;
        
        ali_assert(!is_internal || n <= _size - (str - _begin));
        
        return is_internal_pointer_result{
            is_internal ? static_cast<int>(str - _begin) : -1};
    }

    is_internal_pointer_result is_internal_pointer( wstring_const_ref str ) const
    {
        return is_internal_pointer(str.data(), str.size());
    }

    is_internal_pointer_result is_internal_pointer( wstring_const_ptr str ) const
    {
        return is_internal_pointer(str->data(), str->size());
    }
    
    void set( int pos, wstring_const_ref str )
    {
        wstring_ref(_begin, _size).copy(pos, str);
    }

    void set( int pos, int n, E c )
    {
        wstring_ref(_begin, _size).mutable_ref(pos, n).fill(c);
    }

    void move( int first1, int first2, int n )
    {
        wstring_ref self(_begin, _size);

        self.mutable_ref(first1, n) = self.ref(first2, n);
    }
    
private:    // Methods
    wstring2data_sso& operator=(
        wstring2data_sso const& b );

    bool is_small( void ) const
    {
        return _begin == _small.begin;
    }

    void release( void );

    void private_reserve(
        int min_capacity,
        int retained_size );
    
    static void swap_ll(
        wstring2data_sso& a,
        wstring2data_sso& b )
    {
        ali_assert(!a.is_small());
        ali_assert(!b.is_small());
    
        using ali::swap;
        swap(a._begin, b._begin);
        swap(a._size, b._size);
        swap(a._capacity, b._capacity);
    }
    
    static void swap_ss(
        wstring2data_sso& a,
        wstring2data_sso& b )
    {
        ali_assert(a.is_small());
        ali_assert(b.is_small());
    
        using ali::swap;
        swap(a._size, b._size);
        swap_by_copy(a._small, b._small);
    }

    static void swap_ls(
        wstring2data_sso& a,
        wstring2data_sso& b );

private:    // Data members
    static int const    _nice_object_size = 16;
    static int const    _min_object_size
                            = sizeof(E*) + sizeof(unsigned int) + meta::maxi<
                                sizeof(int), sizeof(E)>::result;
    static int const    _object_size
                            = meta::maxi<
                                _min_object_size,
                                _nice_object_size>::result;
    static int const    _small_capacity
                            = (_object_size - sizeof(E*) - sizeof(unsigned int))
                                / sizeof(E) - 1;

    struct small_data
    {
        E                   begin[_small_capacity + 1];
    };

    union
    {
        int                 _capacity;
        small_data          _small;
    };
    E*                      _begin;
    int                     _size;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class wstring2data_sso_rc2
// ******************************************************************
{
public:     // Typedefs
    typedef wchar               value_type;
    typedef value_type          E;
    typedef value_type*         iterator;
    typedef value_type const*   const_iterator;
    
public:
    wstring2data_sso_rc2( void )
    :   _begin(_small.begin),
        _size(0)
    {
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(init_backtraces());
        _begin[_size] = L'\0';

        ali_assert(is_small());
    }

    wstring2data_sso_rc2(
        wstring2data_sso_rc2 const& b,
        int pos, int n );
    
    explicit wstring2data_sso_rc2( wstring_const_ref str );
    
    bool is_empty( void ) const
    {
        return _size == 0;
    }

    int size( void ) const
    {
        return _size;
    }
    
    int capacity( void ) const
    {
        if ( is_small() )
            return _small_capacity;
        return _large->capacity;
    }
    
    iterator begin( void )
    {
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(check_for_null_dereference());
        make_non_sharable();
        return _begin;
    }

    friend iterator begin( wstring2data_sso_rc2 & str )
    {
        return str.begin();
    }
    
    const_iterator begin( void ) const
    {
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(check_for_null_dereference());
        return _begin;
    }

    friend const_iterator begin( wstring2data_sso_rc2 const& str )
    {
        return str.begin();
    }
    
    iterator end( void )
    {
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(check_for_null_dereference());
        make_non_sharable();
        return _begin + _size;
    }

    friend iterator end( wstring2data_sso_rc2 & str )
    {
        return str.end();
    }
    
    const_iterator end( void ) const
    {
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(check_for_null_dereference());
        return _begin + _size;
    }

    friend const_iterator end( wstring2data_sso_rc2 const& str )
    {
        return str.end();
    }

    E const& operator[]( int i ) const
    {
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(check_for_null_dereference());
        ali_assert(0 <= i);
        ali_assert(i < _size);
        return _begin[i];
    }
    
    E& operator[]( int i )
    {
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(check_for_null_dereference());
        ali_assert(0 <= i);
        ali_assert(i < _size);
        make_non_sharable();
        return _begin[i];
    }
    
    E const* data( void ) const
    {
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(check_for_null_dereference());
        ali_assert(_begin[_size] == '\0');
        return _begin;
    }
    
    E const* c_str( void ) const
    {
        return data();
    }
    
protected:  // Methods
    ~wstring2data_sso_rc2( void )
    {
        release();
        _begin = nullptr;
        _size = 0;
    }

    void assign(
        wstring2data_sso_rc2 const& b,
        int pos, int n );
    void assign( wstring_const_ref b );
    void assign( int n, E c );
    
    void reserve( int n )
    {
        ali_assert(0 <= n);

        private_reserve(n, _size, false);

        make_sharable();
    }
    
    void resize( int n, E c );

    void erase( void );
    
    void swap( wstring2data_sso_rc2& b );

    void ensure_unique( void )
    {
        private_reserve(_size, _size, true);
    }

    is_internal_pointer_result is_internal_pointer( E const* str, int n ) const
    {
        ali_assert(0 <= n);

        bool const is_internal = _begin <= str && str <= _begin + _size;
        
        ali_assert(!is_internal || n <= _size - (str - _begin));
        
        return is_internal_pointer_result{
            is_internal ? static_cast<int>(str - _begin) : -1};
    }

    is_internal_pointer_result is_internal_pointer( wstring_const_ref str ) const
    {
        return is_internal_pointer(str.data(), str.size());
    }

    is_internal_pointer_result is_internal_pointer( wstring_const_ptr str ) const
    {
        return is_internal_pointer(str->data(), str->size());
    }
    
    void set( int pos, wstring_const_ref str )
    {
        ensure_unique();

        ali_assert(is_small() || _large->rc < 2);

        wstring_ref(_begin, _size).copy(pos, str);
    }

    void set( int pos, int n, E c )
    {
        ensure_unique();

        ali_assert(is_small() || _large->rc < 2);

        wstring_ref(_begin, _size).mutable_ref(pos, n).fill(c);
    }

    void move( int first1, int first2, int n )
    {
        ensure_unique();

        ali_assert(is_small() || _large->rc < 2);

        wstring_ref self(_begin, _size);

        self.mutable_ref(first1, n) = self.ref(first2, n);
    }

ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(
    void check_for_null_dereference( void ) const;
    void init_backtraces( void );
);

private:    //  Struct
    struct large_header
    {
        int rc;
        int capacity;
    };

    ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(
        enum { BacktraceItems = 20 });
    
private:    //  Methods
    wstring2data_sso_rc2& operator=(
        wstring2data_sso_rc2 const& b );

    bool is_small( void ) const
    {
        return _begin == _small.begin;
    }

    bool is_sharable( void ) const
    {
        ali_assert(!is_small());
        return _large->rc != 0;
    }

    void make_non_sharable( void )
    {
        ensure_unique();

        ali_assert(is_small() || _large->rc < 2);

        if ( !is_small() )
            _large->rc = 0;
    }
    
    void make_sharable( void )
    {
        if ( !is_small() && _large->rc == 0 )
            ++_large->rc;
    }

    void release( large_header* large );

    void release( void );

    void private_reserve(
        int min_capacity,
        int retained_size,
        bool ensure_unique );

    static void swap_ll(
        wstring2data_sso_rc2& a,
        wstring2data_sso_rc2& b )
    {
        ali_assert(!a.is_small());
        ali_assert(!b.is_small());
    
        using ali::swap;
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(
            ali::swap(a._created, b._created);
            ali::swap(a._assigned, b._assigned));
        swap(a._begin, b._begin);
        swap(a._size, b._size);
        swap(a._large, b._large);
    }
    
    static void swap_ss(
        wstring2data_sso_rc2& a,
        wstring2data_sso_rc2& b )
    {
        ali_assert(a.is_small());
        ali_assert(b.is_small());
    
        using ali::swap;
        ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(
            ali::swap(a._created, b._created);
            ali::swap(a._assigned, b._assigned));
        swap(a._size, b._size);
        swap_by_copy(a._small, b._small);
    }

    static void swap_ls(
        wstring2data_sso_rc2& a,
        wstring2data_sso_rc2& b );

private:    // Data members
    static int const    _nice_object_size = 16;
    static int const    _min_object_size
                            = sizeof(E*) + sizeof(unsigned int) + meta::maxi<
                                sizeof(large_header*), sizeof(E)>::result;
    static int const    _object_size
                            = meta::maxi<
                                _min_object_size,
                                _nice_object_size>::result;
    static int const    _small_capacity
                            = (_object_size - sizeof(E*) - sizeof(unsigned int))
                                / sizeof(E) - 1;

    struct small_data
    {
        E               begin[_small_capacity + 1];
    };

    union
    {
        large_header*   _large;
        small_data      _small;
    };
    E*                  _begin;
    int                 _size;
    ALI_STRING2_IF_DEBUG_NULL_DEREFERENCE(
        void*                           _created[BacktraceItems];
        void*                           _assigned[BacktraceItems];
        void*                           _destroyed[BacktraceItems]);
};

// ******************************************************************
// ******************************************************************

#ifdef  ALI_STRING2_COPY_ON_WRITE

// ******************************************************************
typedef string2data_sso_rc2 string2data;
typedef wstring2data_sso_rc2 wstring2data;
// ******************************************************************

#else   //  !ALI_STRING2_COPY_ON_WRITE

// ******************************************************************
typedef string2data_sso string2data;
typedef wstring2data_sso wstring2data;
// ******************************************************************

#endif  //  ALI_STRING2_COPY_ON_WRITE

// ******************************************************************
// ******************************************************************

// ******************************************************************
class string2 : public string2data
// ******************************************************************
{
public:
    static int const    npos = ali::meta::integer::max_value<
                            int>::result;

public:
    string2( void )
    {}

#if defined(ALI_FEATURE_RVALUE_REF)
    string2( string2&& b )
    {
        swap(b);
    }
#endif
    
    string2( string2 const& b, int pos = 0, int n = npos )
    :   string2data(b, pos, n)
    {}

    string2( E const* str )
    :   string2data(string_const_ref(str))
    {}
    
    string2( E const* str, int n )
    :   string2data(string_const_ref(str, ali::maxi(0, n)))
    {
        ali_assert(0 <= n);
    }

    string2( string_const_ref str )
    :   string2data(str)
    {}
    
    string2& operator=( string2 const& b )
    {
        return assign(b);
    }
    
    string2& operator=( E const* b )
    {
        return assign(b);
    }

    string2& operator=( string_const_ref b )
    {
        return assign(b);
    }

ALI_DEPRECATED(
    int length( void ) const
    {
        return size();
    }
);

    string2& assign(
        string2 const& b,
        int pos = 0,
        int n = npos );
    
    string2& assign( E const* b, int n );
    
    string2& assign( E const* b )
    {
        return assign(string_const_ref(b));
    }

    string2& assign( string_const_ref b )
    {
        string2data::assign(b);
        return *this;
    }
    
    string2& assign( int n, E c );
    
    string2& reserve( int n );
    
    string2& resize( int n, E c );
    
    string2& resize( int n )
    {
        return resize(n, E());
    }
    
    E const& at( int i ) const
    {
        check_range(i, 0, size());
        return operator[](i);
    }
    
    E& at( int i )
    {
        check_range(i, 0, size());
        return operator[](i);
    }
/*
    string2& operator+=( string2 const& str )
    {
        return append(str);
    }
*/    
    string2& operator+=( E const* str )
    {
        return append(str);
    }

    string2& operator+=( string_const_ref str )
    {
        return append(str);
    }
    
    string2& operator+=( E c )
    {
        return append(1, c);
    }

    // append
    string2& append(
        string2 const& str,
        int pos,
        int n = npos );
/*
    string2& append( E const* str )
    {
        return append(string_const_ref(str));
    }
*/
    string2& append( E const* str, int n );

    string2& append( string_const_ref str );
    
    string2& append( int n, E c );

    string2& push_back( E c )
	{
		return this->append(1, c);
	}

    string2& push_front( E c )
	{
		return this->insert(0, 1, c);
	}

    // replace
/*
    string2& replace(
        int pos1,
        int n1,
        string2 const& str )
    {
        return replace(pos1, n1, str, 0, npos);
    }
*/
    string2& replace(
        int pos1, int n1,
        string2 const& str,
        int pos2, int n2 );

    string2& replace( int pos1, int n1, E const* str )
    {
        return replace(pos1, n1, string_const_ref(str));
    }

    string2& replace(
        int pos1, int n1,
        E const* str, int n2 );

    string2& replace(
        int pos1, int n1,
        string_const_ref str );

    string2& replace(
        int pos1, int n1,
        int n2, E c );
    
    string2& replace(
        string_const_ref src,
        string_const_ref target );

    string2& nocase_replace(
        string_const_ref src,
        string_const_ref target );

    // erase
    string2& erase( int pos, int n = npos );
    
    string2& erase( void )
    {
        string2data::erase();
        return *this;
    }

    // insert
/*
    string2& insert( int pos1, string2 const& str )
    {
        return insert(pos1, str.ref());
    }
*/
    string2& insert(
        int pos1, string2 const& str,
        int pos2, int n );

    string2& insert( int pos1, E const* str, int n );

    string2& insert( int pos1, E const* str )
    {
        return insert(pos1, string_const_ref(str));
    }

    string2& insert( int pos1, string_const_ref str );

    string2& insert( int pos1, int n, E c );

    //  transform

    template <typename char_transform_in_place>
    string2& transform_chars_in_place( char_transform_in_place t )
    {
        string_ref(begin(), size()).for_each(t);
        return *this;
    }

    //  misc

    void swap( string2& b )
    {
        string2data::swap(b);
    }

    friend void swap( string2& a, string2& b )
    {
        a.swap(b);
    }

    string2& ensure_unique( void )
    {
        string2data::ensure_unique();
        return *this;
    }

    //  utility
    string2 substr( int pos = 0, int n = npos ) const
    {
        return string2(*this, pos, n);
    }

    bool begins_with( E const* prefix, int n ) const;

/*
    bool begins_with( string2 const& prefix ) const
    {
        return ref().begins_with(prefix.ref());
    }

    bool begins_with( E const* prefix ) const
    {
        return ref().begins_with(string_const_ref(prefix));
    }
*/
    bool begins_with( string_const_ref prefix ) const
    {
        return ref().begins_with_n(prefix);
    }

	bool begins_with( E prefix ) const
	{
		return ref().begins_with(prefix);
	}

    bool nocase_begins_with( E const* prefix, int n ) const;

/*
    bool nocase_begins_with( string2 const& prefix ) const
    {
        return ref().nocase_begins_with(prefix.ref());
    }

    bool nocase_begins_with( E const* prefix ) const
    {
        return ref().nocase_begins_with(string_const_ref(prefix));
    }
*/
    bool nocase_begins_with( string_const_ref prefix ) const
    {
        return ref().nocase_begins_with_n(prefix);
    }

	bool nocase_begins_with( E prefix ) const
    {
        return ref().nocase_begins_with(prefix);
    }

    bool ends_with( E const* suffix, int n ) const;

/*
    bool ends_with( string2 const& suffix ) const
    {
        return ref().ends_with(suffix.ref());
    }

    bool ends_with( E const* suffix ) const
    {
        return ref().ends_with(string_const_ref(suffix));
    }
*/
    bool ends_with( string_const_ref suffix ) const
    {
        return ref().ends_with_n(suffix);
    }

	bool ends_with( E suffix ) const
	{
		return ref().ends_with(suffix);
	}

    bool nocase_ends_with( E const* suffix, int n ) const;
/*
    bool nocase_ends_with( string2 const& suffix ) const
    {
        return ref().nocase_ends_with(suffix.ref());
    }

    bool nocase_ends_with( E const* suffix ) const
    {
        return ref().nocase_ends_with(string_const_ref(suffix));
    }
*/
    bool nocase_ends_with( string_const_ref suffix ) const
    {
        return ref().nocase_ends_with_n(suffix);
    }

	bool nocase_ends_with( E suffix ) const
    {
        return ref().nocase_ends_with(suffix);
    }

    int compare( string2 const& b ) const
    {
        return ref().compare(b.ref());
    }

    int compare(
        int pos1, int n1,
        string2 const& b ) const
    {
        return this->compare(pos1, n1, b.ref());
    }

    int compare(
        int pos1, int n1,
        string2 const& b,
        int pos2, int n2 ) const;

    int compare( E const* str ) const
    {
        return ref().compare(string_const_ref(str));
    }

    int compare( string_const_ref str ) const
    {
        return ref().compare(str);
    }

    int compare( int pos1, int n1, E const* str ) const
    {
        return this->compare(pos1, n1, string_const_ref(str));
    }

    int compare(
        int pos1, int n1,
        string_const_ref str ) const;

    int compare(
        int pos1, int n1,
        E const* str, int n2 ) const;

    int nocase_compare( string2 const& b ) const
    {
        return ref().nocase_compare(b.ref());
    }

    int nocase_compare(
        int pos1, int n1,
        string2 const& b ) const
    {
        return this->nocase_compare(pos1, n1, b.ref());
    }

    int nocase_compare(
        int pos1, int n1,
        string2 const& b,
        int pos2, int n2 ) const;

    int nocase_compare( E const* str ) const
    {
        return ref().nocase_compare(string_const_ref(str));
    }

    int nocase_compare( string_const_ref str ) const
    {
        return ref().nocase_compare(str);
    }

    int nocase_compare(
        int pos1,
        int n1,
        E const* str ) const
    {
        return this->nocase_compare(pos1, n1, string_const_ref(str));
    }

    int nocase_compare(
        int pos1, int n1,
        string_const_ref str ) const;

    int nocase_compare(
        int pos1, int n1,
        E const* str, int n2 ) const;

/*
    int find( string2 const& str, int pos1 = 0 ) const
    {
        return this->find(str.ref(), pos1);
    }

    int find( E const* str, int pos1 = 0 ) const
    {
        return this->find(string_const_ref(str), pos1);
    }
*/
    int find( string_const_ref str, int pos1 = 0 ) const;
    int find( E const* str, int pos1, int n2 ) const;
    int find( E c, int pos1 = 0 ) const;

/*
    int nocase_find( string2 const& str, int pos1 = 0 ) const
    {
        return this->nocase_find(str.ref(), pos1);
    }

    int nocase_find( E const* str, int pos1 = 0 ) const
    {
        return this->nocase_find(string_const_ref(str), pos1);
    }
*/
    int nocase_find( string_const_ref str, int pos1 = 0 ) const;
    int nocase_find( E const* str, int pos1, int n2 ) const;
    int nocase_find( E c, int pos1 = 0 ) const;

/*
    int rfind( string2 const& str, int pos1 = npos ) const
    {
        return this->rfind(str.ref(), pos1);
    }

    int rfind( E const* str, int pos1 = npos ) const
    {
        return this->rfind(string_const_ref(str), pos1);
    }
*/
    int rfind( string_const_ref str, int pos1 = npos ) const;
    int rfind( E const* str, int pos1, int n2 ) const;
    int rfind( E c, int pos1 = npos ) const;
        //  post:   result == npos  //  Not found.
        //      ||  (0 <= result && result <= pos1)
/*
    int nocase_rfind( string2 const& str, int pos1 = npos ) const
    {
        return this->nocase_rfind(str.ref(), pos1);
    }

    int nocase_rfind( E const* str, int pos1 = npos ) const
    {
        return this->nocase_rfind(string_const_ref(str), pos1);
    }
*/
    int nocase_rfind( string_const_ref str, int pos1 = npos ) const;
    int nocase_rfind( E const* str, int pos1, int n2 ) const;
    int nocase_rfind( E c, int pos1 = npos ) const;
        //  post:   result == npos  //  Not found.
        //      ||  (0 <= result && result <= pos1)
    
    int find_first_of( string_const_ref set, int pos1 = 0 ) const;
    int find_first_not_of( string_const_ref set, int pos1 = 0 ) const;
    int find_last_not_of( string_const_ref set, int pos1 = npos ) const;
    
    // deprecated
    int find_any( string_const_ref set, int pos1 = 0 ) const
    {
        return find_first_of(set, pos1);
    }
    
    // deprecated
    int find_neither( string_const_ref set, int pos1 = 0 ) const
    {
        return find_first_not_of(set, pos1);
    }

    //  pointer
    
    string_const_ptr pointer( int pos, int n ) const
    {
        return pointer()->pointer(pos, n);
    }

    c_string_const_ptr pointer( void ) const
    {
        return c_string_const_ptr(c_str(), size());
    }

    string_const_ptr pointer_left( int pos ) const
    {
        return pointer()->pointer_left(pos);
    }
    
    c_string_const_ptr pointer_right( int pos ) const
    {
        return pointer()->pointer_right(pos);
    }

    string_const_ptr pointer_front( int n ) const
    {
        return pointer()->pointer_front(n);
    }

    c_string_const_ptr pointer_back( int n ) const
    {
        return pointer()->pointer_back(n);
    }

    //  mutable_pointer

    string_ptr mutable_pointer( int pos, int n )
    {
        return mutable_pointer()->mutable_pointer(pos, n);
    }

    string_ptr mutable_pointer( void )
    {
        return string_ptr(begin(), size());
    }

    string_ptr mutable_pointer_left( int pos )
    {
        return mutable_pointer()->mutable_pointer_left(pos);
    }
    
    string_ptr mutable_pointer_right( int pos )
    {
        return mutable_pointer()->mutable_pointer_right(pos);
    }

    string_ptr mutable_pointer_front( int n )
    {
        return mutable_pointer()->mutable_pointer_front(n);
    }

    string_ptr mutable_pointer_back( int n )
    {
        return mutable_pointer()->mutable_pointer_back(n);
    }

    //  ref

    string_const_ref ref( int pos, int n ) const
    {
        return ref().ref(pos, n);
    }

    c_string_const_ref ref( void ) const
    {
        return *this;
    }

    string_const_ref ref_left( int pos ) const
    {
        return ref().ref_left(pos);
    }
    
    c_string_const_ref ref_right( int pos ) const
    {
        return ref().ref_right(pos);
    }

    string_const_ref ref_front( int n ) const
    {
        return ref().ref_front(n);
    }

    c_string_const_ref ref_back( int n ) const
    {
        return ref().ref_back(n);
    }

    //  mutable_ref

    string_ref mutable_ref( int pos, int n )
    {
        return mutable_ref().mutable_ref(pos, n);
    }

    string_ref mutable_ref( void )
    {
        return *this;
    }

    string_ref mutable_ref_left( int pos )
    {
        return mutable_ref().mutable_ref_left(pos);
    }

    string_ref mutable_ref_right( int pos )
    {
        return mutable_ref().mutable_ref_right(pos);
    }

    string_ref mutable_ref_front( int n )
    {
        return mutable_ref().mutable_ref_front(n);
    }

    string_ref mutable_ref_back( int n )
    {
        return mutable_ref().mutable_ref_back(n);
    }

    //  conversion operators

    operator c_string_const_ref( void ) const
    {
        return c_string_const_ref(c_str(), size());
    }

    operator c_string_const_ref( void )
    {
        return c_string_const_ref(c_str(), size());
    }

    operator string_const_ref( void ) const
    {
        return string_const_ref(c_str(), size());
    }

    operator string_const_ref( void )
    {
        //return string_const_ref(begin(), size());
            //  The above calls non-const version of begin
            //  which is not what we want in this case.
        return string_const_ref(c_str(), size());
    }

    operator string_ref( void )
    {
        return string_ref(begin(), size());
    }

    blob_const_ref as_blob( void ) const
    {
        return ref().as_blob();
    }

    blob_ref as_mutable_blob( void )
    {
        return mutable_ref().as_mutable_blob();
    }

private:    // Methods
    static void check_range( int cur, int b, int e );

	//  If the compiler complains about this method being private, you have
    //  an error in your program's logic. Maybe you are using ali::optional<>
    //  directly instead of using operator *()?
	string2& operator +=(bool);
};


// ******************************************************************
class wstring2 : public wstring2data
// ******************************************************************
{
public:
    static int const    npos = ali::meta::integer::max_value<
                            int>::result;

public:
    wstring2( void )
    {}

#if defined(ALI_FEATURE_RVALUE_REF)
    // ******************************************************************
    wstring2(wstring2 && b)
    // ******************************************************************
    {
        swap(b);
    }
#endif
    
    wstring2( wstring2 const& b, int pos = 0, int n = npos )
    :   wstring2data(b, pos, n)
    {}

    wstring2( E const* str )
    :   wstring2data(wstring_const_ref(str))
    {}
    
    wstring2( E const* str, int n )
    :   wstring2data(wstring_const_ref(str, ali::maxi(0, n)))
    {
        ali_assert(0 <= n);
    }

    wstring2( wstring_const_ref str )
    :   wstring2data(str)
    {}
    
    wstring2& operator=( wstring2 const& b )
    {
        return assign(b);
    }
    
    wstring2& operator=( E const* b )
    {
        return assign(b);
    }

    wstring2& operator=( wstring_const_ref b )
    {
        return assign(b);
    }
    
ALI_DEPRECATED(
    int length( void ) const
    {
        return size();
    }
);

    wstring2& assign(
        wstring2 const& b,
        int pos = 0,
        int n = npos );
    
    wstring2& assign( E const* b, int n );
    
    wstring2& assign( E const* b )
    {
        return assign(wstring_const_ref(b));
    }

    wstring2& assign( wstring_const_ref b )
    {
        wstring2data::assign(b);
        return *this;
    }
    
    wstring2& assign( int n, E c );
    
    wstring2& reserve( int n );
    
    wstring2& resize( int n, E c );
    
    wstring2& resize( int n )
    {
        return resize(n, E());
    }
    
    E const& at( int i ) const
    {
        check_range(i, 0, size());
        return operator[](i);
    }
    
    E& at( int i )
    {
        check_range(i, 0, size());
        return operator[](i);
    }
/*
    wstring2& operator+=( wstring2 const& str )
    {
        return append(str);
    }
*/    
    wstring2& operator+=( E const* str )
    {
        return append(str);
    }

    wstring2& operator+=( wstring_const_ref str )
    {
        return append(str);
    }
    
    wstring2& operator+=( E c )
    {
        return append(1, c);
    }

    // append
    wstring2& append(
        wstring2 const& str,
        int pos,
        int n = npos );
/*
    wstring2& append( E const* str )
    {
        return append(wstring_const_ref(str));
    }
*/
    wstring2& append( E const* str, int n );

    wstring2& append( wstring_const_ref str );
    
    wstring2& append( int n, E c );

    wstring2& push_back( E c )
	{
		return this->append(1, c);
	}

    wstring2& push_front( E c )
	{
		return this->insert(0, 1, c);
	}

    // replace
/*
    wstring2& replace(
        int pos1,
        int n1,
        wstring2 const& str )
    {
        return replace(pos1, n1, str, 0, npos);
    }
*/
    wstring2& replace(
        int pos1, int n1,
        wstring2 const& str,
        int pos2, int n2 );

    wstring2& replace( int pos1, int n1, E const* str )
    {
        return replace(pos1, n1, wstring_const_ref(str));
    }

    wstring2& replace(
        int pos1, int n1,
        E const* str, int n2 );

    wstring2& replace(
        int pos1, int n1,
        wstring_const_ref str );

    wstring2& replace(
        int pos1, int n1,
        int n2, E c );
    
    wstring2& replace(
        wstring_const_ref src,
        wstring_const_ref target );

    wstring2& nocase_replace(
        wstring_const_ref src,
        wstring_const_ref target );

    // erase
    wstring2& erase( int pos, int n = npos );
    
    wstring2& erase( void )
    {
        wstring2data::erase();
        return *this;
    }

    // insert
/*
    wstring2& insert( int pos1, wstring2 const& str )
    {
        return insert(pos1, str.ref());
    }
*/
    wstring2& insert(
        int pos1, wstring2 const& str,
        int pos2, int n );

    wstring2& insert( int pos1, E const* str, int n );

    wstring2& insert( int pos1, E const* str )
    {
        return insert(pos1, wstring_const_ref(str));
    }

    wstring2& insert( int pos1, wstring_const_ref str );

    wstring2& insert( int pos1, int n, E c );

    //  transform

    template <typename char_transform_in_place>
    wstring2& transform_chars_in_place( char_transform_in_place t )
    {
        wstring_ref(begin(), size()).for_each(t);
        return *this;
    }

    //  misc

    void swap( wstring2& b )
    {
        wstring2data::swap(b);
    }

    friend void swap( wstring2& a, wstring2& b )
    {
        a.swap(b);
    }

    wstring2& ensure_unique( void )
    {
        wstring2data::ensure_unique();
        return *this;
    }

    //  utility
    wstring2 substr( int pos = 0, int n = npos ) const
    {
        return wstring2(*this, pos, n);
    }

    bool begins_with( E const* prefix, int n ) const;

/*
    bool begins_with( wstring2 const& prefix ) const
    {
        return ref().begins_with(prefix.ref());
    }

    bool begins_with( E const* prefix ) const
    {
        return ref().begins_with(wstring_const_ref(prefix));
    }
*/
    bool begins_with( wstring_const_ref prefix ) const
    {
        return ref().begins_with_n(prefix);
    }

	bool begins_with( E prefix ) const
	{
		return ref().begins_with(prefix);
	}

    bool nocase_begins_with( E const* prefix, int n ) const;

/*
    bool nocase_begins_with( wstring2 const& prefix ) const
    {
        return ref().nocase_begins_with(prefix.ref());
    }

    bool nocase_begins_with( E const* prefix ) const
    {
        return ref().nocase_begins_with(wstring_const_ref(prefix));
    }
*/
    bool nocase_begins_with( wstring_const_ref prefix ) const
    {
        return ref().nocase_begins_with_n(prefix);
    }

	bool nocase_begins_with( E prefix ) const
    {
        return ref().nocase_begins_with(prefix);
    }

    bool ends_with( E const* suffix, int n ) const;

/*
    bool ends_with( wstring2 const& suffix ) const
    {
        return ref().ends_with(suffix.ref());
    }

    bool ends_with( E const* suffix ) const
    {
        return ref().ends_with(wstring_const_ref(suffix));
    }
*/
    bool ends_with( wstring_const_ref suffix ) const
    {
        return ref().ends_with_n(suffix);
    }

	bool ends_with( E suffix ) const
	{
		return ref().ends_with(suffix);
	}

    bool nocase_ends_with( E const* suffix, int n ) const;
/*
    bool nocase_ends_with( wstring2 const& suffix ) const
    {
        return ref().nocase_ends_with(suffix.ref());
    }

    bool nocase_ends_with( E const* suffix ) const
    {
        return ref().nocase_ends_with(wstring_const_ref(suffix));
    }
*/
    bool nocase_ends_with( wstring_const_ref suffix ) const
    {
        return ref().nocase_ends_with_n(suffix);
    }

	bool nocase_ends_with( E suffix ) const
    {
        return ref().nocase_ends_with(suffix);
    }

    int compare( wstring2 const& b ) const
    {
        return ref().compare(b.ref());
    }

    int compare(
        int pos1, int n1,
        wstring2 const& b ) const
    {
        return this->compare(pos1, n1, b.ref());
    }

    int compare(
        int pos1, int n1,
        wstring2 const& b,
        int pos2, int n2 ) const;

    int compare( E const* str ) const
    {
        return ref().compare(wstring_const_ref(str));
    }

    int compare( wstring_const_ref str ) const
    {
        return ref().compare(str);
    }

    int compare( int pos1, int n1, E const* str ) const
    {
        return this->compare(pos1, n1, wstring_const_ref(str));
    }

    int compare(
        int pos1, int n1,
        wstring_const_ref str ) const;

    int compare(
        int pos1, int n1,
        E const* str, int n2 ) const;

    int nocase_compare( wstring2 const& b ) const
    {
        return ref().nocase_compare(b.ref());
    }

    int nocase_compare(
        int pos1, int n1,
        wstring2 const& b ) const
    {
        return this->nocase_compare(pos1, n1, b.ref());
    }

    int nocase_compare(
        int pos1, int n1,
        wstring2 const& b,
        int pos2, int n2 ) const;

    int nocase_compare( E const* str ) const
    {
        return ref().nocase_compare(wstring_const_ref(str));
    }

    int nocase_compare( wstring_const_ref str ) const
    {
        return ref().nocase_compare(str);
    }

    int nocase_compare(
        int pos1,
        int n1,
        E const* str ) const
    {
        return this->nocase_compare(pos1, n1, wstring_const_ref(str));
    }

    int nocase_compare(
        int pos1, int n1,
        wstring_const_ref str ) const;

    int nocase_compare(
        int pos1, int n1,
        E const* str, int n2 ) const;

/*
    int find( wstring2 const& str, int pos1 = 0 ) const
    {
        return this->find(str.ref(), pos1);
    }

    int find( E const* str, int pos1 = 0 ) const
    {
        return this->find(wstring_const_ref(str), pos1);
    }
*/
    int find( wstring_const_ref str, int pos1 = 0 ) const;
    int find( E const* str, int pos1, int n2 ) const;
    int find( E c, int pos1 = 0 ) const;

/*
    int nocase_find( wstring2 const& str, int pos1 = 0 ) const
    {
        return this->nocase_find(str.ref(), pos1);
    }

    int nocase_find( E const* str, int pos1 = 0 ) const
    {
        return this->nocase_find(wstring_const_ref(str), pos1);
    }
*/
    int nocase_find( wstring_const_ref str, int pos1 = 0 ) const;
    int nocase_find( E const* str, int pos1, int n2 ) const;
    int nocase_find( E c, int pos1 = 0 ) const;

/*
    int rfind( wstring2 const& str, int pos1 = npos ) const
    {
        return this->rfind(str.ref(), pos1);
    }

    int rfind( E const* str, int pos1 = npos ) const
    {
        return this->rfind(wstring_const_ref(str), pos1);
    }
*/
    int rfind( wstring_const_ref str, int pos1 = npos ) const;
    int rfind( E const* str, int pos1, int n2 ) const;
    int rfind( E c, int pos1 = npos ) const;
        //  post:   result == npos  //  Not found.
        //      ||  (0 <= result && result <= pos1)

/*
    int nocase_rfind( wstring2 const& str, int pos1 = npos ) const
    {
        return this->nocase_rfind(str.ref(), pos1);
    }

    int nocase_rfind( E const* str, int pos1 = npos ) const
    {
        return this->nocase_rfind(wstring_const_ref(str), pos1);
    }
*/
    int nocase_rfind( wstring_const_ref str, int pos1 = npos ) const;
    int nocase_rfind( E const* str, int pos1, int n2 ) const;
    int nocase_rfind( E c, int pos1 = npos ) const;
        //  post:   result == npos  //  Not found.
        //      ||  (0 <= result && result <= pos1)
    
    int find_first_of( wstring_const_ref set, int pos1 = 0 ) const;
    int find_first_not_of( wstring_const_ref set, int pos1 = 0 ) const;
    int find_last_not_of( wstring_const_ref set, int pos1 = npos ) const;
    
    // deprecated
    int find_any( wstring_const_ref set, int pos1 = 0 ) const
    {
        return find_first_of(set, pos1);
    }
    
    // deprecated
    int find_neither( wstring_const_ref set, int pos1 = 0 ) const
    {
        return find_first_not_of(set, pos1);
    }

    //  pointer
    
    wstring_const_ptr pointer( int pos, int n ) const
    {
        return pointer()->pointer(pos, n);
    }

    c_wstring_const_ptr pointer( void ) const
    {
        return c_wstring_const_ptr(c_str(), size());
    }

    wstring_const_ptr pointer_left( int pos ) const
    {
        return pointer()->pointer_left(pos);
    }
    
    c_wstring_const_ptr pointer_right( int pos ) const
    {
        return pointer()->pointer_right(pos);
    }

    wstring_const_ptr pointer_front( int n ) const
    {
        return pointer()->pointer_front(n);
    }

    c_wstring_const_ptr pointer_back( int n ) const
    {
        return pointer()->pointer_back(n);
    }

    //  mutable_pointer

    wstring_ptr mutable_pointer( int pos, int n )
    {
        return mutable_pointer()->mutable_pointer(pos, n);
    }

    wstring_ptr mutable_pointer( void )
    {
        return wstring_ptr(begin(), size());
    }

    wstring_ptr mutable_pointer_left( int pos )
    {
        return mutable_pointer()->mutable_pointer_left(pos);
    }
    
    wstring_ptr mutable_pointer_right( int pos )
    {
        return mutable_pointer()->mutable_pointer_right(pos);
    }

    wstring_ptr mutable_pointer_front( int n )
    {
        return mutable_pointer()->mutable_pointer_front(n);
    }

    wstring_ptr mutable_pointer_back( int n )
    {
        return mutable_pointer()->mutable_pointer_back(n);
    }

    //  ref

    wstring_const_ref ref( int pos, int n ) const
    {
        return ref().ref(pos, n);
    }

    c_wstring_const_ref ref( void ) const
    {
        return *this;
    }

    wstring_const_ref ref_left( int pos ) const
    {
        return ref().ref_left(pos);
    }
    
    c_wstring_const_ref ref_right( int pos ) const
    {
        return ref().ref_right(pos);
    }

    wstring_const_ref ref_front( int n ) const
    {
        return ref().ref_front(n);
    }

    c_wstring_const_ref ref_back( int n ) const
    {
        return ref().ref_back(n);
    }

    //  mutable_ref

    wstring_ref mutable_ref( int pos, int n )
    {
        return mutable_ref().mutable_ref(pos, n);
    }

    wstring_ref mutable_ref( void )
    {
        return *this;
    }

    wstring_ref mutable_ref_left( int pos )
    {
        return mutable_ref().mutable_ref_left(pos);
    }
    
    wstring_ref mutable_ref_right( int pos )
    {
        return mutable_ref().mutable_ref_right(pos);
    }

    wstring_ref mutable_ref_front( int n )
    {
        return mutable_ref().mutable_ref_front(n);
    }

    wstring_ref mutable_ref_back( int n )
    {
        return mutable_ref().mutable_ref_back(n);
    }

    //  conversion operators

    operator c_wstring_const_ref( void ) const
    {
        return c_wstring_const_ref(c_str(), size());
    }

    operator c_wstring_const_ref( void )
    {
        return c_wstring_const_ref(c_str(), size());
    }

    operator wstring_const_ref( void ) const
    {
        return wstring_const_ref(c_str(), size());
    }

    operator wstring_const_ref( void )
    {
        //return wstring_const_ref(begin(), size());
            //  The above calls non-const version of begin
            //  which is not what we want in this case.
        return wstring_const_ref(c_str(), size());
    }

    operator wstring_ref( void )
    {
        return wstring_ref(begin(), size());
    }

    blob_const_ref as_blob( void ) const
    {
        return ref().as_blob();
    }

    blob_ref as_mutable_blob( void )
    {
        return mutable_ref().as_mutable_blob();
    }

private:    // Methods
    static void check_range( int cur, int b, int e );

	//  If the compiler complains about this method being private, you have
    //  an error in your program's logic. Maybe you are using ali::optional<>
    //  directly instead of using operator *()?
	wstring2& operator +=(bool);
};

}   //  namespace ali
