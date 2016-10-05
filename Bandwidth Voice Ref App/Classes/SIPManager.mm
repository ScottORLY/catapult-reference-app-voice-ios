//
//  SIPManager.mm
//  Bandwidth Voice Ref App
//
//  Copyright © 2016 Bandwidth. All rights reserved.
//

#import "SIPManager.h"
#import "CurrentCallHolder.h"
#import <ali/ali_xml_parser2_interface.h>

#include <ali/ali_mac_str_utils.h>
#include <Softphone/Softphone.h>
#include <Softphone/Softphone_iOS.h>
#include "SoftphoneObserverProxy.h"

#import <CallKit/CallKit.h>
#import <CoreTelephony/CTCall.h>
#import <CoreTelephony/CTCallCenter.h>
#import <UIKit/UIKit.h>

#import "Bandwidth_Voice_Ref_App-Swift.h"

#define SIP_ACCOUNT_ID "TEST_ACCOUNT"
#define ACROBITS_LICENSE "tc769fn5cgsjun78qj7qfdm1h3"

// MARK: - Class methods

@interface SIPManager ()

@property(nonatomic) RegistrationState registrationState;

@end

@implementation SIPManager {
    Softphone::Instance *softphone;
    id<CallDelegate> callDelegate;
}

+ (instancetype)sharedManager {
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
        NSString *licenseFormat = @"<root><saas>"
                "<identifier>%s</identifier>"
                "</saas></root>";
        
        ali::xml::tree licenseXml;
        int errorIndex = 0;
        NSString *licenseString = [NSString stringWithFormat:licenseFormat, ACROBITS_LICENSE];
        bool success = ali::xml::parse(licenseXml, ali::mac::str::from_nsstring(licenseString), &errorIndex);
        ali_assert(success);
        
        // initialize the SDK with the SaaS license
        success = Softphone::init(licenseXml);
        
        // obtain the SDK instance
        softphone = Softphone::instance();
        
        Softphone::Preferences &preferences = softphone->settings()->getPreferences();
        preferences.trafficLogging.set(true);

        softphone->ringtones()->registerRingtone("ringtone", Softphone::RingtoneRecord("ringtone", "ringtone.wav", 0));

        auto *instance = [Softphone_iOS sharedInstance];
        if ([self isCallKitAvailable]) {
            CXProviderConfiguration *cxConfig = [instance callKitConfiguration];
            cxConfig.supportsVideo = NO;
            cxConfig.maximumCallGroups = 1;
            cxConfig.maximumCallsPerCallGroup = 1;
            cxConfig.iconTemplateImageData = UIImagePNGRepresentation([UIImage imageNamed:@"AppIcon60x60"]);
            [instance updateCallKitConfiguration: cxConfig];
        } else {
            bool const hasCT = NSClassFromString(@"CTCallCenter") != nil;
            if (hasCT) {
                CTCallCenter *cc = [[CTCallCenter alloc] init];
                cc.callEventHandler = ^(CTCall* call) {
                    NSLog(@"%@", call.callState);
                    if (cc.currentCalls.count != 0) {
                        [self performSelectorOnMainThread:@selector(rejectIncomingCall) withObject:nil waitUntilDone:NO];
                    }
                };
            }
        }
    }
    
    return self;
}

/**
 Register with the SIP registrar using the credentials stored in the user
 
 - parameter user: the user object containing the credentials
 */
- (void)registerWithUser:(User*)user {
    
    softphone->setObserver(new SoftphoneObserverProxy(self));
    
    if (softphone->registration()->getAccount(SIP_ACCOUNT_ID) == NULL) {
        NSString *sipAccountXmlFormat = @"<account id=\"%s\">"
            "<username>%@</username>"
            "<password>%@</password>"
            "<host>%@</host>"
            "</account>";
        
        NSString *sipAccountString = [NSString stringWithFormat:sipAccountXmlFormat, SIP_ACCOUNT_ID, user.endpoint.credentials.username, user.password, user.endpoint.credentials.realm];
        
        int errorIndex = 0;
        ali::xml::tree sipAccountXml;
        bool success = ali::xml::parse(sipAccountXml, ali::mac::str::from_nsstring(sipAccountString), &errorIndex);
        ali_assert(success);
    
        softphone->registration()->saveAccount(sipAccountXml);
    
        softphone->registration()->updateAll();
    }
    
    softphone->state()->update(softphone->state()->Active);
}

- (void) unregister {
    softphone->registration()->deleteAccount(SIP_ACCOUNT_ID);
    softphone->registration()->updateAll();

}

