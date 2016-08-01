/*
 *  ali_tchar.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_wchar.h"

namespace ali
{

#ifdef UNICODE

typedef wchar            tchar;

#define _TT(x)           L ## x
#define _T(x)            _TT(x)

#else

typedef char            tchar;

#define _TT(x)          x
#define _T(x)           _TT(x)

#endif // UNICODE

}


#define _STR2(x)        #x
#define _STR(x)         _STR2(x)

#define _WSTR2(x)       L ## #x
#define _WSTR(x)        _WSTR2(x)

#define _TSTR2(x)       _T(#x)
#define _TSTR(x)        _TSTR2(x)
