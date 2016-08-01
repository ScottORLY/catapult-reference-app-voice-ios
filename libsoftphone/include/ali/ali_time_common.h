/*
 *  ali_time_common.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_array_forward.h"
#include "ali_typed_int.h"
#include "ali_string_forward.h"
#include "ali_math.h"

namespace ali
{

namespace time
{

namespace unit
{

enum type
{
    microsecond,
    millisecond,
    second,
    minute,
    hour
};

}   //  namespace unit

// ******************************************************************
typedef ali::typed_int<
    ali::int64,
    time::unit::type,
    time::unit::microsecond>    microseconds64;
// ******************************************************************

// ******************************************************************
typedef ali::typed_int<
    ali::int64,
    time::unit::type,
    time::unit::millisecond>    milliseconds64;
// ******************************************************************

// ******************************************************************
typedef ali::typed_int<
    int,
    time::unit::type,
    time::unit::millisecond>    milliseconds;
// ******************************************************************

// ******************************************************************
typedef ali::typed_int<
    ali::int64,
    time::unit::type,
    time::unit::second>         seconds64;
// ******************************************************************

// ******************************************************************
typedef ali::typed_int<
    int,
    time::unit::type,
    time::unit::second>         seconds;
// ******************************************************************

// ******************************************************************
typedef ali::typed_int<
    double,
    time::unit::type,
    time::unit::second>         float_seconds;
// ******************************************************************

// ******************************************************************
typedef ali::typed_int<
    int,
    time::unit::type,
    time::unit::minute>         minutes;
// ******************************************************************

// ******************************************************************
typedef ali::typed_int<
    int,
    time::unit::type,
    time::unit::hour>           hours;
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
inline time::milliseconds64& convert(
    time::milliseconds64& a,
    time::seconds b )
// ******************************************************************
{
    a.value = b.value;
    a.value *= 1000;
    return a;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
inline time::seconds& operator+=(
        time::seconds& a,
        time::minutes b )
// ******************************************************************
{
    a.value += 60 * b.value;
    return a;
}

// ******************************************************************
inline time::seconds64& operator+=(
        time::seconds64& a,
        time::minutes b )
// ******************************************************************
{
    a.value += 60 * b.value;
    return a;
}

// ******************************************************************
inline time::float_seconds& operator+=(
        time::float_seconds& a,
        time::minutes b )
// ******************************************************************
{
    a.value += 60 * b.value;
    return a;
}

// ******************************************************************
inline time::seconds& operator-=(
        time::seconds& a,
        time::minutes b )
// ******************************************************************
{
    a.value -= 60 * b.value;
    return a;
}

// ******************************************************************
inline time::seconds64& operator-=(
        time::seconds64& a,
        time::minutes b )
// ******************************************************************
{
    a.value -= 60 * b.value;
    return a;
}

// ******************************************************************
inline time::float_seconds& operator-=(
        time::float_seconds& a,
        time::minutes b )
// ******************************************************************
{
    a.value -= 60 * b.value;
    return a;
}

// ******************************************************************
inline time::seconds& operator+=(
        time::seconds& a,
        time::hours b )
// ******************************************************************
{
    a.value += 60 * 60 * b.value;
    return a;
}

// ******************************************************************
inline time::seconds64& operator+=(
        time::seconds64& a,
        time::hours b )
// ******************************************************************
{
    a.value += 60 * 60 * b.value;
    return a;
}

// ******************************************************************
inline time::float_seconds& operator+=(
        time::float_seconds& a,
        time::hours b )
// ******************************************************************
{
    a.value += 60 * 60 * b.value;
    return a;
}

// ******************************************************************
inline time::seconds& operator-=(
        time::seconds& a,
        time::hours b )
// ******************************************************************
{
    a.value -= 60 * 60 * b.value;
    return a;
}

// ******************************************************************
inline time::seconds64& operator-=(
        time::seconds64& a,
        time::hours b )
// ******************************************************************
{
    a.value -= 60 * 60 * b.value;
    return a;
}

// ******************************************************************
inline time::float_seconds& operator-=(
        time::float_seconds& a,
        time::hours b )
// ******************************************************************
{
    a.value -= 60 * 60 * b.value;
    return a;
}

// ******************************************************************
inline time::minutes& operator+=(
    time::minutes& a,
    time::hours b )
// ******************************************************************
{
    a.value += 60 * b.value;
    return a;
}

// ******************************************************************
inline time::minutes& operator-=(
    time::minutes& a,
    time::hours b )
// ******************************************************************
{
    a.value -= 60 * b.value;
    return a;
}

// ******************************************************************
inline time::seconds operator+(
        time::seconds a,
        time::minutes b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::seconds64 operator+(
        time::seconds64 a,
        time::minutes b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::seconds operator-(
        time::seconds a,
        time::minutes b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::seconds64 operator-(
        time::seconds64 a,
        time::minutes b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::seconds operator+(
        time::seconds a,
        time::hours b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::seconds64 operator+(
        time::seconds64 a,
        time::hours b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::seconds operator-(
        time::seconds a,
        time::hours b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::seconds64 operator-(
        time::seconds64 a,
        time::hours b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::minutes operator+(
    time::minutes a,
    time::hours b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::minutes operator-(
    time::minutes a,
    time::hours b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::hours whole_hours( time::minutes m )
// ******************************************************************
{
    return time::hours(m.value / 60);
}

// ******************************************************************
inline time::minutes strip_hours( time::minutes m )
// ******************************************************************
{
    m.value %= 60; return m;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct unix_timestamp
// ******************************************************************
{
    unix_timestamp( void )
    {}
    
    explicit unix_timestamp( time::seconds value )
    :   value(value)
    {}

    unix_timestamp& operator+=( time::seconds delta )
    {
        value += delta;
        return *this;
    }
    
    unix_timestamp& operator-=( time::seconds delta )
    {
        value -= delta;
        return *this;
    }
    
    unix_timestamp& operator+=( time::minutes delta )
    {
        value += delta;
        return *this;
    }
    
    unix_timestamp& operator-=( time::minutes delta )
    {
        value -= delta;
        return *this;
    }
    
    unix_timestamp& operator+=( time::hours delta )
    {
        value += delta;
        return *this;
    }
    
    unix_timestamp& operator-=( time::hours delta )
    {
        value -= delta;
        return *this;
    }
    
    bool operator==( time::unix_timestamp b ) const
    {
        return value == b.value;
    }

    bool operator!=( time::unix_timestamp b ) const
    {
        return value != b.value;
    }

    bool operator<( time::unix_timestamp b ) const
    {
        return value < b.value;
    }

    bool operator<=( time::unix_timestamp b ) const
    {
        return value <= b.value;
    }

    bool operator>( time::unix_timestamp b ) const
    {
        return value > b.value;
    }

    bool operator>=( time::unix_timestamp b ) const
    {
        return value >= b.value;
    }

    void swap( time::unix_timestamp& b )
    {
        using ali::swap;
        swap(value, b.value);
    }

    friend void swap(
        time::unix_timestamp& a,
        time::unix_timestamp& b )
    {
        a.swap(b);
    }
    
    time::seconds value;
};

// ******************************************************************
inline time::unix_timestamp operator+(
    time::unix_timestamp a, time::seconds b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::unix_timestamp operator-(
    time::unix_timestamp a, time::seconds b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::unix_timestamp operator+(
    time::unix_timestamp a, time::minutes b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::unix_timestamp operator-(
    time::unix_timestamp a, time::minutes b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::unix_timestamp operator+(
    time::unix_timestamp a, time::hours b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::unix_timestamp operator-(
    time::unix_timestamp a, time::hours b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::seconds operator-(
    time::unix_timestamp a,
    time::unix_timestamp b )
// ******************************************************************
{
    return a.value - b.value;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct unix_timestamp64
// ******************************************************************
{
    unix_timestamp64( void )
    {}
    
    explicit unix_timestamp64( time::seconds64 value )
    :   value(value)
    {}

    unix_timestamp64& operator+=( time::seconds64 delta )
    {
        value += delta;
        return *this;
    }
    
    unix_timestamp64& operator-=( time::seconds64 delta )
    {
        value -= delta;
        return *this;
    }
    
    unix_timestamp64& operator+=( time::minutes delta )
    {
        value += delta;
        return *this;
    }
    
    unix_timestamp64& operator-=( time::minutes delta )
    {
        value -= delta;
        return *this;
    }
    
    unix_timestamp64& operator+=( time::hours delta )
    {
        value += delta;
        return *this;
    }
    
    unix_timestamp64& operator-=( time::hours delta )
    {
        value -= delta;
        return *this;
    }
    
    bool operator==( time::unix_timestamp64 b ) const
    {
        return value == b.value;
    }

    bool operator!=( time::unix_timestamp64 b ) const
    {
        return value != b.value;
    }

    bool operator<( time::unix_timestamp64 b ) const
    {
        return value < b.value;
    }

    bool operator<=( time::unix_timestamp64 b ) const
    {
        return value <= b.value;
    }

    bool operator>( time::unix_timestamp64 b ) const
    {
        return value > b.value;
    }

    bool operator>=( time::unix_timestamp64 b ) const
    {
        return value >= b.value;
    }
    
    time::seconds64 value;
};

// ******************************************************************
inline time::unix_timestamp64 operator+(
    time::unix_timestamp64 a, time::seconds64 b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::unix_timestamp64 operator-(
    time::unix_timestamp64 a, time::seconds64 b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::unix_timestamp64 operator+(
    time::unix_timestamp64 a, time::minutes b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::unix_timestamp64 operator-(
    time::unix_timestamp64 a, time::minutes b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::unix_timestamp64 operator+(
    time::unix_timestamp64 a, time::hours b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::unix_timestamp64 operator-(
    time::unix_timestamp64 a, time::hours b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::seconds64 operator-(
    time::unix_timestamp64 a,
    time::unix_timestamp64 b )
// ******************************************************************
{
    return a.value - b.value;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct unix_micro_timestamp
// ******************************************************************
{
    unix_micro_timestamp( void )
    {}
    
    explicit unix_micro_timestamp( time::seconds value )
    :   value(value.value * 1000000)
    {}

    explicit unix_micro_timestamp( time::float_seconds value )
    :   value(static_cast<ali::int64>(value.value * 1000000.0))
    {}

    explicit unix_micro_timestamp( time::microseconds64 value )
    :   value(value)
    {}

    unix_micro_timestamp& operator+=( time::seconds delta )
    {
        return operator+=(unix_micro_timestamp(delta).value);
    }

    unix_micro_timestamp& operator+=( time::float_seconds delta )
    {
        return operator+=(unix_micro_timestamp(delta).value);
    }

    unix_micro_timestamp& operator+=( time::microseconds64 delta )
    {
        value += delta;
        return *this;
    }
    
    unix_micro_timestamp& operator-=( time::seconds delta )
    {
        return operator-=(unix_micro_timestamp(delta).value);
    }

    unix_micro_timestamp& operator-=( time::float_seconds delta )
    {
        return operator-=(unix_micro_timestamp(delta).value);
    }

    unix_micro_timestamp& operator-=( time::microseconds64 delta )
    {
        value -= delta;
        return *this;
    }
    
    bool operator==( time::unix_micro_timestamp b ) const
    {
        return value == b.value;
    }

    bool operator!=( time::unix_micro_timestamp b ) const
    {
        return value != b.value;
    }

    bool operator<( time::unix_micro_timestamp b ) const
    {
        return value < b.value;
    }

    bool operator<=( time::unix_micro_timestamp b ) const
    {
        return value <= b.value;
    }

    bool operator>( time::unix_micro_timestamp b ) const
    {
        return value > b.value;
    }

    bool operator>=( time::unix_micro_timestamp b ) const
    {
        return value >= b.value;
    }
    
    time::microseconds64    value;
};

// ******************************************************************
inline time::unix_micro_timestamp operator+(
    time::unix_micro_timestamp a, time::seconds b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::unix_micro_timestamp operator+(
    time::unix_micro_timestamp a, time::microseconds64 b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::unix_micro_timestamp operator-(
    time::unix_micro_timestamp a, time::seconds b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::unix_micro_timestamp operator-(
    time::unix_micro_timestamp a, time::microseconds64 b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::microseconds64 operator-(
    time::unix_micro_timestamp a,
    time::unix_micro_timestamp b )
// ******************************************************************
{
    return a.value - b.value;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct ntp_timestamp
//  NTP timestamp is a 64-bit fixed point number of seconds
//  from 0h on Jan 1, 1900 with 32-bit fractional part.
// ******************************************************************
{
    ntp_timestamp( void )
    :   value(0)
    {}

    ntp_timestamp& operator+=( time::seconds delta )
    {
        ali::uint64 seconds
            = ali::math::unsigned_abs(delta.value);
        
        seconds <<= 32;
        
        if ( delta.value < 0 )
            value -= seconds;
        else
            value += seconds;
        
        return *this;
    }
    
    ntp_timestamp& operator+=( time::milliseconds delta )
    {
        ali::uint64 seconds
            = ali::math::unsigned_abs(delta.value) / 1000U;
            
        seconds <<= 32;
        
        ali::uint64 fraction
            = ali::math::unsigned_abs(delta.value) % 1000U;
        
        fraction <<= 32;
        
        fraction /= 1000U;
        
        if ( delta.value < 0 )
            value -= seconds, value -= fraction;
        else
            value += seconds, value += fraction;
        
        return *this;
    }
    
    ntp_timestamp& operator-=( time::seconds delta )
    {
        ali::uint64 seconds
            = ali::math::unsigned_abs(delta.value);
        
        seconds <<= 32;
        
        if ( delta.value < 0 )
            value += seconds;
        else
            value -= seconds;
        
        return *this;
    }
    
    ntp_timestamp& operator-=( time::milliseconds delta )
    {
        ali::uint64 seconds
            = ali::math::unsigned_abs(delta.value) / 1000U;
            
        seconds <<= 32;
        
        ali::uint64 fraction
            = ali::math::unsigned_abs(delta.value) % 1000U;
        
        fraction <<= 32;
        
        fraction /= 1000U;
        
        if ( delta.value < 0 )
            value += seconds, value += fraction;
        else
            value -= seconds, value -= fraction;
        
        return *this;
    }
    
    bool operator==( ntp_timestamp b ) const
    {
        return value == b.value;
    }
    
    bool operator!=( ntp_timestamp b ) const
    {
        return value != b.value;
    }
    
    bool operator<( ntp_timestamp b ) const
    {
        return value < b.value;
    }
    
    bool operator<=( ntp_timestamp b ) const
    {
        return value <= b.value;
    }
    
    bool operator>( ntp_timestamp b ) const
    {
        return value > b.value;
    }
    
    bool operator>=( ntp_timestamp b ) const
    {
        return value >= b.value;
    }
    
    unsigned int seconds( void ) const
    {
        ali::uint64 temp = value; temp >>= 32;
        return static_cast<unsigned int>(temp) & 0xffffffffU;
    }
    
    unsigned int fraction( void ) const
    {
        return static_cast<unsigned int>(value) & 0xffffffffU;
    }
    
    ali::uint64 value;
};

// ******************************************************************
inline time::ntp_timestamp operator+(
    time::ntp_timestamp a, time::seconds b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::ntp_timestamp operator+(
    time::ntp_timestamp a, time::milliseconds b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::ntp_timestamp operator-(
    time::ntp_timestamp a, time::seconds b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::ntp_timestamp operator-(
    time::ntp_timestamp a, time::milliseconds b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct julian_day
// ******************************************************************
{
    explicit julian_day( double value = 0.0 )
    :   value(value)
    {}

    void swap( julian_day& b )
    {
        using ali::swap;
        swap(value, b.value);
    }

    friend void swap( julian_day& a, julian_day& b )
    {
        a.swap(b);
    }

    friend bool operator==( julian_day a, julian_day b )
    {
        return a.value == b.value;
    }

    friend bool operator!=( julian_day a, julian_day b )
    {
        return !(a == b);
    }

    friend bool operator<( julian_day a, julian_day b )
    {
        return a.value < b.value;
    }

    friend bool operator<=( julian_day a, julian_day b )
    {
        return a.value <= b.value;
    }

    friend bool operator>( julian_day a, julian_day b )
    {
        return a.value > b.value;
    }

    friend bool operator>=( julian_day a, julian_day b )
    {
        return a.value >= b.value;
    }

    friend double operator-( julian_day a, julian_day b )
    {
        return a.value - b.value;
    }

    friend int compare( julian_day a, julian_day b )
    {
        using ali::compare;
        return compare(a.value, b.value);
    }
    
    double  value;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct mac_absolute_time : time::float_seconds
// ******************************************************************
{
    mac_absolute_time( time::float_seconds value = time::float_seconds() )
    :   time::float_seconds(value)
    {}

    mac_absolute_time( double value )
    :   time::float_seconds(value)
    {}

    mac_absolute_time& operator+=( time::float_seconds delta )
    {
        value += delta.value;
        return *this;
    }

    mac_absolute_time& operator+=( time::seconds delta )
    {
        value += delta.value;
        return *this;
    }

    mac_absolute_time& operator+=( time::milliseconds delta )
    {
        value += static_cast<double>(delta.value) / 1000;
        return *this;
    }

    mac_absolute_time& operator+=( time::microseconds64 delta )
    {
        value += static_cast<double>(delta.value) / 1000000;
        return *this;
    }

    mac_absolute_time& operator-=( time::float_seconds delta )
    {
        value -= delta.value;
        return *this;
    }

    mac_absolute_time& operator-=( time::seconds delta )
    {
        value -= delta.value;
        return *this;
    }

    mac_absolute_time& operator-=( time::milliseconds delta )
    {
        value -= static_cast<double>(delta.value) / 1000;
        return *this;
    }

    mac_absolute_time& operator-=( time::microseconds64 delta )
    {
        value -= static_cast<double>(delta.value) / 1000000;
        return *this;
    }


};

// ******************************************************************
inline time::mac_absolute_time operator+(
    time::mac_absolute_time a, time::float_seconds b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::mac_absolute_time operator+(
    time::mac_absolute_time a, time::seconds b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::mac_absolute_time operator+(
    time::mac_absolute_time a, time::milliseconds b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::mac_absolute_time operator+(
    time::mac_absolute_time a, time::microseconds64 b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::mac_absolute_time operator-(
    time::mac_absolute_time a, time::float_seconds b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::mac_absolute_time operator-(
    time::mac_absolute_time a, time::seconds b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::mac_absolute_time operator-(
    time::mac_absolute_time a, time::milliseconds b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::mac_absolute_time operator-(
    time::mac_absolute_time a, time::microseconds64 b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
// ******************************************************************
    
// ******************************************************************
struct parts
// ******************************************************************
{
    parts( void )
    :   year(0),
        month(0),
        day(0),
        hour(0),
        minute(0),
        second(0),
        millisecond(0)
    {}

    static bool leap_year( short year );
    
    bool leap_year( void ) const
    {
        return leap_year(year);
    }
    
    static short days_in_month(
        short year, short month );

    short days_in_month( void ) const
    {
        return days_in_month(year, month);
    }
    
    bool is_valid( void ) const;

    void swap( parts& b )
    {
        ali::swap_by_copy(*this, b);
    }

    friend void swap( parts& a, parts& b )
    {
        a.swap(b);
    }
    
    short year;
    short month;        //  1=January..12=December
    short day;          //  1..days_in_month()
    short hour;         //  0..23
    short minute;       //  0..59
    short second;       //  0..59
    short millisecond;  //  0..999
};

// ******************************************************************
inline bool operator==( time::parts const& a, time::parts const& b )
// ******************************************************************
{
    return a.millisecond == b.millisecond
        && a.second == b.second
        && a.minute == b.minute
        && a.hour == b.hour
        && a.day == b.day
        && a.month == b.month
        && a.year == b.year;
}

// ******************************************************************
inline bool operator!=( time::parts const& a, time::parts const& b )
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
bool operator<( time::parts const& a, time::parts const& b );
// ******************************************************************

// ******************************************************************
inline bool operator<=( time::parts const& a, time::parts const& b )
// ******************************************************************
{
    //  a <= b --> !(a > b) --> !(b < a)

    return !(b < a);
}

// ******************************************************************
inline bool operator>( time::parts const& a, time::parts const& b )
// ******************************************************************
{
    return b < a;
}

// ******************************************************************
inline bool operator>=( time::parts const& a, time::parts const& b )
// ******************************************************************
{
    //  a >= b --> !(a < b)
    
    return !(a < b);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
time::unix_timestamp& convert(
    time::unix_timestamp& timestamp,
    time::unix_timestamp64 t );
// ******************************************************************

// ******************************************************************
time::unix_timestamp& convert(
    time::unix_timestamp& timestamp,
    time::unix_micro_timestamp t );
// ******************************************************************

// ******************************************************************
time::unix_timestamp& convert(
    time::unix_timestamp& timestamp,
    time::mac_absolute_time t );
// ******************************************************************

// ******************************************************************
time::unix_timestamp& convert(
    time::unix_timestamp& timestamp,
    time::parts const& parts );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
inline time::unix_timestamp64& convert(
    time::unix_timestamp64& timestamp,
    time::unix_timestamp t )
// ******************************************************************
{
    timestamp.value.value = t.value.value;
    return timestamp;
}

// ******************************************************************
time::unix_timestamp64& convert(
    time::unix_timestamp64& timestamp,
    time::mac_absolute_time t );
// ******************************************************************

// ******************************************************************
time::unix_timestamp64& convert(
    time::unix_timestamp64& timestamp,
    time::parts const& parts );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
inline time::unix_micro_timestamp& convert(
    time::unix_micro_timestamp& timestamp,
    time::unix_timestamp t )
// ******************************************************************
{
    timestamp.value.value = t.value.value;
    timestamp.value.value *= 1000000;
    return timestamp;
}

// ******************************************************************
time::unix_micro_timestamp& convert(
    time::unix_micro_timestamp& timestamp,
    time::mac_absolute_time t );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
time::mac_absolute_time& convert(
    time::mac_absolute_time& timestamp,
    time::unix_timestamp t );
// ******************************************************************

// ******************************************************************
time::mac_absolute_time& convert(
     time::mac_absolute_time& timestamp,
     time::unix_micro_timestamp t );
// ******************************************************************

// ******************************************************************
time::mac_absolute_time& convert(
    time::mac_absolute_time& timestamp,
    time::parts const& parts );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
time::parts& convert(
    time::parts& parts,
    time::julian_day jd );
// ******************************************************************

// ******************************************************************
time::parts& convert(
    time::parts& parts,
    time::unix_timestamp timestamp );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::parts& convert(
    time::parts& parts,
    time::unix_timestamp64 timestamp );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::parts& convert(
    time::parts& parts,
    time::unix_micro_timestamp timestamp );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::parts& convert(
     time::parts& parts,
     time::mac_absolute_time mat );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
time::ntp_timestamp& convert(
    time::ntp_timestamp& ntp,
    time::unix_micro_timestamp timestamp );
// ******************************************************************

// ******************************************************************
time::unix_micro_timestamp& convert(
    time::unix_micro_timestamp& micro_timestamp,
    time::ntp_timestamp ntp );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
time::julian_day& convert(
    time::julian_day& jd,
    time::parts parts );
// ******************************************************************

// ******************************************************************
// ******************************************************************
   
// ******************************************************************
template <class T>
class timer
// ******************************************************************
{
public:
    void start( void );
    T read( void ) const;
    T stop( void );
};

const double MacAbsoluteTimeIntervalSince1970 = 978307200.0;

// ******************************************************************
namespace day_of_week
// ******************************************************************
{

// ******************************************************************
enum type {mon,tue,wed,thu,fri,sat,sun};
// ******************************************************************

// ******************************************************************
day_of_week::type get( time::julian_day jd );
// ******************************************************************

// ******************************************************************
inline day_of_week::type get( time::parts const& parts )
// ******************************************************************
{
    time::julian_day jd;
    return day_of_week::get(time::convert(jd, parts));
}

// ******************************************************************
inline day_of_week::type get( time::unix_timestamp timestamp )
// ******************************************************************
{
    time::parts parts;
    return day_of_week::get(time::convert(parts, timestamp));
}


}   //  namespace day_of_week

namespace current
{

namespace gmt
{

// ******************************************************************
time::unix_timestamp as_unix_timestamp( void );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::unix_timestamp64 as_unix_timestamp64( void );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::unix_micro_timestamp as_unix_micro_timestamp( void );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::parts as_parts( void );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::mac_absolute_time as_mac_absolute_time( void );
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
inline time::ntp_timestamp as_ntp_timestamp( void )
// ******************************************************************
{
    time::ntp_timestamp timestamp;
    return time::convert(timestamp, as_unix_micro_timestamp());
}

// ******************************************************************
inline time::julian_day as_julian_day( void )
// ******************************************************************
{
    time::julian_day jd;
    return time::convert(jd, as_parts());
}

}   //  namespace gmt

namespace local
{

// ******************************************************************
time::unix_timestamp as_unix_timestamp( void );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::unix_timestamp64 as_unix_timestamp64( void );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::unix_micro_timestamp as_unix_micro_timestamp( void );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::parts as_parts( void );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::mac_absolute_time as_mac_absolute_time( void );
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
inline time::ntp_timestamp as_ntp_timestamp( void )
// ******************************************************************
{
    time::ntp_timestamp timestamp;
    return time::convert(timestamp, as_unix_micro_timestamp());
}

// ******************************************************************
inline time::julian_day as_julian_day( void )
// ******************************************************************
{
    time::julian_day jd;
    return time::convert(jd, as_parts());
}

}   //  namespace local

}   //  namespace current

namespace local
{

// ******************************************************************
time::minutes gmt_offset( void );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::unix_timestamp from_gmt( time::unix_timestamp timestamp );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

}   //  namespace local

namespace rfc1123
{

ali::string format( time::parts const& parts );
    
bool parse(
           time::parts& parts,
           time::minutes& shift,
           ali::string const& str );

}

namespace iso8601w3c // See: http://www.w3.org/TR/NOTE-datetime
{

namespace format_option
{

enum type
{
    year, year_month, date,
    date_hours_minutes,
    date_hours_minutes_seconds,
    date_hours_minutes_seconds_milliseconds,
};

}   //  namespace format

// ******************************************************************
ali::string format( time::parts const& parts,
    format_option::type fmt = format_option::date_hours_minutes_seconds_milliseconds );
// ******************************************************************

// ******************************************************************
ali::string format( time::parts const& parts, time::minutes shift,
    format_option::type fmt = format_option::date_hours_minutes_seconds_milliseconds );
// ******************************************************************

}   //  namespace iso8601w3c

namespace rfc3339
{

// ******************************************************************
ali::string format( time::parts const& parts );
// ******************************************************************

// ******************************************************************
ali::string format( time::parts const& parts, time::minutes shift );
// ******************************************************************
//  When shift != 0, then the parts structure shall contain
//  LOCAL time and the shift shall be "LOCAL time" - "UTC time".
// ******************************************************************

// ******************************************************************
bool parse(
    time::parts& parts,
    time::minutes& shift,
    ali::string const& str );
// ******************************************************************
//  When shift != 0, then the parts structure contains LOCAL time.
//  To obtain UTC time, one needs to SUBTRACT the shift minutes.
// ******************************************************************

}   //  namespace rfc3339

// ******************************************************************
array<bool> test( void );
// ******************************************************************

}   //  namespace time

}   //  namespace ali
