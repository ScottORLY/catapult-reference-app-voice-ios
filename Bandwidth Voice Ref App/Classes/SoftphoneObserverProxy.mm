/*
 *  SoftphoneObserverProxy.cpp
 *  demophone
 *
 *  Created by jiri on 4/1/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SoftphoneObserverProxy.h"

// ******************************************************************
SoftphoneObserverProxy::SoftphoneObserverProxy(NSObject<SoftphoneDelegate> * delegate)
:_delegate(delegate)
// ******************************************************************
{
	_delegate = delegate;
}

// ******************************************************************
SoftphoneObserverProxy::~SoftphoneObserverProxy()
// ******************************************************************
{
	_delegate = nil;
}

#if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
// ******************************************************************
void SoftphoneObserverProxy::onRegistrationStateChanged(ali::string const& accountId, Registrator::State::Type state)
// ******************************************************************
{
    [_delegate softphoneRegistrationStateChanged:state forAccount:accountId];
}
#else
// ******************************************************************
void SoftphoneObserverProxy::onRegistrationStateChanged(Registrator::State::Type state)
// ******************************************************************
{
    [_delegate softphoneRegistrationStateChanged:state];
}
#endif

// ******************************************************************
void SoftphoneObserverProxy::onNetworkChangeDetected(Softphone::Network::Type network)
// ******************************************************************
{
    // this may be used to reflect the change in GUI, but all reinvites and
    // re-registrations are handled internally by libsoftphone
}

// ******************************************************************
void SoftphoneObserverProxy::onIncomingEvent(Softphone::EventHistory::Event::Pointer event)
// ******************************************************************
{
    [_delegate softphoneHasIncomingEvent : event];
}

// ******************************************************************
void SoftphoneObserverProxy::onCallStateChanged(Softphone::EventHistory::CallEvent::Pointer call,
                                                Call::State::Type state)
// ******************************************************************
{
    [_delegate softphoneCall:call changedState:state];
}

// ******************************************************************
void SoftphoneObserverProxy::onCallHoldStateChanged(Softphone::EventHistory::CallEvent::Pointer call,
                                                    Call::HoldStates const& states)
// ******************************************************************
{
    [_delegate softphoneCall:call changedHoldStates:states];
}

// ******************************************************************
void SoftphoneObserverProxy::onMediaStatusChanged(Softphone::EventHistory::CallEvent::Pointer call,
                                                  Call::MediaStatus const& media)
// ******************************************************************
{
    [_delegate softphoneCall:call mediaStatusChanged:media];
}

// ******************************************************************
void SoftphoneObserverProxy::onAudioRouteChanged(AudioRoute::Type route)
// ******************************************************************
{
    [_delegate softphoneAudioRouteChanged:route];
}

// ******************************************************************
void SoftphoneObserverProxy::onTransferOffered(Softphone::EventHistory::CallEvent::Pointer call)
// ******************************************************************
{
	[_delegate softphoneTransferOffered:call];
}

// ******************************************************************
void SoftphoneObserverProxy::onTransferResult(Softphone::EventHistory::CallEvent::Pointer call,
                                              bool success)
// ******************************************************************
{
	[_delegate softphoneTransferResultForCall:call success:success];
}

// ******************************************************************
void SoftphoneObserverProxy::onEventsChanged(Softphone::EventHistory::ChangedEvents const& events,
                                             Softphone::EventHistory::ChangedStreams const& streams)
// ******************************************************************
{
    [_delegate softphoneHasChangedEvents:events streams:streams];
}

// ******************************************************************
ali::string SoftphoneObserverProxy::getRingtone(Softphone::EventHistory::Event::Pointer event)
// ******************************************************************
{
    if(event->eventType == Softphone::EventHistory::EventType::Message)
    {
        return "drum";
    }else
    {
        static int demoCounter = 0;
        
        // cycle between the two ringtones. The real app should probably play
        // some configured ringtone or choose the ringtone based on callee
        
        // if this function returns an empty string, the default ringtone
        // (or callee-specific ringtone, registered via setCalleeCallHandle
        // will be used
        
        if(demoCounter++ % 2)
        {
            return "drum";
        }else
        {
            return "ding-dong";
        }
    }
}

#if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)

// ******************************************************************
void SoftphoneObserverProxy::onVoicemail(ali::string const& accountId, Voicemail::Record const& voicemail)
// ******************************************************************
{
    [_delegate voicemailAvailable:voicemail forAccount:accountId];
}
#else
// ******************************************************************
void SoftphoneObserverProxy::onVoicemail(Voicemail::Record const& voicemail)
// ******************************************************************
{
    [_delegate voicemailAvailable:voicemail];
}

#endif

// ******************************************************************
void SoftphoneObserverProxy::onSimulatedMicrophoneStopped()
// ******************************************************************
{
    [_delegate onSimulatedMicrophoneStopped];
}
