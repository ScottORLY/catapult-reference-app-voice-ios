//
//  BWGps.h
//  bwsip-framework
//
//  Created by Vinícius Egidio on 4/29/14.
//

#import <CoreLocation/CoreLocation.h>
#import <Foundation/Foundation.h>

@class BWGps;

/// Protocol
@protocol BWGpsDelegate <NSObject>

- (void)onCoordinatesUpdated:(BWGps *)gps;

@end

/// Objective-C++ Class
@interface BWGps : NSObject <CLLocationManagerDelegate>

@property (nonatomic, weak) id<BWGpsDelegate> delegate;

@property (nonatomic, strong) CLLocationManager *locationManager;
@property (nonatomic) double latitude;
@property (nonatomic) double longitude;
@property (nonatomic, strong) NSDate *time;
@property (nonatomic, readonly) BOOL isSuccess;

- (void)updateCoordinates;
- (NSString *)generatePIDFLOXml;

@end