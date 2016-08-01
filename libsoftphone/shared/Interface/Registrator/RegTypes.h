//
//  RegTypes.h
//  libsoftphone
//
//  Created by Jiri Kral on 4/27/12.
//  Copyright (c) 2012 Acrobits. All rights reserved.
//

#pragma once

#include "ali_string.h"

namespace Registrator
{
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct State
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        enum Type
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            None, Discovering, NotRegistered, PushHandshake, Registering,
            Registered, Unregistering, Unauthorized, Error
        };

        static ali::string toString(Type state);
    };
}


namespace ali
{
template <class T>
struct format_t;

template <>
struct format_t<Registrator::State::Type>
{
    ali::string operator ()(const Registrator::State::Type &value) const
    {
        return Registrator::State::toString(value);
    }
};
}
