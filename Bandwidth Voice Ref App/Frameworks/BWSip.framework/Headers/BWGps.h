//
//  BWGps.h
//  bwsip-framework
//
//  Created by Vinícius Egidio on 4/29/14.
//

#import <CoreLocation/CoreLocation.h>
#import <Foundation/Foundation.h>

@interface BWGps : NSObject <CLLocationManagerDelegate>

@property (nonatomic, strong) CLLocationManager *locationManager;
@property (nonatomic) double latitude;
@property (nonatomic) double longitude;
@property (nonatomic, strong) NSDate *time;

@property (nonatomic, readonly) BOOL isFinished;
@property (nonatomic, readonly) BOOL isSuccess;

- (void)updateCoordinates;

@end