//
//  BWSoftphoneDelegate.m
//  Bandwidth Voice Ref App
//
//  Created by Isac Souza on 29/7/16.
//  Copyright © 2016 Bandwidth. All rights reserved.
//

#import "BWSoftphoneDelegate.h"
#import "CurrentCallHolder.h"
#import <Foundation/Foundation.h>
#include "ali_mac_str_utils.h"

@implementation BWSoftphoneDelegate

-(id) initWithSipManager:(SIPManager*) sipManager
{
    if(self = [super init]) {
        _sipManager = sipManager;
    }
    
    return self;
}

-(void) softphoneRegistrationStateChanged:(Registrator::State::Type) state
                               forAccount:(const ali::string &)accountId
{
    [_sipManager onRegistrationStateChanged:[BWSoftphoneDelegate acrobbitsRegStateToBWRegState:state]
                                 forAccount:ali::mac::str::to_nsstring(accountId)];
}

-(void) softphoneHasIncomingEvent:(Softphone::EventHistory::Event::Pointer) event {
    if (event->isCall()) {
        [CurrentCallHolder acquire:event];
        [_sipManager onIncomingCall];
    }
}

-(void) softphoneCall:(Softphone::EventHistory::CallEvent::Pointer) call changedState:(Call::State::Type) state
{
    [CurrentCallHolder setLastState:state];
    [_sipManager onCallStateChanged:[BWSoftphoneDelegate acrobbitsCallStateToBWCallState:state]];
}

+(RegistrationState) acrobbitsRegStateToBWRegState:(Registrator::State::Type) state
{
    switch (state) {
        case Registrator::State::Type::Registered:
            return Registered;
        case Registrator::State::Type::Registering:
            return Registering;
        default:
            return NotRegistered;
            
    }
}

+(CallState) acrobbitsCallStateToBWCallState:(Call::State::Type) state
{
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

// Unhandled callbacks
/// Called when the hold state of a call changes
-(void) softphoneCall:(Softphone::EventHistory::CallEvent::Pointer) call changedHoldStates:(Call::HoldStates const& ) holdStates {}

/// Called when the media status for the call has changed, for example when the video stream became available.
-(void) softphoneCall:(Softphone::EventHistory::CallEvent::Pointer) call
   mediaStatusChanged:(Call::MediaStatus const&) media {}

/// @brief Called when the hardware audio route changes, for example when the headset is plugged in or when bluetooth is
/// connected. The receiver should update GUI to reflect the change, for example by updating the speaker button on/off
/// state.
-(void) softphoneAudioRouteChanged:(AudioRoute::Type)route {}

/// @brief Called to notify GUI that the call with callId received a REFER wifh an offered transfer. The transfer can be then
/// accepted or rejected by calling @ref Softphone::InstanceEx::CallsEx::acceptOfferedTransfer or @ref
/// Softphone::InstanceEx::CallsEx::rejectOfferedTransfer.
-(void) softphoneTransferOffered:(Softphone::EventHistory::CallEvent::Pointer) call {}

/// Notifies the received about the result of a previous transfer.
-(void) softphoneTransferResultForCall:(Softphone::EventHistory::CallEvent::Pointer) call success:(bool)success {}

/// @brief Called every time after some active events / streams are changed.
-(void) softphoneHasChangedEvents:(Softphone::EventHistory::ChangedEvents) events
                          streams:(Softphone::EventHistory::ChangedStreams) streams {}

-(void) voicemailAvailable:(Voicemail::Record const&)voicemail forAccount:(ali::string const&) accountId {}

-(void) onSimulatedMicrophoneStopped {}

@end