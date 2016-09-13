 //
 //  SoftphoneObserverProxy.cpp
 //  Bandwidth Voice Ref App
 //
 //  Copyright © 2016 Bandwidth. All rights reserved.
 //

#include "SoftphoneObserverProxy.h"
#import "CurrentCallHolder.h"
#include <ali/ali_mac_str_utils.h>

SoftphoneObserverProxy::SoftphoneObserverProxy(SIPManager * aSipManager) {
	sipManager = aSipManager;
}

SoftphoneObserverProxy::~SoftphoneObserverProxy() {
	sipManager = nil;
}

void SoftphoneObserverProxy::onRegistrationStateChanged(ali::string const& accountId, Registrator::State::Type state) {
    [sipManager onRegistrationStateChanged:SoftphoneObserverProxy::acrobbitsRegStateToBWRegState(state)
                                forAccount:ali::mac::str::to_nsstring(accountId)];
}

void SoftphoneObserverProxy::onNetworkChangeDetected(Softphone::Network::Type network) {
    // this may be used to reflect the change in GUI, but all reinvites and
    // re-registrations are handled internally by libsoftphone
}

void SoftphoneObserverProxy::onIncomingEvent(Softphone::EventHistory::Event::Pointer event) {
    if (event->isCall()) {
        [CurrentCallHolder acquire:event];
        [sipManager onIncomingCall];
    }
}

void SoftphoneObserverProxy::onCallStateChanged(Softphone::EventHistory::CallEvent::Pointer call,
                                                Call::State::Type state) {
    [CurrentCallHolder setLastState:state];
    [sipManager onCallStateChanged:SoftphoneObserverProxy::acrobbitsCallStateToBWCallState(state)];
}

void SoftphoneObserverProxy::onCallHoldStateChanged(Softphone::EventHistory::CallEvent::Pointer call,
                                                    Call::HoldStates const& states) {}

void SoftphoneObserverProxy::onMediaStatusChanged(Softphone::EventHistory::CallEvent::Pointer call,
                                                  Call::MediaStatus const& media) {}

void SoftphoneObserverProxy::onAudioRouteChanged(Softphone::AudioRoute::Type route) {}

void SoftphoneObserverProxy::onTransferOffered(Softphone::EventHistory::CallEvent::Pointer call) {}

void SoftphoneObserverProxy::onTransferResult(Softphone::EventHistory::CallEvent::Pointer call,
                                              bool success) {}

void SoftphoneObserverProxy::onEventsChanged(Softphone::EventHistory::ChangedEvents const& events,
                                             Softphone::EventHistory::ChangedStreams const& streams) {}

ali::string SoftphoneObserverProxy::getRingtone(Softphone::EventHistory::Event::Pointer event) {
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

void SoftphoneObserverProxy::onVoicemail(ali::string const& accountId, Voicemail::Record const& voicemail) {}

void SoftphoneObserverProxy::onSimulatedMicrophoneStopped() {}

RegistrationState SoftphoneObserverProxy::acrobbitsRegStateToBWRegState(Registrator::State::Type state) {
    switch (state) {
        case Registrator::State::Type::Registered:
            return Registered;
        case Registrator::State::Type::Registering:
            return Registering;
        default:
            return NotRegistered;
            
    }
}

CallState SoftphoneObserverProxy::acrobbitsCallStateToBWCallState(Call::State::Type state) {
    switch (state) {
        case Call::State::Type::Busy:
            return Busy;
        case Call::State::Type::Error:
            return Error;
        case Call::State::Type::Established:
            return Established;
        case Call::State::Type::IncomingAnsweredElsewhere:
            return IncomingAnsweredElsewhere;
        case Call::State::Type::IncomingForwarded:
            return IncomingForwarded;
        case Call::State::Type::IncomingIgnored:
            return IncomingIgnored;
        case Call::State::Type::IncomingMissed:
            return IncomingMissed;
        case Call::State::Type::IncomingRejected:
            return IncomingRejected;
        case Call::State::Type::IncomingRinging:
            return IncomingRinging;
        case Call::State::Type::IncomingTrying:
            return IncomingTrying;
        case Call::State::Type::RedirectedToAlternativeService:
            return RedirectedToAlternativeService;
        case Call::State::Type::Ringing:
            return Ringing;
        case Call::State::Type::Terminated:
            return Terminated;
        case Call::State::Type::Trying:
            return Trying;
        case Call::State::Type::Unauthorized:
            return Unauthorized;
        case Call::State::Type::Unknown:
            return Unknown;
    }
}

