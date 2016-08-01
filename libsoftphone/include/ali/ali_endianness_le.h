/*
 *  ali_endianness_le.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_endianness_common.h"

namespace ali
{

// ******************************************************************
inline short to_big_endian( short data )
// ******************************************************************
{
    return to_other_endian(data);
}

// ******************************************************************
inline unsigned short to_big_endian( unsigned short data )
// ******************************************************************
{
    return to_other_endian(data);
}

// ******************************************************************
inline ali::wchar to_big_endian( ali::wchar data )
// ******************************************************************
{
    return to_other_endian(data);
}

// ******************************************************************
inline int to_big_endian( int data )
// ******************************************************************
{
    return to_other_endian(data);
}

// ******************************************************************
inline unsigned int to_big_endian( unsigned int data )
// ******************************************************************
{
    return to_other_endian(data);
}

// ******************************************************************
inline long to_big_endian( long data )
// ******************************************************************
{
    return to_other_endian(data);
}

// ******************************************************************
inline unsigned long to_big_endian( unsigned long data )
// ******************************************************************
{
    return to_other_endian(data);
}

// ******************************************************************
inline long long to_big_endian( long long data )
// ******************************************************************
{
    return to_other_endian(data);
}

// ******************************************************************
inline unsigned long long to_big_endian( unsigned long long data )
// ******************************************************************
{
    return to_other_endian(data);
}

// ******************************************************************
inline short to_little_endian( short data )
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline unsigned short to_little_endian( unsigned short data )
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline ali::wchar to_little_endian( ali::wchar data )
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline int to_little_endian( int data )
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline unsigned int to_little_endian( unsigned int data )
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline long to_little_endian( long data )
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline unsigned long to_little_endian( unsigned long data )
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline long long to_little_endian( long long data )
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline unsigned long long to_little_endian( unsigned long long data )
// ******************************************************************
{
    return data;
}


// ******************************************************************
inline short to_native_from_big_endian( short data )
// ******************************************************************
{
    // to_big_endian applied to a big endian results in a little endian
    return to_big_endian(data);
}

// ******************************************************************
inline unsigned short to_native_from_big_endian( unsigned short data )
// ******************************************************************
{
    // to_big_endian applied to a big endian results in a little endian
    return to_big_endian(data);
}

// ******************************************************************
inline ali::wchar to_native_from_big_endian( ali::wchar data )
// ******************************************************************
{
    // to_big_endian applied to a big endian results in a little endian
    return to_big_endian(data);
}

// ******************************************************************
inline int to_native_from_big_endian( int data )
// ******************************************************************
{
    // to_big_endian applied to a big endian results in a little endian
    return to_big_endian(data);
}

// ******************************************************************
inline unsigned int to_native_from_big_endian( unsigned int data )
// ******************************************************************
{
    // to_big_endian applied to a big endian results in a little endian
    return to_big_endian(data);
}


// ******************************************************************
inline long to_native_from_big_endian( long data )
// ******************************************************************
{
    // to_big_endian applied to a big endian results in a little endian
    return to_big_endian(data);
}

// ******************************************************************
inline unsigned long to_native_from_big_endian( unsigned long data )
// ******************************************************************
{
    // to_big_endian applied to a big endian results in a little endian
    return to_big_endian(data);
}

// ******************************************************************
inline long long to_native_from_big_endian( long long data )
// ******************************************************************
{
    // to_big_endian applied to a big endian results in a little endian
    return to_big_endian(data);
}

// ******************************************************************
inline unsigned long long to_native_from_big_endian( unsigned long long data )
// ******************************************************************
{
    // to_big_endian applied to a big endian results in a little endian
    return to_big_endian(data);
}

// ******************************************************************
inline short to_native_from_little_endian( short data )
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline unsigned short to_native_from_little_endian( unsigned short data )
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline ali::wchar to_native_from_little_endian( ali::wchar data )
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline int to_native_from_little_endian( int data )
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline unsigned int to_native_from_little_endian( unsigned int data )
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline long to_native_from_little_endian( long data )
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline unsigned long to_native_from_little_endian( unsigned long data )
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline long long to_native_from_little_endian( long long data )
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline unsigned long long to_native_from_little_endian( unsigned long long data )
// ******************************************************************
{
    return data;
}

} // namespace ali
