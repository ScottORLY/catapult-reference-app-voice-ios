/*
 *  ali_buffer.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_array_utils.h"
#include "ali_auto_ptr.h"
#include "ali_memory.h"
#include "ali_utility.h"

namespace ali
{

// ******************************************************************
template <class T>
class basic_pod_buffer
// ******************************************************************
{
public:
    bool is_empty( void ) const
    {
        return this->_begin == this->_end;
    }

    int capacity( void ) const
    {
        return static_cast<int>(this->_end - this->_begin);
    }
    
    T* begin( void )
    {
        return this->_begin;
    }
    
    T const* begin( void ) const
    {
        return this->_begin;
    }
    
    T* end( void )
    {
        return this->_end;
    }
    
    T const* end( void ) const
    {
        return this->_end;
    }
    
    T& operator[]( int idx )
    {
        return this->_begin[idx];
    }
    
    T const& operator[]( int idx ) const
    {
        return this->_begin[idx];
    }
    
    bool operator==( basic_pod_buffer const& b ) const
    {
        return this->ref() == b.ref();
    }
    
    bool operator!=( basic_pod_buffer const& b ) const
    {
        return !operator==(b);
    }

    operator array_const_ref<T>( void ) const
    {
        return array_const_ref<T>(this->_begin, this->capacity());
    }

    operator array_ref<T>( void )
    {
        return array_ref<T>(this->_begin, this->capacity());
    }

    array_const_ref<T> ref( void ) const
    {
        return *this;
    }

    array_ref<T> mutable_ref( void )
    {
        return *this;
    }

    blob_const_ref as_blob( void ) const
    {
        return blob_const_ref(
            reinterpret_cast<ali::uint8 const*>(this->_begin),
            sizeof(T) * this->capacity());
    }

    blob_ref as_mutable_blob( void )
    {
        return blob_ref(
            reinterpret_cast<ali::uint8*>(this->_begin),
            sizeof(T) * this->capacity());
    }
    
protected:  //  Method
    basic_pod_buffer( void )
    :   _begin(),
        _end()
    {}
    
    explicit basic_pod_buffer( int capacity )
    :   _begin(capacity == 0 ? nullptr : static_cast<T*>(
            allocate(capacity * sizeof(T)))),
        _end(_begin + capacity)
    {}
    
    basic_pod_buffer( T const data[], int capacity )
    :   _begin(capacity == 0 ? nullptr : static_cast<T*>(
            allocate(capacity * sizeof(T)))),
        _end(_begin + capacity)
    {
        this->mutable_ref() = make_ref(data, capacity);
    }
    
    ~basic_pod_buffer( void )
    {
        deallocate(this->_begin);
    }
    
    void swap( basic_pod_buffer& b )
    {
        using ali::swap;
        swap(this->_begin, b._begin);
        swap(this->_end, b._end);
    }

private:    //  Methods
    basic_pod_buffer( basic_pod_buffer const& b );
    basic_pod_buffer& operator=( basic_pod_buffer const& b );

private:    //  Data members
    T*  _begin;
    T*  _end;
};

// ******************************************************************
template <>
inline blob_const_ref basic_pod_buffer<ali::uint8>::as_blob( void ) const
// ******************************************************************
{
    return blob_const_ref(this->_begin, this->capacity());
}

// ******************************************************************
template <>
inline blob_ref basic_pod_buffer<ali::uint8>::as_mutable_blob( void )
// ******************************************************************
{
    return blob_ref(this->_begin, this->capacity());
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <class T>
class pod_buffer : public basic_pod_buffer<T>
// ******************************************************************
{
public:
    pod_buffer( void )
    {}
    
    explicit pod_buffer( int capacity )
    :   basic_pod_buffer<T>(capacity)
    {}
    
    pod_buffer( T const data[], int capacity )
    :   basic_pod_buffer<T>(data, capacity)
    {}
    
    pod_buffer( pod_buffer const& b )
    :   basic_pod_buffer<T>()
    {
        pod_buffer(
            b.begin(),
            b.capacity())
                .swap(*this);
    }
    
    pod_buffer& operator=( pod_buffer const& b )
    {
        if ( this->capacity() == b.capacity() )
        {
            this->mutable_ref() = b.ref();
        }
        else
        {
            pod_buffer(
                b.begin(),
                b.capacity())
                    .swap(*this);
        }
        return *this;
    }

    pod_buffer& set_capacity( int capacity )
    //  post:   this->capacity() == capacity
    //  note:   The contents is NOT guaranteed to be preserved.
    {
        if ( this->capacity() != capacity )
            pod_buffer(capacity).swap(*this);
        return *this;
    }

    pod_buffer& reserve( int capacity )
    //  post:   this->capacity() == max(this->capacity(), capacity)
    //  note:   The contents is NOT guaranteed to be preserved.
    {
        return this->set_capacity(
            ali::maxi(
                this->capacity(),
                capacity));
    }
    
    void swap( pod_buffer& b )
    {
        basic_pod_buffer<T>::swap(b);
    }

    friend void swap(
        pod_buffer& a,
        pod_buffer& b )
    {
        a.swap(b);
    }
};

// ******************************************************************
template <class T>
class sensitive_pod_buffer : public basic_pod_buffer<T>
// ******************************************************************
{
public:
    sensitive_pod_buffer( void )
    {}
    
    explicit sensitive_pod_buffer( int capacity )
    :   basic_pod_buffer<T>(capacity)
    {}
    
    sensitive_pod_buffer( T const data[], int capacity )
    :   basic_pod_buffer<T>(data, capacity)
    {}
    
    sensitive_pod_buffer( sensitive_pod_buffer const& b )
    :   basic_pod_buffer<T>()
    {
        sensitive_pod_buffer(
            b.begin(),
            b.capacity())
                .swap(*this);
    }

    sensitive_pod_buffer( array_const_ref<T> b )
    :   basic_pod_buffer<T>()
    {
        sensitive_pod_buffer(
            b.data(), b.size())
                .swap(*this);
    }
    
    ~sensitive_pod_buffer( void )
    {
        this->mutable_ref().fill();
    }
    
    sensitive_pod_buffer& operator=( sensitive_pod_buffer const& b )
    {
        return this->operator=(b.ref());
    }

    sensitive_pod_buffer& operator=( array_const_ref<T> b )
    {
        if ( this->capacity() == b.size() )
        {
            this->mutable_ref() = b;
        }
        else
        {
            sensitive_pod_buffer(
                b.data(), b.size())
                    .swap(*this);
        }
        return *this;
    }
    
    sensitive_pod_buffer& set_capacity( int capacity )
    //  post:   this->capacity() == capacity
    //  note:   The contents is NOT guaranteed to be preserved.
    {
        if ( this->capacity() != capacity )
            sensitive_pod_buffer(capacity).swap(*this);
        return *this;
    }

    sensitive_pod_buffer& reserve( int capacity )
    //  post:   this->capacity() == max(this->capacity(), capacity)
    //  note:   The contents is NOT guaranteed to be preserved.
    {
        return this->set_capacity(
            ali::maxi(
                this->capacity(),
                capacity));
    }
    
    void swap( sensitive_pod_buffer& b )
    {
        basic_pod_buffer<T>::swap(b);
    }

    friend void swap(
        sensitive_pod_buffer& a,
        sensitive_pod_buffer& b )
    {
        a.swap(b);
    }
};

// ******************************************************************
// ******************************************************************

namespace hidden
{

// ******************************************************************
template <typename T>
inline void buffer_destroy_content(
    array_ref<T> arr, meta::define_bool_result<true> is_pod )
// ******************************************************************
{
    arr.fill();
}

// ******************************************************************
template <typename T>
inline void buffer_destroy_content(
    array_ref<T> arr, meta::define_bool_result<false> is_pod )
// ******************************************************************
{
}

// ******************************************************************
template <typename T>
inline void buffer_destroy_content( array_ref<T> arr )
// ******************************************************************
{
    buffer_destroy_content(arr, meta::is_pod<T>());
}

}   //  namespace hidden

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T, int _capacity = 0>
class buffer
// ******************************************************************
{
public:
    buffer( void )
    {}

    explicit buffer( array_const_ref<T> b )
    {
        this->mutable_ref() = b;
    }

    ~buffer( void )
    {
        hidden::buffer_destroy_content(this->mutable_ref());
    }

    buffer& operator=( array_const_ref<T> b )
    {
        this->mutable_ref() = b;
        return *this;
    }

    static bool is_empty( void )
    {
        return false;
    }

    static int capacity( void )
    {
        return _capacity;
    }

    T const& operator[]( int pos ) const
    {
        ali_assert(0 <= pos);
        ali_assert(pos < _capacity);
        return this->_begin[pos];
    }

    T& operator[]( int pos )
    {
        ali_assert(0 <= pos);
        ali_assert(pos < _capacity);
        return this->_begin[pos];
    }

    T* data( void )
    {
        return this->_begin;
    }

    T const* data( void ) const
    {
        return this->_begin;
    }

    void swap( buffer& b )
    {
        using ali::swap;
        swap(this->_begin, b._begin);
    }

    friend void swap( buffer& a, buffer& b )
    {
        a.swap(b);
    }

    operator array_const_ref<T>( void ) const
    {
        return array_const_ref<T>(this->_begin);
    }

    operator array_ref<T>( void )
    {
        return array_ref<T>(this->_begin, _capacity);
    }

    array_const_ref<T> ref( void ) const
    {
        return *this;
    }

    array_ref<T> mutable_ref( void )
    {
        return *this;
    }

private:    //  Data members
    T   _begin[_capacity];
};

// ******************************************************************
template <typename T>
class buffer<T, 0>
// ******************************************************************
//  Dynamic buffer.
// ******************************************************************
{
public:
    buffer( void )
    :   _capacity(0),
        _begin(nullptr)
    {}

    explicit buffer( int capacity )
    :   _capacity(ali::maxi(0, capacity)),
        _begin(_capacity == 0 ? nullptr
            : out_of_memory_if_null(
                new T[_capacity],
                ALI_HERE))
    {
        ali_assert(0 <= capacity);
    }

    buffer( buffer const& b )
    :   _capacity(0),
        _begin(nullptr)
    {
        buffer(b.ref()).swap(*this);
    }

    explicit buffer( array_const_ref<T> b )
    :   _capacity(0),
        _begin(nullptr)
    {
        buffer temp(b.size());
        
        temp.mutable_ref() = b;
        
        this->swap(temp);
    }

    ~buffer( void )
    {
        hidden::buffer_destroy_content(this->mutable_ref());
        delete[] this->_begin;
        this->_begin = nullptr;
        this->_capacity = 0;
    }

    buffer& operator=( buffer const& b )
    {
        return this->operator=(b.ref());
    }

    buffer& operator=( array_const_ref<T> b )
    {
        if ( this->_capacity == b.size() )
            this->mutable_ref() = b;
        else
            buffer(b).swap(*this);
        return *this;
    }

    bool is_empty( void ) const
    {
        return this->_capacity == 0;
    }

    int capacity( void ) const
    {
        return this->_capacity;
    }

    T const& operator[]( int pos ) const
    {
        ali_assert(0 <= pos);
        ali_assert(pos < this->_capacity);
        return this->_begin[pos];
    }

    T& operator[]( int pos )
    {
        ali_assert(0 <= pos);
        ali_assert(pos < this->_capacity);
        return this->_begin[pos];
    }

    T* data( void )
    {
        return this->_begin;
    }

    T const* data( void ) const
    {
        return this->_begin;
    }

    buffer& lossy_set_capacity( int capacity )
    //  post:   this->capacity() == capacity
    //  note:   The contents is NOT guaranteed to be preserved.
    {
        if ( this->_capacity != capacity )
            buffer(capacity).swap(*this);
        return *this;
    }

    buffer& lossy_reserve( int capacity )
    //  post:   this->capacity() == max(this->capacity(), capacity)
    //  note:   The contents is NOT guaranteed to be preserved.
    {
        return this->lossy_set_capacity(
            ali::maxi(this->_capacity, capacity));
    }

    buffer& lossless_set_capacity( int capacity )
    //  post:   this->capacity() == capacity
    //  note:   The contents is guaranteed to be preserved up to the min(this->capacity(), capacity).
    {
        if ( this->_capacity != capacity )
        {
            buffer temp(capacity);
            temp.mutable_ref().copy_front(*this);
            this->swap(temp);
        }
        return *this;
    }

    buffer& lossless_reserve( int capacity )
    //  post:   this->capacity() == max(this->capacity(), capacity)
    //  note:   The contents is guaranteed to be preserved up to the min(this->capacity(), capacity).
    {
        return this->lossless_set_capacity(
            ali::maxi(this->_capacity, capacity));
    }
    
    void swap( buffer& b )
    {
        using ali::swap;
        swap(_capacity, b._capacity);
        swap(_begin, b._begin);
    }

    friend void swap( buffer& a, buffer& b )
    {
        a.swap(b);
    }

    operator array_const_ref<T>( void ) const
    {
        return array_const_ref<T>(
            this->_begin, this->_capacity);
    }

    operator array_ref<T>( void )
    {
        return array_ref<T>(
            this->_begin, this->_capacity);
    }

    array_const_ref<T> ref( void ) const
    {
        return *this;
    }

    array_ref<T> mutable_ref( void )
    {
        return *this;
    }

private:    //  Data members
    int _capacity;
    T*  _begin;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T, int _capacity = 0>
class circular_buffer
// ******************************************************************
{
public:
    circular_buffer( void )
    :   _read(0),
        _write(0)
    {}

    //  Consumer

    array_ref<T> read_ref( void )
    {
        int const written_size = _write - _read;

        int const no_wrap_size = _capacity - _read;

        return array_ref<T>(
            &_begin[_read],
            ali::mini(written_size, no_wrap_size));
    }

    bool read_ref_size_can_grow( void ) const
    {
        int const written_size = _write - _read;

        int const no_wrap_size = _capacity - _read;

        return written_size < no_wrap_size;
    }

    circular_buffer& consume( int n )
    {
        ali_assert(0 <= n);
        ali_assert(n <= read_ref().size());

        _read += n;

        if ( _read == _capacity )
        {
            _read = 0;
                
            _write -= _capacity;
        }

        return *this;
    }

    bool is_empty( void ) const
    {
        return _write == _read;
    }

    int size( void ) const
    {
        return _write - _read;
    }

    T& operator[]( int i )
    {
        ali_assert(0 <= i);
        ali_assert(i < size());

        return _begin[(_read + i) % _capacity];
    }

    T const& operator[]( int i ) const
    {
        ali_assert(0 <= i);
        ali_assert(i < size());

        return _begin[(_read + i) % _capacity];
    }

    //  Producer

    array_ref<T> write_ref( void )
    {
        int const written_size = _write - _read;

        int const no_wrap_size = _capacity - _write % _capacity;

        return array_ref<T>(
            &_begin[_write % _capacity],
            ali::mini(_capacity - written_size, no_wrap_size));
    }

    bool write_ref_size_can_grow( void ) const
    {
        int const written_size = _write - _read;

        int const no_wrap_size = _capacity - _write % _capacity;

        return _capacity - written_size < no_wrap_size;
    }

    circular_buffer& produce( int n )
    {
        ali_assert(0 <= n);
        ali_assert(n <= write_ref().size());

        _write += n;

        return *this;
    }

    circular_buffer& push_back( T const& t )
    {
        ali_assert(!write_ref().is_empty());

        write_ref().front() = t;
        
        return produce(1);
    }

    circular_buffer& force_push_back( T const& t )
    {
        if ( write_ref().is_empty() )
            consume(1);

        return push_back(t);
    }

    //  Other

    int capacity( void ) const
    {
        return _capacity;
    }

    circular_buffer& erase( void )
    {
        _read = _write = 0;

        return *this;
    }

private:    //  Data members
    int _read;
    int _write;
    T   _begin[_capacity];
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class circular_buffer<T, 0>
// ******************************************************************
{
public:
    circular_buffer( void )
    :   _capacity(0),
        _read(0),
        _write(0)
    {}

    explicit circular_buffer( int capacity )
    :   _capacity(capacity),
        _read(0),
        _write(0)
    {
        if ( _capacity != 0 )
            ali::auto_ptr<T[]>(_capacity)
                .swap(_begin);
    }
    
    circular_buffer( circular_buffer const& b )
    :   _capacity(b._capacity),
        _read(b._read),
        _write(b._write)
    {
        if ( _capacity != 0 )
        {
            ali::auto_ptr<T[]>(_capacity)
                .swap(_begin);

            make_mutable_ref(_begin.get(), _capacity)
                = make_ref(b._begin.get(), _capacity);
        }
    }

    circular_buffer& operator=( circular_buffer const& b )
    {
        if ( this == &b )
            ;   //  NOOP
        else if ( _capacity == b._capacity )
        {
            if ( _capacity != 0 )
            {
                make_mutable_ref(_begin.get(), _capacity)
                    = make_ref(b._begin.get(), _capacity);
            
                _read = b._read;
            
                _write = b._write;
            }
        }
        else
        {
            circular_buffer(b).swap(*this);
        }
        
        return *this;
    }

    //  Consumer

    array_ref<T> read_ref( void )
    {
        ali_assert(_capacity != 0);

        int const written_size = _write - _read;

        int const no_wrap_size = _capacity - _read;

        return array_ref<T>(
            &_begin[_read],
            ali::mini(written_size, no_wrap_size));
    }

    bool read_ref_size_can_grow( void ) const
    {
        ali_assert(_capacity != 0);

        int const written_size = _write - _read;

        int const no_wrap_size = _capacity - _read;

        return written_size < no_wrap_size;
    }

    circular_buffer& consume( int n )
    {
        ali_assert(_capacity != 0);
        ali_assert(0 <= n);
        ali_assert(n <= read_ref().size());

        _read += n;

        if ( _read == _capacity )
        {
            _read = 0;
                
            _write -= _capacity;
        }

        return *this;
    }

    int is_empty( void ) const
    {
        return _write == _read;
    }

    int size( void ) const
    {
        return _write - _read;
    }

    T& operator[]( int i )
    {
        ali_assert(_capacity != 0);
        ali_assert(0 <= i);
        ali_assert(i < size());

        return _begin[(_read + i) % _capacity];
    }

    T const& operator[]( int i ) const
    {
        ali_assert(_capacity != 0);
        ali_assert(0 <= i);
        ali_assert(i < size());

        return _begin[(_read + i) % _capacity];
    }

    //  Producer

    array_ref<T> write_ref( void )
    {
        ali_assert(_capacity != 0);

        int const written_size = _write - _read;

        int const no_wrap_size = _capacity - _write % _capacity;

        return array_ref<T>(
            &_begin[_write % _capacity],
            ali::mini(_capacity - written_size, no_wrap_size));
    }

    bool write_ref_size_can_grow( void ) const
    {
        ali_assert(_capacity != 0);

        int const written_size = _write - _read;

        int const no_wrap_size = _capacity - _write % _capacity;

        return _capacity - written_size < no_wrap_size;
    }

    circular_buffer& produce( int n )
    {
        ali_assert(_capacity != 0);

        ali_assert(0 <= n);
        ali_assert(n <= write_ref().size());

        _write += n;

        return *this;
    }

    circular_buffer& push_back( T const& t )
    {
        ali_assert(!write_ref().is_empty());

        write_ref().front() = t;
        
        return produce(1);
    }

    circular_buffer& force_push_back( T const& t )
    {
        if ( write_ref().is_empty() )
            consume(1);

        return push_back(t);
    }

    //  Other

    int capacity( void ) const
    {
        return _capacity;
    }

    circular_buffer& erase( void )
    {
        _read = _write = 0;

        return *this;
    }

    void swap( circular_buffer& b )
    {
        using ali::swap;
        swap(_capacity, b._capacity);
        swap(_read, b._read);
        swap(_write, b._write);
        swap(_begin, b._begin);
    }

    friend void swap(
        circular_buffer& a,
        circular_buffer& b )
    {
        a.swap(b);
    }

private:    //  Data members
    int                 _capacity;
    int                 _read;
    int                 _write;
    ali::auto_ptr<T[]>  _begin;
};

}   //  namespace ali

