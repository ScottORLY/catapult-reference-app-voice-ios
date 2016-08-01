#pragma once
#include "ali_array_utils.h"
#include "ali_integer.h"
#include "ali_meta.h"
#include "ali_string.h"

namespace ali
{

// ******************************************************************
struct uuid
// ******************************************************************
{
    uuid& generate( void );

    uuid& generate_from_name( uuid const& ns, blob_const_ref name );
    
    uuid& generate_from_name( uuid const& ns, string_const_ref name )
    {
        return generate_from_name(ns, name.as_blob());
    }

    uuid& generate_from_name( blob_const_ref name );
    
    uuid& generate_from_name( string_const_ref name )
    {
        return generate_from_name(name.as_blob());
    }

    uuid& generate_random( void );

    ali::string& format( ali::string& str ) const;
        //  str += 8*XDIGIT '-' 4*XDIGIT '-' 4*XDIGIT '-' 4*XDIGIT '-' 12*XDIGIT
    
    ali::string format( void ) const
    {
        ali::string str;
        format(str);
        return str;
    }

    bool parse( string_const_ref str );
        //  str     := '{' uuid '}' | '[' uuid ']' | uuid
        //  uuid    := 8*XDIGIT '-' 4*XDIGIT '-' 4*XDIGIT '-' 4*XDIGIT '-' 12*XDIGIT

    void swap( uuid& b )
    {
        using ali::swap;
        swap(data1, b.data1);
        swap(data2, b.data2);
        swap(data3, b.data3);
        swap(data4, b.data4);
    }

    friend void swap( uuid& a, uuid& b )
    {
        a.swap(b);
    }

    ali::uint32 data1;
    ali::uint16 data2;
    ali::uint16 data3;
    ali::uint8  data4[8];

    static uuid const null;
    static uuid const namespace_dns;
    static uuid const namespace_url;
    static uuid const namespace_oid;
    static uuid const namespace_x500;
};

// ******************************************************************
static_assert(
    meta::is_pod<uuid>::result,
    "meta::is_pod<uuid>::result");
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
int compare( uuid const& a, uuid const& b );
// ******************************************************************

// ******************************************************************
inline bool operator==( uuid const& a, uuid const& b )
// ******************************************************************
{
    return compare(a, b) == 0;
}

// ******************************************************************
inline bool operator!=( uuid const& a, uuid const& b )
// ******************************************************************
{
    return compare(a, b) != 0;
}

// ******************************************************************
inline bool operator<( uuid const& a, uuid const& b )
// ******************************************************************
{
    return compare(a, b) < 0;
}

// ******************************************************************
inline bool operator<=( uuid const& a, uuid const& b )
// ******************************************************************
{
    return compare(a, b) <= 0;
}

// ******************************************************************
inline bool operator>( uuid const& a, uuid const& b )
// ******************************************************************
{
    return compare(a, b) > 0;
}

// ******************************************************************
inline bool operator>=( uuid const& a, uuid const& b )
// ******************************************************************
{
    return compare(a, b) >= 0;
}

}   //  namespace ali
