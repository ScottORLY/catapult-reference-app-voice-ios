//
//  BWCall.m
//  Bandwidth Voice Ref App
//
//  Created by Isac Souza on 1/8/16.
//  Copyright © 2016 Bandwidth. All rights reserved.
//

#import "BWCall.h"

@implementation BWCall

-(id) init {
    self = [super init];
    
    return self;
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

@end