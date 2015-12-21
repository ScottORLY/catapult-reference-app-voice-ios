//
//  EndpointInfo.h
//  bwsip-framework
//
//  Created by Vinícius Egidio on 6/9/15.
//

#import <Foundation/Foundation.h>

@interface BWEndpointInfo : NSObject

@property (nonatomic, strong) NSString *ipLocalAddress;
@property (nonatomic, strong) NSString *ipRemoteAddress;
@property (nonatomic, strong) NSArray  *nameServers;
@property (nonatomic, strong) NSString *wifiSSID;

@end