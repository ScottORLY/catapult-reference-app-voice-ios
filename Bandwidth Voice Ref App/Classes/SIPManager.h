//
//  SIPManager.h
//  Bandwidth Voice Ref App
//
//  Copyright © 2016 Bandwidth. All rights reserved.
//

#ifndef SIPManager_h
#define SIPManager_h

#import <Foundation/Foundation.h>
#import <Intents/Intents.h>

#import "Reachability.h"
#import "BWCall.h"
#import "CallDelegate.h"

@class User;

typedef NS_ENUM(NSInteger, RegistrationState){
    NotRegistered,
    Registering,
    Registered
};

@interface SIPManager : NSObject

@property(nonatomic,readonly) RegistrationState registrationState;

/**
 *  Returns the default instance for this manager.
 *
 *  @return default manager instance
 */
+ (instancetype) sharedManager;
+ (NSString*) regStateToString:(RegistrationState) state;
+ (NSString*) getCallReceivedNotificationName;

- (id) init;
- (void) registerWithUser:(User*)user;
- (void) unregister;
- (void) answerIncomingCall;
- (void) rejectIncomingCall;
- (void) hangupCall;
- (BOOL) makeCallTo:(NSString*) number;
- (void) startDigit:(NSString*)digit;
- (void) stopDigit;
- (void) setSpeakerEnabled:(BOOL)enabled;
- (void) setMute:(BOOL)enabled;
- (BWCall*) getCurrentCall;

- (void) onRegistrationStateChanged:(RegistrationState) state
                               forAccount:(NSString*)accountId;
- (void) onIncomingCall;
- (void) onCallStateChanged:(CallState) state;

- (void) setCallDelegate:(id<CallDelegate>)delegate;

- (BOOL) processIntent:(INIntent *)intent;

@end

#endif /* SIPManager_h */
