/*
 *  ali_nullptr.h
 *  ali library
 *
 *  Copyright 2011 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_features.h"

#if defined(ALI_FEATURE_NULLPTR)

#include <cstddef>

namespace ali
{

typedef std::nullptr_t  nullptr_type;

}

#else

namespace ali
{

// ******************************************************************
// Null pointer type
// ******************************************************************
struct nullptr_type
{
public:
    template <class T>
    inline operator T*() const
    {
#   if defined(__GNUC__)
        return __null;
#   else
        return 0;
#   endif
    }

    template <class C,
              class T>
    inline operator T C::*() const
    {
#   if defined(__GNUC__)
        return __null;
#   else
        return 0;
#   endif
    }

    // Cannot take the address of nullptr
#   if defined(ALI_FEATURE_DELETED_FUNCTIONS)
    void operator&() const = delete;
#   else
private:
    void operator&() const;
#   endif
};


// ******************************************************************
// Null pointer
// ******************************************************************
extern const nullptr_type nullptr;

}

using ali::nullptr;

#endif
