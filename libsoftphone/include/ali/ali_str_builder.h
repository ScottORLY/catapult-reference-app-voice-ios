/*
 *  ali_str_builder.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_format.h"

namespace ali
{

namespace str
{

// ******************************************************************
class builder
// ******************************************************************
{
public:
    builder( void )
    {}
    
    explicit builder( ali::string const& str )
    :   _str(str)
    {}
    
    explicit builder( char const* str )
    :   _str(str)
    {}
    
    builder( char const* str, int length )
    :   _str(str, length)
    {}
    
    builder& operator()( string_const_ref str )
    {
        _str.append(str);
        return *this;
    }
    
    builder& operator()( char const* str, int length )
    {
        _str.append(str, length);
        return *this;
    }
    
    builder& operator()( short val, int digits = 0 )
    {
        return operator()(static_cast<long>(val), digits);
    }
    
    builder& operator()( unsigned short val, int digits = 0 )
    {
        return operator()(static_cast<unsigned long>(val), digits);
    }
    
    builder& operator()( int val, int digits = 0 )
    {
        return operator()(static_cast<long>(val), digits);
    }
    
    builder& operator()( unsigned int val, int digits = 0 )
    {
        return operator()(static_cast<unsigned long>(val), digits);
    }
    
    builder& operator()( long val, int digits = 0 );
    builder& operator()( unsigned long val, int digits = 0 );

    template <class T>
    builder& operator()( T const& val )
    {
        _str.append(ali::str::format(val));
        return *this;
    }
/*    
    builder& hex( short val, int digits = 0 );
    builder& hex( unsigned short val, int digits = 0 );
    builder& hex( int val, int digits = 0 );
    builder& hex( unsigned int val, int digits = 0 );
    builder& hex( long val, int digits = 0 );
    builder& hex( unsigned long val, int digits = 0 );
*/
    ali::string& str( void )
    {
        return _str;
    }
    
    ali::string const& str( void ) const
    {
        return _str;
    }
    
private:    // Data members
    ali::string     _str;
};

} // namespace str


namespace wstr
{

// ******************************************************************
class builder
// ******************************************************************
{
public:
    builder( void )
    {}
    
    explicit builder( ali::wstring const& str )
    :   _str(str)
    {}
    
    explicit builder( wchar const* str )
    :   _str(str)
    {}
    
    builder( wchar const* str, int length )
    :   _str(str, length)
    {}
    
    builder& operator()( wstring_const_ref str )
    {
        _str.append(str);
        return *this;
    }
    
    builder& operator()( wchar const* str, int length )
    {
        _str.append(str, length);
        return *this;
    }
    
    builder& operator()( short val, int digits = 0 )
    {
        return operator()(static_cast<long>(val), digits);
    }
    
    builder& operator()( unsigned short val, int digits = 0 )
    {
        return operator()(static_cast<unsigned long>(val), digits);
    }
    
    builder& operator()( int val, int digits = 0 )
    {
        return operator()(static_cast<long>(val), digits);
    }
    
    builder& operator()( unsigned int val, int digits = 0 )
    {
        return operator()(static_cast<unsigned long>(val), digits);
    }
    
    builder& operator()( long val, int digits = 0 );
    builder& operator()( unsigned long val, int digits = 0 );

    template <class T>
    builder& operator()( T const& val )
    {
        _str.append(ali::wstr::format(val));
        return *this;
    }
/*    
    builder& hex( short val, int digits = 0 );
    builder& hex( unsigned short val, int digits = 0 );
    builder& hex( int val, int digits = 0 );
    builder& hex( unsigned int val, int digits = 0 );
    builder& hex( long val, int digits = 0 );
    builder& hex( unsigned long val, int digits = 0 );
*/
    ali::wstring& str( void )
    {
        return _str;
    }
    
    ali::wstring const& str( void ) const
    {
        return _str;
    }

private:    // Data members
    ali::wstring     _str;
};

} // namespace wstr

namespace tstr
{

#ifdef UNICODE

using namespace wstr;

#else

using namespace str;

#endif //  UNICODE

} // namespace tstr


// ******************************************************************
// Formatting
// ******************************************************************


template <>
struct format_t<ali::str::builder>
{
    ali::string operator ()(ali::str::builder const& builder) const
    {
        return builder.str();
    }
};


template <>
struct format_t<ali::wstr::builder>
{
    ali::wstring operator ()(ali::wstr::builder const& builder) const
    {
        return builder.str();
    }
};

} // namespace ali

