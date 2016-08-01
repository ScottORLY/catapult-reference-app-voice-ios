/*
 *  ali_exception.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_array_utils_forward.h"
#include "ali_attribute.h"
#include "ali_backtrace.h"
#include "ali_nullptr.h"
#include "ali_tstring_forward.h"
#include "ali_throw.h"
#include "ali_location.h"


namespace ali
{

// ******************************************************************
class exception_trace_item;
// ******************************************************************

// ******************************************************************
ali::location exception_trace_item_location(
    exception_trace_item const& item );
// ******************************************************************

// ******************************************************************
tstring_const_ref exception_trace_item_text(
    exception_trace_item const& item );
// ******************************************************************

// ******************************************************************
exception_trace_item const* exception_trace_item_next(
    exception_trace_item const& item );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
class exception_trace_item_holder
// ******************************************************************
{
public:
    exception_trace_item_holder( void ) {}

    exception_trace_item_holder(
        exception_trace_item_holder const& b );

    ~exception_trace_item_holder( void );

    void operator()(
        ali::location location,
        tstring_const_ref text );

/*
#if defined(ALI_FEATURE_VARIADIC_TEMPLATES)
    // ******************************************************************
    template <typename... T>
    void operator ()( ali::location const& location,
                      tstring_const_ref format,
                      T... params )
    // ******************************************************************
    {
        tformatter fmt(format);
        ali::for_each(fmt, params...);
        operator ()(location, fmt.str());
    }
#endif
*/

    exception_trace_item const* get( void ) const;

protected:  // Data members
    struct data;
    data*   _data{};
    
private:    //  Methods
    exception_trace_item_holder& operator=(
        exception_trace_item_holder const& );
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class safe_exception_string
// ******************************************************************
//  Reference counted string wrapper whose copy constructor and
//  assignment operator doesn't throw.
// ******************************************************************
{
public:
    safe_exception_string( tstring_const_ref str );

    safe_exception_string( safe_exception_string const& b );

    ~safe_exception_string( void );

    safe_exception_string& operator=( safe_exception_string b );

    tstring_const_ref get( void ) const;

    //void swap( safe_exception_string& b )
    //{
    //    using ali::swap;
    //    swap(_data, b._data);
    //}
    //
    //friend void swap(
    //    safe_exception_string& a,
    //    safe_exception_string& b )
    //{
    //    a.swap(b);
    //}
    
private:    // Data members
    struct data;
    data*   _data{};
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class exception
// ******************************************************************
//  The ultimate base class for all exceptions.
// ******************************************************************
{
public:
    virtual ~exception( void ) {}

    virtual ali::tstring& append_description( ali::tstring& descr ) const = 0;
    ali::tstring& append_located_description( ali::tstring& descr ) const;

    ali::tstring what( void ) const throw();


    mutable ali::location location; ///< Location where the exception was thrown
    mutable exception_trace_item_holder trace; ///< Application trace
    ali::backtrace::storage backtrace; ///< Backtrace when the exception was thrown
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename Ex>
const Ex& operator^( const Ex &ex, ali::location location )
// ******************************************************************
{
	ex.location = location;
	return ex;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
class general_exception : public exception
// ******************************************************************
{
public:
    general_exception( tstring_const_ref descr );

    virtual ali::tstring& append_description(
        ali::tstring& descr ) const override;

private:
    safe_exception_string   _descr;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class syntax_error_exception : public exception
// ******************************************************************
{
public:
    syntax_error_exception( tstring_const_ref descr );

    virtual ali::tstring& append_description(
        ali::tstring& descr ) const override;

private:
    safe_exception_string   _descr;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class basic_range_error_exception : public exception
// ******************************************************************
{
public:
    basic_range_error_exception( tstring_const_ref const& descr );

    virtual ali::tstring& append_description(
        ali::tstring& descr ) const override;

private:
    safe_exception_string   _descr;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class range_error_exception : public basic_range_error_exception
// ******************************************************************
{
public:
    range_error_exception(
        tstring_const_ref const& descr, T cur, T begin, T end )
    :   basic_range_error_exception{descr},
        _cur{cur},
        _begin{begin},
        _end{end}
    {}

    T current( void ) const
    {
        return _cur;
    }

    T begin( void ) const
    {
        return _begin;
    }

    T end( void ) const
    {
        return _end;
    }

private:    //  Data members
    T   _cur;
    T   _begin;
    T   _end;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class out_of_memory_exception : public exception
// ******************************************************************
{
public:
    virtual ali::tstring& append_description(
        ali::tstring& descr ) const override;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class optional_is_null_exception : public exception
// ******************************************************************
{
public:
    virtual ali::tstring& append_description(
        ali::tstring& descr ) const override;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************    
template <typename>
struct format_t;
// ******************************************************************

// ******************************************************************
template <>
struct format_t<ali::exception>
// ******************************************************************
{
    ali::tstring operator ()(const ali::exception &e) const;
};

// ******************************************************************
template <>
struct format_t<ali::general_exception>
// ******************************************************************
    : public format_t<ali::exception> {};

// ******************************************************************
template <>
struct format_t<ali::syntax_error_exception>
// ******************************************************************
    : public format_t<ali::exception> {};

// ******************************************************************
template <>
struct format_t<ali::basic_range_error_exception>
// ******************************************************************
    : public format_t<ali::exception> {};

// ******************************************************************
template <typename T>
struct format_t<ali::range_error_exception<T> >
// ******************************************************************
    : public format_t<ali::exception> {};

// ******************************************************************
template <>
struct format_t<ali::out_of_memory_exception>
// ******************************************************************
    : public format_t<ali::exception> {};

// ******************************************************************
template <>
struct format_t<ali::optional_is_null_exception>
// ******************************************************************
    : public format_t<ali::exception> {};

}   //  namespace ali
