/*
 *  ali_location.h
 *  ali Library
 *
 *  Copyright (c) 2010 - 2015 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#define ALI_HERE \
    ali::location(__FILE__, __LINE__)


namespace ali
{

class c_string_const_ref;
class string2;

// Use ALI_HERE to construct
// ******************************************************************
class location
// ******************************************************************
{
public:
    location( void );

    //  Using reference to array so that ali::string.c_str()
    //  (or any other temporary buffer) won't be accepted.
    // ******************************************************************
    template <unsigned int i>
    location( char const (&path)[i],
              unsigned int line )
    // ******************************************************************
    :   _path(path)
    ,   _pathLen(i - 1 /* NULL */)
    ,   _line(line)
    {}

    static c_string_const_ref file_from_path( c_string_const_ref path );

    // inline methods below are defined in ali_array_utils.h

    inline c_string_const_ref path( void ) const;

    // The last part of path()
    inline c_string_const_ref file( void ) const;

    // ******************************************************************
    unsigned int line( void ) const
    // ******************************************************************
    {
        return _line;
    }

    string2 format( void ) const;
    string2& format( string2 & str ) const;

private:
    const char*     _path;
    int             _pathLen;
    unsigned int    _line;
};

}

