/*
 *  ali_blob.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_blob_forward.h"

#ifdef  ALI_BLOB_IS_ARRAY

#include "ali_array.h"

#else   //  !ALI_BLOB_IS_ARRAY

#include "ali_array_utils.h"
#include "ali_deprecated.h"
#include "ali_integer.h"
#include "ali_string.h"
#include "ali_utility.h"

namespace ali
{

// ******************************************************************
class blob
// ******************************************************************
{
public:     // Typedefs
    typedef int size_type;

public:     // Data members
    static size_type const npos;

public:     // Methods
    blob( void );
    
    blob(
        blob const& b,
        size_type pos = 0,
        size_type n = npos );

    blob( blob_const_ref b);
    
    blob(
        ali::string const& str,
        int pos = 0,
        int n = ali::string::npos );

    blob(
        ali::wstring const& str,
        int pos = 0,
        int n = ali::wstring::npos );

    blob( void const* buf, size_type n );
    
    explicit blob( size_type n );
    
    ~blob( void );

    blob& operator=( blob && b );
    blob& operator=( blob const& b );
    blob& operator=( blob_const_ref b );
    blob& operator=( ali::string const& str );
    blob& operator=( ali::wstring const& str );

    // size/capacity
    size_type size( void ) const
    {
        return _rep == nullptr ? 0 : _rep->size;
    }
    
    size_type capacity( void ) const
    {
        return _rep == nullptr ? 0 : _rep->cap;
    }

    blob& resize( size_type n );
    
    blob& reserve( size_type n );

    bool is_empty( void ) const
    {
        return size() == 0;
    }

ALI_DEPRECATED(
    bool empty( void ) const
    {
        return is_empty();
    }
);

    // erase
    blob& erase( size_type pos, size_type n = npos );

    blob& erase( void );

ALI_DEPRECATED(    
    blob& clear( void )
    {
        return erase();
    }
);

    // assign
    blob& assign( void const* buf, size_type n );
    
    blob& assign(
        blob const& buf,
        size_type pos = 0,
        size_type n = npos );

    // insert
    blob& insert(
        size_type pos1,
        blob const& buf,
        size_type pos2 = 0,
        size_type n2 = npos );
    
    blob& insert(
        size_type pos1,
        void const* buf,
        size_type n );

    blob& insert(
        size_type pos1,
        ali::string const& str,
        int pos2 = 0,
        int n2 = ali::string::npos )
    {
        pos2 = ali::maxi(0, ali::mini(pos2, str.size()));
        n2 = ali::mini(n2, str.size() - pos2);
        return insert(pos1, str.data() + pos2, n2 * sizeof(ali::string::value_type));
    }

    blob& insert(
        size_type pos1,
        ali::wstring const& str,
        int pos2 = 0,
        int n2 = ali::wstring::npos )
    {
        pos2 = ali::maxi(0, ali::mini(pos2, str.size()));
        n2 = ali::mini(n2, str.size() - pos2);
        return insert(pos1, str.data() + pos2, n2 * sizeof(ali::wstring::value_type));
    }
    
    blob& insert_int8(
        size_type pos1,
        ali::uint8 val,
        size_type n = 1 );

    // append
    blob& append(
        blob const& buf,
        size_type pos = 0,
        size_type n = npos );

    blob& append( blob_const_ref buf )
    {
        return append(buf.data(), buf.size());
    }
        
    blob& append( void const* buf, size_type n );
    
    blob& append( char const* str )
    {
        return append(str, ali::str_len(str));
    }

    blob& append(
        ali::string const& str,
        int pos = 0,
        int n = ali::string::npos )
    {
        pos = ali::maxi(0, ali::mini(pos, str.size()));
        n = ali::mini(n, str.size() - pos);
        return append(str.data() + pos, n * sizeof(ali::string::value_type));
    }

    blob& append(
        ali::wstring const& str,
        int pos = 0,
        int n = ali::wstring::npos )
    {
        pos = ali::maxi(0, ali::mini(pos, str.size()));
        n = ali::mini(n, str.size() - pos);
        return append(str.data() + pos, n * sizeof(ali::wstring::value_type));
    }

    blob& append_int_be( ali::uint32 val, int nBytes );
    
    blob& append_int_le( ali::uint32 val, int nBytes );
    
    
    blob& append_int8( ali::uint8 val, size_type n = 1 );
    
    
    blob& append_int16_be( ali::uint16 val )
    {
        return append_int_be(val, 2);
    }
    
    blob& append_int16_le( ali::uint16 val )
    {
        return append_int_le(val, 2);
    }
    
    blob& append_int16_ne( ali::uint16 val )
    {
        return append_int_le(to_little_endian(val), 2);
    }


    blob& append_int32_be( ali::uint32 val )
    {
        return append_int_be(val, 4);
    }

    blob& append_int32_le( ali::uint32 val )
    {
        return append_int_le(val, 4);
    }
    
    blob& append_int32_ne( ali::uint32 val )
    {
        return append_int_le(to_little_endian(val), 4);
    }
    

    blob& append_byte( ali::uint8 val )
        //  Deprecated!
    {
        return append_int8(val);
    }
    
    
    blob& append_word_be( ali::uint16 val )
        //  Deprecated!
    {
        return append_int16_be(val);
    }

    blob& append_word_le( ali::uint16 val )
        //  Deprecated!
    {
        return append_int16_le(val);
    }
    
    
    blob& append_dword_be( ali::uint32 val )
        //  Deprecated!
    {
        return append_int32_be(val);
    }

    blob& append_dword_le( ali::uint32 val )
        //  Deprecated!
    {
        return append_int32_le(val);
    }

    // replace
    blob& replace(
        size_type pos1,
        size_type n1,
        blob const& buf,
        size_type pos2 = 0,
        size_type n2 = npos );
        
    blob& replace(
        size_type pos1,
        size_type n1,
        void const* buf,
        size_type n2 );

    blob& replace(
        size_type pos1,
        size_type n1,
        ali::string const& str,
        int pos2 = 0,
        int n2 = ali::string::npos )
    {
        pos2 = ali::maxi(0, ali::mini(pos2, str.size()));
        n2 = ali::mini(n2, str.size() - pos2);
        return replace(
            pos1, n1,
            str.data() + pos2,
            n2 * sizeof(ali::string::value_type));
    }

    blob& replace(
        size_type pos1,
        size_type n1,
        ali::wstring const& str,
        int pos2 = 0,
        int n2 = ali::wstring::npos )
    {
        pos2 = ali::maxi(0, ali::mini(pos2, str.size()));
        n2 = ali::mini(n2, str.size() - pos2);
        return replace(
            pos1, n1,
            str.data() + pos2,
            n2 * sizeof(ali::wstring::value_type));
    }

    // compare
    int compare( blob const& buf ) const
    {
        return compare(0, size(), buf);
    }

    int compare( void const* buf, size_type n ) const
    {
        return compare(0, size(), buf, n);
    }

    int compare(
        size_type pos1,
        size_type n1,
        blob const& buf ) const;
        
    int compare(
        size_type pos1,
        size_type n1,
        blob const& buf,
        size_type pos2,
        size_type n2 ) const;

    int compare(
        size_type pos1,
        size_type n1,
        void const* buf,
        size_type n2 ) const;
    
    // find
    size_type find(
        ali::uint8 byte,
        size_type pos1 = 0 ) const;
    
    size_type find(
        blob const& buf,
        size_type pos1 = 0 ) const
    {
        return find(buf, 0, buf.size(), pos1);
    }

    size_type find(
        ali::string const& str,
        size_type pos1 = 0 ) const
    {
        return find(str, 0, str.size(), pos1);
    }

    size_type find(
        ali::wstring const& str,
        size_type pos1 = 0 ) const
    {
        return find(str, 0, str.size(), pos1);
    }

    size_type find(
        blob const& buf,
        size_type pos2,
        size_type n2,
        size_type pos1 = 0 ) const;
        
    size_type find(
        void const* buf,
        size_type n,
        size_type pos1 = 0 ) const;

    size_type find(
        ali::string const& str,
        int pos2,
        int n2,
        size_type pos1 = 0 ) const
    {
        pos2 = ali::maxi(0, ali::mini(pos2, str.size()));
        n2 = ali::mini(n2, str.size() - pos2);
        return find(
            str.data() + pos2,
            n2 * sizeof(ali::string::value_type),
            pos1);
    }

    size_type find(
        ali::wstring const& str,
        int pos2,
        int n2,
        size_type pos1 = 0 ) const
    {
        pos2 = ali::maxi(0, ali::mini(pos2, str.size()));
        n2 = ali::mini(n2, str.size() - pos2);
        return find(
            str.data() + pos2,
            n2 * sizeof(ali::wstring::value_type),
            pos1);
    }

    // rfind
    size_type rfind(
        ali::uint8 byte,
        size_type pos1 = npos ) const;
    
    size_type rfind(
        blob const& buf,
        size_type pos1 = npos ) const
    {
        return rfind(buf, 0, buf.size(), pos1);
    }

    size_type rfind(
        ali::string const& str,
        size_type pos1 = npos ) const
    {
        return rfind(str, 0, str.size(), pos1);
    }

    size_type rfind(
        ali::wstring const& str,
        size_type pos1 = npos ) const
    {
        return rfind(str, 0, str.size(), pos1);
    }

    size_type rfind(
        blob const& buf,
        size_type pos2,
        size_type n2,
        size_type pos1 = npos ) const;
        
    size_type rfind(
        void const* buf,
        size_type n,
        size_type pos1 = npos ) const;

    size_type rfind(
        ali::string const& str,
        int pos2, 
        int n2,
        size_type pos1 = npos ) const
    {
        pos2 = ali::maxi(0, ali::mini(pos2, str.size()));
        n2 = ali::mini(n2, str.size() - pos2);
        return rfind(str.data() + pos2, n2 * sizeof(ali::string::value_type), pos1);
    }

    size_type rfind(
        ali::wstring const& str,
        int pos2, 
        int n2,
        size_type pos1 = npos ) const
    {
        pos2 = ali::maxi(0, ali::mini(pos2, str.size()));
        n2 = ali::mini(n2, str.size() - pos2);
        return rfind(str.data() + pos2, n2 * sizeof(ali::wstring::value_type), pos1);
    }

    // subblob
    blob subblob( size_type pos = 0, size_type n = npos ) const
    {
        return blob(*this, pos, n);
    }

    // accessors
    ali::uint8* data( void )
    {
        //ali_assert(_rep != nullptr); // nullptr is valid for zero size
        return _rep->data;
    }

    ali::uint8 const* data( void ) const
    {
        //ali_assert(_rep != nullptr); // nullptr is valid for zero size
        return _rep->data;
    }

    ali::uint8& at( size_type pos )
    {
        ensure_unique();
        ali_assert(_rep != nullptr);
        return _rep->data[pos];
    }

    ali::uint8 const& at( size_type pos ) const
    {
        ali_assert(_rep != nullptr);
        return _rep->data[pos];
    }

    ali::uint8& operator[]( size_type pos )
    {
        ensure_unique();
        ali_assert(_rep != nullptr);
        return _rep->data[pos];
    }

    ali::uint8 const& operator[]( size_type pos ) const
    {
        ali_assert(_rep != nullptr);
        return _rep->data[pos];
    }
    
    // pre: nBytes in set {1, 2, 3, 4}
    static ali::uint32 int_be(
        ali::uint8 const* bytes,
        int nBytes );

    // pre: nBytes in set {1, 2, 3, 4}
    static ali::uint32 int_le(
        ali::uint8 const* bytes,
        int nBytes );
        
    static ali::uint8 int8( ali::uint8 const* bytes )
    {
        return *bytes;
    }
    
    static ali::uint16 int16_be( ali::uint8 const* bytes )
    {
        return static_cast<ali::uint16>(int_be(bytes, 2));
    }

    static ali::uint16 int16_le( ali::uint8 const* bytes )
    {
        return static_cast<ali::uint16>(int_le(bytes, 2));
    }

    static ali::uint32 int32_be( ali::uint8 const* bytes )
    {
        return int_be(bytes, 4);
    }

    static ali::uint32 int32_le( ali::uint8 const* bytes )
    {
        return int_le(bytes, 4);
    }
    
    // pre: nBytes in set {1, 2, 3, 4}
    ali::uint32 int_be_at( size_type pos, int nBytes ) const
    {
        ali_assert(pos + nBytes <= size());
        return int_be(data() + pos, nBytes);
    }
    
    // pre: nBytes in set {1, 2, 3, 4}
    ali::uint32 int_le_at( size_type pos, int nBytes ) const
    {
        ali_assert(pos + nBytes <= size());
        return int_le(data() + pos, nBytes);
    }
    
    ali::uint8 int8_at( size_type pos ) const
    {
        ali_assert(pos < size());
        return blob::int8(data() + pos);
    }

    ali::uint16 int16_be_at( size_type pos ) const
    {
        return static_cast<ali::uint16>(int_be_at(pos, 2));
    }

    ali::uint16 int16_le_at( size_type pos ) const
    {
        return static_cast<ali::uint16>(int_le_at(pos, 2));
    }
    
    ali::uint16 int16_ne_at( size_type pos ) const
    {
        return to_native_from_little_endian(int16_le_at(pos));
    }

    ali::uint32 int32_be_at( size_type pos ) const
    {
        return int_be_at(pos, 4);
    }

    ali::uint32 int32_le_at( size_type pos ) const
    {
        return int_le_at(pos, 4);
    }
    
    ali::uint32 int32_ne_at( size_type pos ) const
    {
        return to_native_from_little_endian(int32_le_at(pos));
    }


    ali::uint8 byte_at( size_type pos ) const
        //  Deprecated!
    {
        return int8_at(pos);
    }

    ali::uint16 word_be_at( size_type pos ) const
        //  Deprecated!
    {
        return int16_be_at(pos);
    }

    ali::uint16 word_le_at( size_type pos ) const
        //  Deprecated!
    {
        return int16_le_at(pos);
    }

    ali::uint32 dword_be_at( size_type pos ) const
        //  Deprecated!
    {
        return int32_be_at(pos);
    }

    ali::uint32 dword_le_at( size_type pos ) const
        //  Deprecated!
    {
        return int32_le_at(pos);
    }

    // mutators
    blob& set_bin_at(
        size_type pos1,
        blob const& buf,
        size_type pos2 = 0,
        size_type n2 = npos );
        
    blob& set_bin_at(
        size_type pos1,
        void const* buf,
        size_type n );

    // valid nBytes are 1, 2, 3, 4
    blob& set_int_be_at(
        size_type pos,
        ali::uint32 val,
        int nBytes );
        
    blob& set_int_le_at(
        size_type pos,
        ali::uint32 val,
        int nBytes );

    blob& set_int8_at( size_type pos, ali::uint8 val )
    {
        ali_assert(pos < size());
        ensure_unique();
        _rep->data[pos] = val;
        return *this;
    }

    blob& set_int16_be_at( size_type pos, ali::uint16 val )
    {
        return set_int_be_at(pos, val, 2);
    }

    blob& set_int16_le_at( size_type pos, ali::uint16 val )
    {
        return set_int_le_at(pos, val, 2);
    }
    
    blob& set_int16_ne_at( size_type pos, ali::uint16 val )
    {
        return set_int_le_at(pos, to_little_endian(val), 2);
    }

    blob& set_int32_be_at( size_type pos, ali::uint32 val )
    {
        return set_int_be_at(pos, val, 4);
    }

    blob& set_int32_le_at( size_type pos, ali::uint32 val )
    {
        return set_int_le_at(pos, val, 4);
    }
    
    blob& set_int32_ne_at( size_type pos, ali::uint32 val )
    {
        return set_int_le_at(pos, to_little_endian(val), 4);
    }

    blob& set_byte_at( size_type pos, ali::uint8 val )
        //  Deprecated!
    {
        return set_int8_at(pos, val);
    }

    blob& set_word_be_at( size_type pos, ali::uint16 val )
        //  Deprecated!
    {
        return set_int16_be_at(pos, val);
    }

    blob& set_word_le_at( size_type pos, ali::uint16 val )
        //  Deprecated!
    {
        return set_int16_le_at(pos, val);
    }

    blob& set_dword_be_at( size_type pos, ali::uint32 val )
        //  Deprecated!
    {
        return set_int32_be_at(pos, val);
    }

    blob& set_dword_le_at( size_type pos, ali::uint32 val )
        //  Deprecated!
    {
        return set_int32_le_at(pos, val);
    }

    string_const_ref as_string( void ) const
    {
        return string_const_ref(
            is_empty() ? "" : reinterpret_cast<char const*>(data()),
            size());
    }

    wstring_const_ref as_wstring( void ) const
    {
        return wstring_const_ref(
            is_empty() ? L"" : reinterpret_cast<wchar const*>(data()),
            size() / sizeof(wchar));
    }

    blob_const_ptr pointer( int pos, int n ) const
    {
        return pointer()->pointer(pos, n);
    }

    blob_const_ptr pointer( void ) const
    {
        return  _rep == nullptr
            ?   blob_const_ptr()
            :   blob_const_ptr(_rep->data, _rep->size);
    }

    blob_const_ptr pointer_left( int pos ) const
    {
        return pointer()->pointer_left(pos);
    }
    
    blob_const_ptr pointer_right( int pos ) const
    {
        return pointer()->pointer_right(pos);
    }

    blob_const_ptr pointer_front( int n ) const
    {
        return pointer()->pointer_front(n);
    }

    blob_const_ptr pointer_back( int n ) const
    {
        return pointer()->pointer_back(n);
    }

    blob_ptr mutable_pointer( void )
    {
        ensure_unique();
        return  _rep == nullptr
            ?   blob_ptr()
            :   blob_ptr(_rep->data, _rep->size);
    }

    blob_ptr mutable_pointer( int pos, int n )
    {
        return mutable_pointer()->mutable_pointer(pos, n);
    }

    blob_ptr mutable_pointer_left( int pos )
    {
        return mutable_pointer()->mutable_pointer_left(pos);
    }
    
    blob_ptr mutable_pointer_right( int pos )
    {
        return mutable_pointer()->mutable_pointer_right(pos);
    }

    blob_ptr mutable_pointer_front( int n )
    {
        return mutable_pointer()->mutable_pointer_front(n);
    }

    blob_ptr mutable_pointer_back( int n )
    {
        return mutable_pointer()->mutable_pointer_back(n);
    }

    blob_const_ref ref( int pos, int n ) const
    {
        return *pointer(pos, n);
    }

    blob_const_ref ref( void ) const
    {
        return *this;
    }

    blob_const_ref ref_left( int pos ) const
    {
        return *pointer_left(pos);
    }
    
    blob_const_ref ref_right( int pos ) const
    {
        return *pointer_right(pos);
    }

    blob_const_ref ref_front( int n ) const
    {
        return *pointer_front(n);
    }

    blob_const_ref ref_back( int n ) const
    {
        return *pointer_back(n);
    }
    
    blob_ref mutable_ref( void )
    {
        return *this;
    }

    blob_ref mutable_ref( int pos, int n )
    {
        return *mutable_pointer(pos, n);
    }

    blob_ref mutable_ref_left( int pos )
    {
        return *mutable_pointer_left(pos);
    }
    
    blob_ref mutable_ref_right( int pos )
    {
        return *mutable_pointer_right(pos);
    }

    blob_ref mutable_ref_front( int n )
    {
        return *mutable_pointer_front(n);
    }

    blob_ref mutable_ref_back( int n )
    {
        return *mutable_pointer_back(n);
    }

    operator blob_const_ref( void ) const
    {
        return  _rep == nullptr
            ?   blob_const_ref()
            :   blob_const_ref(_rep->data, _rep->size);
    }

    operator blob_const_ref( void )
    {
        return *const_cast<blob const*>(this);
    }

    operator blob_ref( void )
    {
        ensure_unique();
        return  _rep == nullptr
            ?   blob_ref()
            :   blob_ref(_rep->data, _rep->size);
    }

    is_internal_pointer_result
        is_internal_pointer( ali::uint8 const* a ) const
    {
        return ref().is_internal_pointer(a);
    }

    is_internal_pointer_result
        is_internal_pointer( blob_const_ref a ) const
    {
        return ref().is_internal_pointer(a);
    }

    is_internal_pointer_result
        is_internal_pointer( blob_const_ptr a ) const
    {
        return ref().is_internal_pointer(a);
    }

    void swap( blob& b )
    {
        using ali::swap;
        swap(_rep, b._rep);
    }

    friend void swap( blob& a, blob& b )
    {
        a.swap(b);
    }

private:    // Struct
    struct blob_rep
    {
        unsigned    rc;
        size_type   size;
        size_type   cap;
        ali::uint8  data[1];
    };

private:    // Methods
    void ensure_unique( void );
    void ensure_unique( size_type cap );

    static blob_rep* create( size_type cap );
    static blob_rep* clone( blob_rep const* b );
    static blob_rep* clone( blob_rep const* b, size_type cap );
    static void destroy( blob_rep* b );

private:    // Data members
    blob_rep*    _rep;
};

namespace str
{

// ******************************************************************
class from_blob : public ali::string
// ******************************************************************
{
public:
    from_blob( ali::blob const& b )
    {
        if ( b.is_empty() ) return;
        ali::string(reinterpret_cast<char const*>(b.data()), b.size()).swap(*this);
    }
};

} // namespace str

} // namespace ali

#endif  //  ALI_BLOB_IS_ARRAY
