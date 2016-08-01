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

@class User;

typedef NS_ENUM(NSInteger, RegistrationState){
    NotRegistered,
    Registering,
    Registered
};

typedef NS_ENUM(NSInteger, CallState){
    Unknown,
    Trying,
    Ringing,
    Busy,
    IncomingTrying,
    IncomingRinging,
    IncomingIgnored,
    IncomingRejected,
    IncomingMissed,
    Established,
    Error,
    Unauthorized,
    Terminated,
    IncomingForwarded,
    IncomingAnsweredElsewhere,
    RedirectedToAlternativeService
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
- (void)onRegistrationStateChanged:(RegistrationState) state
                               forAccount:(NSString*)accountId;
- (void)onIncomingCall:(unsigned long) callId;
- (void)onCallStateChanged:(unsigned long) callId changedState:(CallState) state;

@end

#endif /* ASIPManager_h */
