/*
 *  ali_safebool.h
 *  ali Library
 *
 *  Copyright 2012 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_debug.h"
#include "ali_nullptr.h"


namespace ali
{

// ******************************************************************
struct safebool_t
// ******************************************************************
{
    // ******************************************************************
    void dummy( void )
    // ******************************************************************
    {
        ali_assert(false);
    }
};


/* Safe boolean that can be used in if and ternary operators but cannot be
 * converted to anything wrong */
typedef void(safebool_t::*safebool)();

// ******************************************************************
inline safebool safeboolize( bool value )
// ******************************************************************
{
    return value ? &safebool_t::dummy : nullptr;
}

} // namespace ali
