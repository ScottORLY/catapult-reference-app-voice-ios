//
//  BWCodec.h
//  bwsip-framework
//
//  Created by Vinícius Egidio on 11/13/14.
//

#import <Foundation/Foundation.h>

@interface BWCodec : NSObject

@property (strong, nonatomic) NSString *codecId;
@property (nonatomic) int priority;

/**
 * List the available codecs.
 */
+ (NSArray *)listAvailable;

/**
 * Change the codecs' priority.
 */
+ (void)setPriority:(NSArray *)codecs;

@end
