//
//  CurrentCallHolder.m
//  Bandwidth Voice Ref App
//
//  Created by Isac Souza on 1/8/16.
//  Copyright © 2016 Bandwidth. All rights reserved.
//

#import "CurrentCallHolder.h"
#import "BWCall.h"
#include "ali_mac_str_utils.h"

@implementation CurrentCallHolder

Call::State::Type lastState = Call::State::Type::Unknown;
Softphone::EventHistory::CallEvent::Pointer currentCall;

+ (void)acquire:(Softphone::EventHistory::CallEvent::Pointer) callEvent {
    currentCall = callEvent;
}

+ (void)release:(Softphone::EventHistory::CallEvent::Pointer) callEvent {
    currentCall = nil;
    lastState = Call::State::Type::Unknown;
}

+ (Softphone::EventHistory::CallEvent::Pointer)get {
    return currentCall;
}

+ (Call::State::Type)getLastState {
    return lastState;
}

+ (void)setLastState:(Call::State::Type)state {
    lastState = state;
}

@end