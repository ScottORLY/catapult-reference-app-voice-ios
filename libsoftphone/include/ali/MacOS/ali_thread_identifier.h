/*
 *  ali_thread_identifier.h
 *  ali Library
 *
 *  Copyright 2013 Acrobits, s.r.o. All rights reserved.
 *
 */


#pragma once

#include <pthread.h>


namespace ali {
namespace thread {

// ******************************************************************
class identifier
// ******************************************************************
{
public:
    identifier( void )
        :   _id(::pthread_self())
    {}

    explicit identifier( pthread_t id )
        :   _id(id)
    {}

    bool operator==( identifier b ) const
    {
        return ::pthread_equal(_id, b._id) != 0;
    }

    bool operator!=( identifier b ) const
    {
        return !operator==(b);
    }

private:
    pthread_t   _id;
};

}
}
