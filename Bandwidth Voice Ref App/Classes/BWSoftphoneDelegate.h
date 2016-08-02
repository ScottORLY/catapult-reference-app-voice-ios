//
//  BWSoftphoneDelegate.h
//  Bandwidth Voice Ref App
//
//  Created by Isac Souza on 29/7/16.
//  Copyright © 2016 Bandwidth. All rights reserved.
//

#ifndef BWSoftphoneDelegate_h
#define BWSoftphoneDelegate_h

#import "SoftphoneDelegate.h"
#import "ASIPManager.h"

@interface BWSoftphoneDelegate: NSObject <SoftphoneDelegate>;

@property(nonatomic,readonly) ASIPManager* sipManager;

-(id) initWithSipManager:(ASIPManager*) sipManager;

+(CallState) acrobbitsCallStateToBWCallState:(Call::State::Type) state;

@end


#endif /* BWSoftphoneDelegate_h */
