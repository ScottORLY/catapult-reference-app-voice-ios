//
//  CallTypes.h
//  libsoftphone
//
//  Created by Jiri Kral on 4/27/12.
//  Copyright (c) 2012 Acrobits. All rights reserved.
//

#pragma once

#include "ali_str_builder.h"
#include "ali_time_common.h"

namespace Call
{
	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	struct State
	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	{
		//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
		enum Type
		//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
		{
			Unknown,
			Trying,
			Ringing,
			Busy,
			IncomingTrying,
			IncomingRinging,
			IncomingIgnored,
			IncomingRejected,
			IncomingMissed,
			Established,
			Error,
			Unauthorized,
			Terminated,
			IncomingForwarded,
			IncomingAnsweredElsewhere,
            RedirectedToAlternativeService
		};

        static ali::string_literal toString(Type state);
		static bool isTerminal(Type state);
	};

	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	struct HoldState
	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	{
		//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
		enum Type
		//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
		{
			None,
			Active,
			Pending,
			Held
		};
	};

	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	struct HoldStates
	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	{
        HoldStates(ali::nullptr_type const& = nullptr)
        :local(HoldState::None)
        ,remote(HoldState::None)
        {}

		HoldStates(HoldState::Type l, HoldState::Type r)
		:local(l),remote(r){}

		HoldState::Type local;
		HoldState::Type remote;
	};

    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	struct Contact
	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	{
		ali::string scheme;
		ali::string userDisplayName;
		ali::string userName;
		ali::string domain;
		ali::string domainPort;

        ali::string phonebookRecordId;
	};

    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct RecordingStatus
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        enum State
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            Initial,
            Recording,
            Paused
        };

        ali::time::float_seconds duration;
        State state;
    };

    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct MediaStatus
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        struct StreamStatus
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            StreamStatus( void )
            :   incomingActive(false),
                outgoingActive(false)
            {}

            bool operator==( StreamStatus const& b ) const
            {
                return  incomingActive == b.incomingActive
                &&  outgoingActive == b.outgoingActive
                &&  incomingPayloadNumber == b.incomingPayloadNumber
                &&  outgoingPayloadNumber == b.outgoingPayloadNumber;
            }

            bool operator!=( StreamStatus const& b ) const
            {
                return !operator==(b);
            }

            bool        incomingActive;
            bool        outgoingActive;
            ali::string incomingPayloadNumber;
            ali::string outgoingPayloadNumber;
        };

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        struct AudioStreamStatus : public StreamStatus
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            bool operator==( AudioStreamStatus const& b ) const
            {
                return StreamStatus::operator==(b)
                && mIncomingEnergy == b.mIncomingEnergy;
            }
            
            bool operator !=( AudioStreamStatus const& b ) const
            {
                return !operator==(b);
            }
            
            float mIncomingEnergy{0.0f};
        };
        
        bool operator==( MediaStatus const& b ) const
        {
            return  audio == b.audio
            &&  video == b.video;
        }

        bool operator!=( MediaStatus const& b ) const
        {
            return !operator==(b);
        }

        AudioStreamStatus    audio;
        StreamStatus         video;
    };
}


namespace ali
{
    template <class T>
    struct format_t;

    template <>
    struct format_t<Call::State::Type>
    {
        ali::string operator ()(const Call::State::Type &state) const
        {
            return Call::State::toString(state);
        }
    };

    template <>
    struct format_t<Call::MediaStatus::StreamStatus>
    {
        ali::string operator ()(const Call::MediaStatus::StreamStatus &status) const
        {
            return ali::str::builder("incoming ")(status.incomingActive ? "active" : "inactive")(" ")(status.incomingPayloadNumber)(", ")
                ("outgoing ")(status.outgoingActive ? "active" : "inactive")(" ")(status.outgoingPayloadNumber).str();
        }
    };

    template <>
    struct format_t<Call::MediaStatus::AudioStreamStatus>
    {
        ali::string operator ()(const Call::MediaStatus::AudioStreamStatus &status) const
        {
            return ali::str::builder("incoming ")(status.incomingActive ? "active" : "inactive")(" ")(status.incomingPayloadNumber)(", ")
            ("outgoing ")(status.outgoingActive ? "active" : "inactive")(" ")(status.outgoingPayloadNumber)(" energy=")(status.mIncomingEnergy).str();
        }
    };

    template <>
    struct format_t<Call::MediaStatus>
    {
        ali::string operator ()(const Call::MediaStatus &status) const
        {
            return ali::str::builder("audio {")(status.audio)("}, ")
                ("video {")(status.video)("}").str();
        }
    };
}
