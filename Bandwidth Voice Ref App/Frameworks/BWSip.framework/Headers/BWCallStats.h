//
//  CallStats.h
//  bwsip-framework
//
//  Created by Vinícius Egidio on 6/9/15.
//

#import <Foundation/Foundation.h>

@interface BWCallStats : NSObject

@property (nonatomic, strong) NSString  *codecName;
@property (nonatomic) NSInteger codecClock;

@property (nonatomic) NSInteger rxBytes;
@property (nonatomic) NSInteger rxPackageNumber;
@property (nonatomic) NSInteger rxPackageLost;
@property (nonatomic) NSInteger rxPackageDiscard;
@property (nonatomic) NSInteger rxPackageDuplicated;
@property (nonatomic) NSInteger rxPackageReordered;
@property (nonatomic) NSInteger rxJitterMean;
@property (nonatomic) NSInteger rxLossPeriodMeant;

@property (nonatomic) NSInteger txBytes;
@property (nonatomic) NSInteger txPackageNumber;
@property (nonatomic) NSInteger txPackageLost;
@property (nonatomic) NSInteger txPackageDiscard;
@property (nonatomic) NSInteger txPackageDuplicated;
@property (nonatomic) NSInteger txPackageReordered;
@property (nonatomic) NSInteger txJitterMean;
@property (nonatomic) NSInteger txLossPeriodMeant;

@property (nonatomic) NSInteger roundTripTimeMean;

@end