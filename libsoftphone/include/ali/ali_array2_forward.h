#pragma once
#include "ali_integer.h"

namespace ali
{

// ******************************************************************
template <typename T, int capacity = 0>
class array;
// ******************************************************************

// ******************************************************************
template <int capacity>
class array<ali::uint8, capacity>;
// ******************************************************************

}   //  namespace ali
