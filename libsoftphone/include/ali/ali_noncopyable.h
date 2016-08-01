/*
 *  ali_noncopyable.h
 *  ali library
 *
 *  Copyright (c) 2011 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_features.h"

namespace ali {

// ******************************************************************
class noncopyable
// ******************************************************************
// A class that cannot be copied
// ******************************************************************
{
public:
    noncopyable()
    {}

#if defined(ALI_FEATURE_DELETED_FUNCTIONS)
    noncopyable(noncopyable const&) = delete;
    noncopyable& operator =(noncopyable const&) = delete;
#else
private:
    noncopyable(noncopyable const&);
    noncopyable& operator =(noncopyable const&);
#endif
};

}
