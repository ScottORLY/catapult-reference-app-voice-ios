//
//  ASIPManager.mm
//  Bandwidth Voice Ref App
//
//  Copyright © 2016 Bandwidth. All rights reserved.
//

#import "ASIPManager.h"
#import "BWSoftphoneDelegate.h"
#import "CurrentCallHolder.h"

#include "ali_mac_str_utils.h"
#import "ali_xml_parser2_interface.h"
#include "Softphone.h"
#include "SoftphoneObserverProxy.h"

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import "Bandwidth_Voice_Ref_App-Swift.h"

// MARK: - Class methods

@implementation ASIPManager {
    NSString *sipAccountXmlFormat;
    ali::string license;
    ali::xml::tree _sipAccount;
    Softphone::Instance *_softphone;
    /// a C++ class which converts the callbacks into ObjectiveC selector invocations
    SoftphoneObserverProxy *_softphoneObserverProxy;
}

+ (instancetype)sharedManager
{
    static dispatch_once_t pred = 0;
    __strong static id _sharedManager = nil;
    dispatch_once(&pred, ^{
        _sharedManager = [[self alloc] init];
    });
    return _sharedManager;
}

- (id)init {
    self = [super init];
    
    if (self) {
        sipAccountXmlFormat = @"<account id=\"sip\">"
            "<username>%@</username>"
            "<password>%@</password>"
            "<host>%@</host>"
            "</account>";

        license = "<root><saas><identifier>"
                "libsoftphone.saas.bandwith.android"
                "</identifier></saas></root>";
    }
    
    return self;
}

/**
 Register with the SIP registrar using the credentials stored in the user
 
 - parameter user: the user object containing the credentials
 */
- (void)registerWithUser:(User*)user {
    
    // If we don't have a BWAccount object, create a new registration
            
    //registrationState = RegistrationState.Registering
    
    ali::xml::tree licenseXml;
    
    int errorIndex = 0;
    NSString *sipAccountXml = [NSString stringWithFormat:sipAccountXmlFormat, user.endpoint.credentials.username, user.password, user.endpoint.credentials.realm];
    bool success = ali::xml::parse(_sipAccount, ali::mac::str::from_nsstring(sipAccountXml), &errorIndex);
    ali_assert(success);
    
    success = ali::xml::parse(licenseXml, license, &errorIndex);
    ali_assert(success);
    
    // initialize the SDK with the SaaS license
    success = Softphone::init(licenseXml);
    
    // obtain the SDK instance
    _softphone = Softphone::instance();
    
    Softphone::Preferences & preferences = _softphone->settings()->getPreferences();
    preferences.trafficLogging.set(true);
    
    _softphoneObserverProxy = new SoftphoneObserverProxy([[BWSoftphoneDelegate alloc] initWithSipManager:self]);
    _softphone->setObserver(_softphoneObserverProxy);
    
    if (_softphone->registration()->getAccount("sip") == NULL) {
    
        _softphone->registration()->saveAccount(_sipAccount);
    
        _softphone->registration()->updateAll();
    }
    
    _softphone->state()->update(_softphone->state()->Active);
}

- (void) answerIncomingCall
{
    Call::State::Type cs = _softphone->calls()->getState([CurrentCallHolder get]);

    if(cs != Call::State::IncomingRinging && cs != Call::State::IncomingIgnored)
        return;
    
    _softphone->calls()->answerIncoming([CurrentCallHolder get], Call::DesiredMedia::voiceOnly());
}

- (void) rejectIncomingCall
{
    Call::State::Type cs = _softphone->calls()->getState([CurrentCallHolder get]);
    
    if(cs != Call::State::IncomingRinging && cs != Call::State::IncomingIgnored)
        return;
    
    _softphone->calls()->rejectIncoming([CurrentCallHolder get]);
    _softphone->calls()->close([CurrentCallHolder get]);
}

- (void) hangupCall
{
    _softphone->calls()->close([CurrentCallHolder get]);
}

