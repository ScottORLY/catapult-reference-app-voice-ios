/*
*  ali_exception_memory.h
*  ali Library
*
*  Copyright 2012 Acrobits, s.r.o. All rights reserved.
*
*/

#pragma once

#include "ali_attribute.h"
#include "ali_location.h"


namespace ali
{


// ******************************************************************
void out_of_memory( ali::location location ) ALI_ATTRIBUTE_NORETURN;
// ******************************************************************

// ******************************************************************
inline void out_of_memory_if( bool error, ali::location location )
// ******************************************************************
{
	if ( error )
		out_of_memory(location);
}

// ******************************************************************
template <typename T>
inline T out_of_memory_if_null( T t, ali::location location )
// ******************************************************************
{
	out_of_memory_if(t == nullptr, location);
	return t;
}

}   //  namespace ali