- (void) answerIncomingCall {
    Call::State::Type cs = softphone->calls()->getState([CurrentCallHolder get]);

    if(cs != Call::State::IncomingRinging && cs != Call::State::IncomingIgnored)
        return;
    
    softphone->calls()->answerIncoming([CurrentCallHolder get], Call::DesiredMedia::voiceOnly());
}

- (void) rejectIncomingCall {
    Call::State::Type cs = softphone->calls()->getState([CurrentCallHolder get]);
    
    if(cs != Call::State::IncomingRinging && cs != Call::State::IncomingIgnored)
        return;
    
    softphone->calls()->rejectIncoming([CurrentCallHolder get]);
    softphone->calls()->close([CurrentCallHolder get]);
}

- (void) hangupCall {
    softphone->calls()->hangup([CurrentCallHolder get]);
}

- (BOOL) makeCallTo:(NSString *) number {
    Softphone::EventHistory::CallEvent::Pointer newCall = Softphone::EventHistory::CallEvent::create(SIP_ACCOUNT_ID, ali::generic_peer_address(ali::mac::str::from_nsstring(number)));
    
    
    Softphone::EventHistory::EventStream::Pointer stream = Softphone::EventHistory::EventStream::load(Softphone::EventHistory::StreamQuery::legacyCallHistoryStreamKey);
    
    newCall->setStream(stream);
    
    newCall->transients["dialAction"] = "voiceCall";
    
    Softphone::Instance::Events::PostResult::Type const result = softphone->events()->post(newCall);
    
    if(result != Softphone::Instance::Events::PostResult::Success)
    {
        // report failure
    } else {
        [CurrentCallHolder acquire:newCall];
    }
    
    return result == Softphone::Instance::Events::PostResult::Success;
}

- (void) startDigit:(NSString*)digit {
    softphone->audio()->dtmfOn([digit characterAtIndex:0]);
}

- (void) stopDigit {
    softphone->audio()->dtmfOff();
}

- (void) setSpeakerEnabled:(BOOL)enabled {
    softphone->audio()->setCallAudioRoute(enabled ? Softphone::AudioRoute::Type::Speaker : Softphone::AudioRoute::Type::Headset);
}

- (void) setMute:(BOOL)enabled {
    softphone->audio()->setMuted(enabled);
}

- (BWCall*) getCurrentCall {
    return [SIPManager callEventToBWCall:[CurrentCallHolder get]->asCall() withLastState:[CurrentCallHolder getLastState]];
}

- (void)onRegistrationStateChanged:(RegistrationState) state
                        forAccount:(NSString*)accountId {
    NSLog(@"INFO: onRegistrationStateChanged: %@", [SIPManager regStateToString:state]);
    
    self.registrationState = state;
}

- (void)onIncomingCall {
    NSLog(@"INFO: onIncomingCall from:%s", [CurrentCallHolder get]->getRemoteUser().getGenericUri().c_str());
    
    dispatch_async(dispatch_get_main_queue(),  ^{
        
        // Make sure we are sending the notification in the main thread
        
        [[NSNotificationCenter defaultCenter] postNotificationName:[SIPManager getCallReceivedNotificationName]
                                                            object:nil];
    });
}

- (void)onCallStateChanged:(CallState) state {
    NSLog(@"INFO: onCallStateChanged %@", [BWCall callStateToString:state]);
    
    if (callDelegate != NULL) {
        [callDelegate onCallStateChanged:[self getCurrentCall]];
    }
    
    if (Call::State::isTerminal([CurrentCallHolder getLastState])) {
        softphone->calls()->close([CurrentCallHolder get]);
    }
}

+ (NSString*)regStateToString:(RegistrationState) state {
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

- (void)setCallDelegate:(id<CallDelegate>)delegate {
    callDelegate = delegate;
}

- (BOOL) isCallKitAvailable
{
    return [[Softphone_iOS sharedInstance] isCallKitAvailable];
}

- (BOOL) processIntent:(INIntent *)intent {
    return [[Softphone_iOS sharedInstance] processIntent:intent];
}

+(BWCall*) callEventToBWCall:(Softphone::EventHistory::CallEvent&) callEvent
               withLastState:(Call::State::Type) lastState {
    BWCall *bwCall = [[BWCall alloc] init];
    bwCall.isIncoming = (callEvent.getDirection() == Softphone::EventHistory::Direction::Type::Incoming);
    bwCall.remoteUri = ali::mac::str::to_nsstring(callEvent.getRemoteUser().getTransportUri().get());
    bwCall.lastState = SoftphoneObserverProxy::acrobbitsCallStateToBWCallState(lastState);
    
    return bwCall;
}

+ (NSString*)getCallReceivedNotificationName {
    return @"SIPManager.CallReceivedNotification";
}

@end
