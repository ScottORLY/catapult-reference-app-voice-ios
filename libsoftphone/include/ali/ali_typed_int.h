/*
 *  ali_typed_int.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_utility.h"

namespace ali
{

// ******************************************************************
template <class intT, class enumT, enumT type>
struct typed_int
// ******************************************************************
{
    typed_int( void )
    :   value(0)
    {}
    
    explicit typed_int( intT aValue )
    :   value(aValue)
    {}

    void swap( typed_int& b )
    {
        using ali::swap;
        swap(value, b.value);
    }

    friend void swap( typed_int& a, typed_int& b )
    {
        a.swap(b);
    }

    intT    value;
};

// ******************************************************************
template <class intT, class enumT, enumT type>
inline typed_int<intT, enumT, type>& operator++(
    typed_int<intT, enumT, type>& a )
// ******************************************************************
{
    ++a.value;
    return a;
}

// ******************************************************************
template <class intT, class enumT, enumT type>
inline typed_int<intT, enumT, type>& operator+=(
    typed_int<intT, enumT, type>& a,
    typed_int<intT, enumT, type> b )
// ******************************************************************
{
    a.value += b.value;
    return a;
}

// ******************************************************************
template <class intT, class enumT, enumT type>
inline typed_int<intT, enumT, type>& operator--(
    typed_int<intT, enumT, type>& a )
// ******************************************************************
{
    --a.value;
    return a;
}

// ******************************************************************
template <class intT, class enumT, enumT type>
inline typed_int<intT, enumT, type>& operator-=(
    typed_int<intT, enumT, type>& a,
    typed_int<intT, enumT, type> b )
// ******************************************************************
{
    a.value -= b.value;
    return a;
}

// ******************************************************************
template <class intT, class enumT, enumT type>
inline typed_int<intT, enumT, type>& operator*=(
    typed_int<intT, enumT, type>& a,
    typed_int<intT, enumT, type> b )
// ******************************************************************
{
    a.value *= b.value;
    return a;
}

// ******************************************************************
template <class intT, class enumT, enumT type>
inline typed_int<intT, enumT, type>& operator/=(
    typed_int<intT, enumT, type>& a,
    typed_int<intT, enumT, type> b )
// ******************************************************************
{
    a.value /= b.value;
    return a;
}

// ******************************************************************
template <class intT, class enumT, enumT type>
inline typed_int<intT, enumT, type>& operator%=(
    typed_int<intT, enumT, type>& a,
    typed_int<intT, enumT, type> b )
// ******************************************************************
{
    a.value %= b.value;
    return a;
}

// ******************************************************************
template <class intT, class enumT, enumT type>
inline typed_int<intT, enumT, type> operator+(
    typed_int<intT, enumT, type> a )
// ******************************************************************
{
    return a;
}

// ******************************************************************
template <class intT, class enumT, enumT type>
inline typed_int<intT, enumT, type> operator+(
    typed_int<intT, enumT, type> a,
    typed_int<intT, enumT, type> b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
template <class intT, class enumT, enumT type>
inline typed_int<intT, enumT, type> operator-(
    typed_int<intT, enumT, type> a )
// ******************************************************************
{
    a.value = -a.value;
    return a;
}


// ******************************************************************
template <class intT, class enumT, enumT type>
inline typed_int<intT, enumT, type> operator-(
    typed_int<intT, enumT, type> a,
    typed_int<intT, enumT, type> b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
template <class intT, class enumT, enumT type>
inline typed_int<intT, enumT, type> operator*(
    typed_int<intT, enumT, type> a,
    typed_int<intT, enumT, type> b )
// ******************************************************************
{
    return a *= b;
}

// ******************************************************************
template <class intT, class enumT, enumT type>
inline typed_int<intT, enumT, type> operator/(
    typed_int<intT, enumT, type> a,
    typed_int<intT, enumT, type> b )
// ******************************************************************
{
    return a /= b;
}

// ******************************************************************
template <class intT, class enumT, enumT type>
inline typed_int<intT, enumT, type> operator%(
    typed_int<intT, enumT, type> a,
    typed_int<intT, enumT, type> b )
// ******************************************************************
{
    return a %= b;
}

// ******************************************************************
template <class intT, class enumT, enumT type>
inline int compare(
    typed_int<intT, enumT, type> a,
    typed_int<intT, enumT, type> b )
// ******************************************************************
{
    using ali::compare;
    return compare(a.value, b.value);
}

// ******************************************************************
template <class intT, class enumT, enumT type>
inline bool operator==(
    typed_int<intT, enumT, type> a,
    typed_int<intT, enumT, type> b )
// ******************************************************************
{
    return a.value == b.value;
}

// ******************************************************************
template <class intT, class enumT, enumT type>
inline bool operator!=(
    typed_int<intT, enumT, type> a,
    typed_int<intT, enumT, type> b )
// ******************************************************************
{
    return a.value != b.value;
}

// ******************************************************************
template <class intT, class enumT, enumT type>
inline bool operator<(
    typed_int<intT, enumT, type> a,
    typed_int<intT, enumT, type> b )
// ******************************************************************
{
    return a.value < b.value;
}

// ******************************************************************
template <class intT, class enumT, enumT type>
inline bool operator>(
    typed_int<intT, enumT, type> a,
    typed_int<intT, enumT, type> b )
// ******************************************************************
{
    return a.value > b.value;
}

// ******************************************************************
template <class intT, class enumT, enumT type>
inline bool operator<=(
    typed_int<intT, enumT, type> a,
    typed_int<intT, enumT, type> b )
// ******************************************************************
{
    return a.value <= b.value;
}

// ******************************************************************
template <class intT, class enumT, enumT type>
inline bool operator>=(
    typed_int<intT, enumT, type> a,
    typed_int<intT, enumT, type> b )
// ******************************************************************
{
    return a.value >= b.value;
}

} // namespace ali

