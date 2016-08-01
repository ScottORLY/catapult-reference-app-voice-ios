/*
*  ali_exception_if.h
*  ali Library
*
*  Copyright 2012 Acrobits, s.r.o. All rights reserved.
*
*/

#pragma once

#include "ali_attribute.h"
#include "ali_location.h"
#include "ali_nullptr.h"
#include "ali_tstring_forward.h"
#include "ali_exception_memory.h"


namespace ali
{

// ******************************************************************
void general_error(
    tstring_const_ref descr,
    ali::location location ) ALI_ATTRIBUTE_NORETURN;
// ******************************************************************

// ******************************************************************
void general_error_if(
    bool error,
    tstring_const_ref descr,
    ali::location location );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
void syntax_error(
    tstring_const_ref descr,
    ali::location location ) ALI_ATTRIBUTE_NORETURN;
// ******************************************************************

// ******************************************************************
void syntax_error_if(
    bool error,
    tstring_const_ref descr,
    ali::location location );
// ******************************************************************

// ******************************************************************
// ******************************************************************

//  Currently this will link as long as T is int.
//  For other Ts another explicit instantiation
//  is needed in the cpp file.

// ******************************************************************
template <typename T>
void range_error(
    T cur, T begin, T end,
    tstring_const_ref descr,
    ali::location location ) /*ALI_ATTRIBUTE_NORETURN*/;
// ******************************************************************

// ******************************************************************
template <typename T>
void range_error_if(
    T cur, T begin, T end,
    tstring_const_ref descr,
    ali::location location );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
void optional_is_null(
    ali::location location ) ALI_ATTRIBUTE_NORETURN;
// ******************************************************************

// ******************************************************************
inline void optional_is_null_if(
    bool error,
    ali::location location )
// ******************************************************************
{
	if ( error )
		optional_is_null(location);
}

}   //  namespace ali

#define ALI_X_GENERAL_IF(expression)    \
    ali::general_error_if(expression, _T(#expression), ALI_HERE)
