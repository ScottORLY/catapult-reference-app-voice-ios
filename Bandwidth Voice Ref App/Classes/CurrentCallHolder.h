//
//  CurrentCallHolder.h
//  Bandwidth Voice Ref App
//
//  Copyright © 2016 Bandwidth. All rights reserved.
//

#ifndef CurrentCallHolder_h
#define CurrentCallHolder_h

#import <Foundation/Foundation.h>
#include <Softphone/Softphone.h>

@interface CurrentCallHolder : NSObject

+ (Softphone::EventHistory::CallEvent::Pointer) get;
+ (void) release:(Softphone::EventHistory::CallEvent::Pointer) callEvent;
+ (void) acquire:(Softphone::EventHistory::CallEvent::Pointer) callEvent;
+ (Call::State::Type) getLastState;
+ (void) setLastState:(Call::State::Type)state;

@end

#endif /* CurrentCallHolder_h */
