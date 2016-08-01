/*
 *  ali_throw.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_debug.h"
#include "ali_nullptr.h"
/*
#define ALI_TRY         try
#define ALI_CATCH(e)    catch ( e )
#define ALI_THROW_LOC(e, loc)    throw (e) ^ (loc)
#define ALI_RETHROW     throw
*/

#define ALI_NORETURN
#define ALI_TRY         if ( true )
#define ALI_CATCH(t, v) else if ( false ) for ( const t &v = *static_cast<const t*>(nullptr); false; )
#define ALI_CATCH_ALL   else
#define ALI_THROW_LOC(e, l) ali::mac::assertion_failure(e.what().c_str(), l)
#define ALI_RETHROW
