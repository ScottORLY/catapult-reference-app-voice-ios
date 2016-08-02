//
//  ASIPManager.h
//  Bandwidth Voice Ref App
//
//  Created by Isac Souza on 28/7/16.
//  Copyright © 2016 Bandwidth. All rights reserved.
//

#ifndef ASIPManager_h
#define ASIPManager_h

#import <Foundation/Foundation.h>
#import "Reachability.h"
#import "BWCall.h"
#import "CallDelegate.h"

@class User;

typedef NS_ENUM(NSInteger, RegistrationState){
    NotRegistered,
    Registering,
    Registered
};

@interface ASIPManager : NSObject

@property(nonatomic,readonly) RegistrationState registrationState;

/**
 *  Returns the default instance for this manager.
 *
 *  @return default manager instance
 */
+ (instancetype)sharedManager;
+ (NSString*)regStateToString:(RegistrationState) state;

- (id)init;
- (void)registerWithUser:(User*)user;
- (void) answerIncomingCall;
- (void) rejectIncomingCall;
- (void) hangupCall;
- (BOOL) makeCallTo:(NSString*) number;
- (void) startDigit:(NSString*)digit;
- (void) stopDigit;
- (void) setSpeakerEnabled:(BOOL)enabled;
- (BWCall*) getCurrentCall;

- (void)onRegistrationStateChanged:(RegistrationState) state
                               forAccount:(NSString*)accountId;
- (void)onIncomingCall;
- (void)onCallStateChanged:(CallState) state;

- (void)setCallDelegate:(id<CallDelegate>)delegate;

@end

#endif /* ASIPManager_h */
