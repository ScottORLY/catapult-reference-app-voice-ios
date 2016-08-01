#pragma once

#include "ali_array_utils_forward.h"
#include "ali_string_forward.h"
#include "ali_tchar.h"

namespace ali
{

#ifdef UNICODE

typedef wstring_ref         tstring_ref;
typedef wstring_ptr         tstring_ptr;
typedef wstring_const_ref   tstring_const_ref;
typedef wstring_const_ptr   tstring_const_ptr;
typedef c_wstring_const_ref c_tstring_const_ref;
typedef c_wstring_const_ptr c_tstring_const_ptr;
typedef wstring_literal     tstring_literal;
typedef wstring             tstring;

#else

typedef string_ref          tstring_ref;
typedef string_ptr          tstring_ptr;
typedef string_const_ref    tstring_const_ref;
typedef string_const_ptr    tstring_const_ptr;
typedef c_string_const_ref  c_tstring_const_ref;
typedef c_string_const_ptr  c_tstring_const_ptr;
typedef string_literal      tstring_literal;
typedef string              tstring;

#endif  //  UNICODE

}   //  namespace ali

