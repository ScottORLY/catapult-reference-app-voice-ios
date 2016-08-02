//
//  BWCall.h
//  Bandwidth Voice Ref App
//
//  Created by Isac Souza on 1/8/16.
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

@property (nonatomic) NSString *localUri;
@property (nonatomic) NSString *remoteUri;
@property (nonatomic) CallState lastState;
@property (nonatomic) BOOL isIncoming;
@property (nonatomic) BOOL isOnHold;
@property (nonatomic) BOOL isMuted;

-(id) init;

@end

#endif /* BWCall_h */
