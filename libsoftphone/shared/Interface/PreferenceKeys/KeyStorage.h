#pragma once
#include "ali_string.h"

namespace PreferenceKeys
{
	class BasicKey;
	/*
	An interface used by GenericBasicKey to read and write key-value pairs, where
	both key and value are ali::strings
	*/
	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	class KeyStorage
	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	{
    public:
        virtual ~KeyStorage()
        {}

		virtual ali::string const& getValueForPreferenceKey(ali::string const& key) const = 0;
		virtual void setValueForPreferenceKey(ali::string const& key, ali::string const& val) = 0;
	};
} // namespace Settings

