/*
 *  AudioRoute.h
 *  softphone
 *
 *  Created by Jiri Kral on 1/9/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once


namespace AudioRoute
{

enum Type
{
	Receiver = 0,
    Headset = 4, /// may be Receiver if detection is not supported
	Speaker = 1,
	BluetoothSCO = 2,
	BluetoothA2DP = 3
};

char const* toString( Type );

}


namespace ali {

template <typename T>
struct format_t;

template <>
struct format_t<AudioRoute::Type>
{
	char const* operator ()(AudioRoute::Type val) const
	{
		return AudioRoute::toString(val);
	}
};

}