- (BOOL) makeCallTo:(NSString *) number
{
    Softphone::EventHistory::CallEvent::Pointer newCall = Softphone::EventHistory::CallEvent::create("sip",ali::generic_peer_address(ali::mac::str::from_nsstring(number)));
    
    
    Softphone::EventHistory::EventStream::Pointer stream = Softphone::EventHistory::EventStream::load(Softphone::EventHistory::StreamQuery::legacyCallHistoryStreamKey);
    
    newCall->setStream(stream);
    
    newCall->transients["dialAction"] = "voiceCall";
    
    Softphone::Instance::Events::PostResult::Type const result = _softphone->events()->post(newCall);
    
    if(result != Softphone::Instance::Events::PostResult::Success)
    {
        // report failure
    } else {
        [CurrentCallHolder acquire:newCall];
    }
    
    return result == Softphone::Instance::Events::PostResult::Success;
}

- (void) startDigit:(NSString*)digit {
    _softphone->audio()->dtmfOn([digit characterAtIndex:0]);
}

- (void) stopDigit {
    _softphone->audio()->dtmfOff();
}

- (BWCall*) getCurrentCall {
    return [ASIPManager callEventToBWCall:[CurrentCallHolder get]->asCall() withLastState:[CurrentCallHolder getLastState]];
}

- (void)onRegistrationStateChanged:(RegistrationState) state
                        forAccount:(NSString*)accountId;
{
    NSLog(@"INFO: onRegistrationStateChanged: %@", [ASIPManager regStateToString:state]);
    
    _registrationState = state;
}

- (void)onIncomingCall
{
    NSLog(@"INFO: onIncomingCall from:%s", [CurrentCallHolder get]->getRemoteUser().getGenericUri().c_str());
    
    dispatch_async(dispatch_get_main_queue(),  ^{
        
        // Make sure we are sending the notification in the main thread
        
        [[NSNotificationCenter defaultCenter] postNotificationName:@"SIPManager.CallReceivedNotification"
                                                            object:nil];
    });
}

- (void)onCallStateChanged:(CallState) state
{
    NSLog(@"INFO: onCallStateChanged %@", [ASIPManager callStateToString:state]);
}

+ (NSString*)regStateToString:(RegistrationState) state
{
    NSString *regStateStr;
    
    switch (state) {
        
        case NotRegistered:
            regStateStr = @"Not registered";
            break;
        case Registering:
            regStateStr = @"Registering";
            break;
        case Registered:
            regStateStr = @"Registered";
            break;
    }
    
    return regStateStr;
}

+ (NSString*)callStateToString:(CallState) state
{
    NSString *callStateStr;
    
    switch (state) {
            
        case Terminated:
            callStateStr = @"Terminated";
            break;
        case Busy:
            callStateStr = @"Busy";
            break;
        case Established:
            callStateStr = @"Established";
            break;
        case IncomingRinging:
            callStateStr = @"IncomingRinging";
            break;
        case IncomingMissed:
            callStateStr = @"IncomingMissed";
            break;
        case Ringing:
            callStateStr = @"Ringing";
            break;
        case Trying:
            callStateStr = @"Trying";
            break;
        case Error:
            callStateStr = @"Error";
            break;
        default:
            callStateStr = @"Unknown";
            break;
    }
    
    return callStateStr;
}

+(BWCall*) callEventToBWCall:(Softphone::EventHistory::CallEvent&) callEvent
               withLastState:(Call::State::Type) lastState {
    BWCall *bwCall = [[BWCall alloc] init];
    bwCall.isIncoming = (callEvent.getDirection() == Softphone::EventHistory::Direction::Type::Incoming);
    bwCall.localUri = ali::mac::str::to_nsstring(callEvent.getSender().get());
    bwCall.remoteUri = ali::mac::str::to_nsstring(callEvent.getRemoteUser().getGenericUri());
    bwCall.lastState = [BWSoftphoneDelegate acrobbitsCallStateToBWCallState:lastState];
    
    return bwCall;
}

@end