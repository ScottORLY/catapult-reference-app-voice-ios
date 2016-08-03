//
//  BWCall.h
//  Bandwidth Voice Ref App
//
//  Copyright © 2016 Bandwidth. All rights reserved.
//

#ifndef BWCall_h
#define BWCall_h

#import <Foundation/Foundation.h>

@interface BWCall: NSObject

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

@property (nonatomic) NSString *remoteUri;
@property (nonatomic) CallState lastState;
@property (nonatomic) BOOL isIncoming;
@property (nonatomic) BOOL isOnHold;
@property (nonatomic) BOOL isMuted;

- (id) init;

+ (NSString*) callStateToString:(CallState) state;

@end

#endif /* BWCall_h */
