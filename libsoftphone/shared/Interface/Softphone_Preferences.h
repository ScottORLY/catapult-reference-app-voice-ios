/*
 *  Softphone_Preferences.h
 *  libsoftphone
 *
 *  Copyright (c) 2013, 2014 Acrobits s.r.o. All rights reserved.
 *
 */

#pragma once

#include "RingerSetting.h"


namespace PreferenceKeys
{
    struct Keys;
    template <typename Type>
    class GenericBasicROKey;
    template <typename Type>
    class GenericBasicKey;
}

namespace Softphone
{
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct Preferences
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        template <typename Type>
        class ROKey
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
        public:
            //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
            ROKey(PreferenceKeys::GenericBasicROKey<Type> & key)
            //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
                : mKey(key)
            {}

            Type get() const;
            Type getDefault( void ) const;
            void overrideDefault(Type const& value);

        protected:
            PreferenceKeys::GenericBasicROKey<Type> & mKey;
        };

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        template <typename Type>
        class Key
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
            : public ROKey<Type>
        {
        public:
            typedef ROKey<Type> Super;

            Key(PreferenceKeys::GenericBasicKey<Type> & key)
                : Super(key)
            {}

            void set(Type const& value);
            void reset();
        };


        explicit Preferences(PreferenceKeys::Keys & keys);

        Key<ali::string>         keepAwake;
        Key<bool>                trafficLogging;
        Key<bool>                echoSuppressionEnabled;
        Key<ali::string>         networkUse;
        Key<int>                 volumeBoostMicrophone;
        Key<int>                 volumeBoostPlayback;
        Key<int>                 keypadVolume;

        ROKey<ali::string>       userAgent;
        ROKey<bool>              voicemailEnabled;
        Key<int>                 audioRtpTosByte;
        Key<int>                 videoRtpTosByte;
        Key<int>                 rtcpTosByte;

        Key<bool>                backgroundNoiseSuppression;
        Key<bool>                playDtmfEvenIfMuted;
        Key<ali::string>         installationSalt;
        //ROKey<bool>            switchAudioSessionDuringTrying;
        Key<RingerSetting>       ringerSettingOverride;
        Key<bool>                verifyCertificates;
        ROKey<bool>              sipisDisabled;
        ROKey<bool>              pushNotificationsEnabled;

#if defined(__APPLE__)
        Key<bool>                bgrUsesPushRingtone;
        ROKey<bool>              pushNotificationsUsePushKit;
#endif

        Key<bool>				 recordWarningBeep;
        Key<int>				 dtmfDurationInMilliseconds;
        Key<int>                 dtmfMinimumGapInMilliseconds;
        Key<int>				 dtmfPauseInMilliseconds;
        Key<bool>				 screenPushedCalls;
        ROKey<ali::string>       contactSourceType;
    };

    extern template class Preferences::ROKey<bool>;
    extern template class Preferences::ROKey<int>;
    extern template class Preferences::ROKey<ali::string>;
    extern template class Preferences::ROKey<RingerSetting>;
    extern template class Preferences::Key<bool>;
    extern template class Preferences::Key<int>;
    extern template class Preferences::Key<ali::string>;
    extern template class Preferences::Key<RingerSetting>;
}

