#pragma once

#include "ali_meta.h"

namespace ali
{

typedef meta::integer::signed_of_exact_size_in_bits<8>::type        int8;
typedef meta::integer::signed_of_exact_size_in_bits<16>::type       int16;
typedef meta::integer::signed_of_exact_size_in_bits<32>::type       int32;
typedef meta::integer::signed_of_exact_size_in_bits<64>::type       int64;

typedef meta::integer::unsigned_of_exact_size_in_bits<8>::type      uint8;
typedef meta::integer::unsigned_of_exact_size_in_bits<16>::type     uint16;
typedef meta::integer::unsigned_of_exact_size_in_bits<32>::type     uint32;
typedef meta::integer::unsigned_of_exact_size_in_bits<64>::type     uint64;

typedef meta::integer::unsigned_of_exact_size_in_bits<
    meta::size_in_bits<void*>::result>::type                        uint_of_pointer_size;

}   //  namespace ali
