//
//  CallDelegate.h
//  Bandwidth Voice Ref App
//
//  Created by Isac Souza on 2/8/16.
//  Copyright © 2016 Bandwidth. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BWCall.h"

@protocol CallDelegate <NSObject>

- (void) onCallStateChanged:(BWCall*)call;

@end
