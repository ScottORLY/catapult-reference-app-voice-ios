/*
 *  EventHistory/EventHistoryTypes.h
 *  libsoftphone
 *
 *  Copyright (c) 2013, 2014 Acrobits s.r.o. All rights reserved.
 */

#pragma once

#include "ali_string.h"
#include "ali_time.h"

namespace Softphone {
namespace EventHistory {

    typedef ali::string                     ContactIdType;
    typedef unsigned long                   EventIdType;
    typedef ali::time::mac_absolute_time    TimestampType;

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct EventType
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        enum Type
        {
            Unknown,
            Call,
            Message,
            FileTransfer,
            Location,
            ParticipantAction
        };

        static ali::string_literal toString(EventType::Type evt);
    };

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct Direction
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        enum Type
        {
            Unspecified = 0,
            Incoming = 1 << 0,
            Outgoing = 1 << 1,

            max = Outgoing,
            all = (max << 1) - 1
        };

        static ali::string_literal toString(Direction::Type dir);
    };

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct DeliveryStatus
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        enum Type
        {
            Unspecified = 0,
            Pending = 1,
            Sent = 2,
            Delivered = 3,
            Read = 4,
            Error = 5
        };

        static ali::string_literal toString(DeliveryStatus::Type status);
    };

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct StorageStatus
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        enum Type
        {
            New = 0,
            Stored = 1,
            Dirty = 2,
            Removed = 3
        };

        static ali::string_literal toString(StorageStatus::Type status);
    };
}
}
