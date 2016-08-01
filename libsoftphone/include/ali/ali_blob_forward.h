#pragma once
#include "ali_array_forward.h"

#ifdef  ALI_STREAMLINED_ARRAYS

#define ALI_BLOB_IS_ARRAY

#endif  //  ALI_STREAMLINED_ARRAYS


namespace ali
{

#ifdef  ALI_BLOB_IS_ARRAY

// ******************************************************************
typedef array<ali::uint8> blob;
// ******************************************************************

#else   //  !ALI_BLOB_IS_ARRAY

// ******************************************************************
class blob;
// ******************************************************************

#endif  //  ALI_BLOB_IS_ARRAY

}   //  namespace ali
