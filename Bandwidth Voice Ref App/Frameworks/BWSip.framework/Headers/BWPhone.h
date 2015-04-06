//
//  BWPhone.h
//  bwsip-framework
//
//  Created by Vinícius Egidio on 3/27/14.
//

#import <Foundation/Foundation.h>
#import "BWCall.h"
#import "BWConsts.h"
#import "pjsua2.hpp"

@class BWPhone;
@class BWAccount;
@class BWCall;

#ifdef __cplusplus
using namespace pj;

/// C++ Class
class MyEndpoint : public Endpoint
{
private:
	BWPhone *bwPhone;

public:
	MyEndpoint(BWPhone *bwPhone);
};
#endif

/// Objective-C Class
@interface BWPhone : NSObject

// PJSUA2 objects
#ifdef __cplusplus
@property (nonatomic, readonly) MyEndpoint *myEndpoint;
#endif

@property (nonatomic, readonly) BOOL ringbackPlaying;
@property (nonatomic, strong) NSMutableArray *nameServers;
@property (nonatomic, strong) NSMutableArray *stunServers;
@property (nonatomic, strong, readonly) NSMutableArray *accounts;
@property (nonatomic) int logLevel;
@property (nonatomic) BOOL srvResolution;
@property (nonatomic, readonly) float inputVolume;
@property (nonatomic, readonly) float outputVolume;

/// Port number
@property (nonatomic) int clientPort;

/// Transport type
@property (nonatomic) BWTransport transportType;
@property (nonatomic, strong, readonly) NSString *transportTag;

/// User Agent
@property (nonatomic, strong) NSString *userAgent;

/**
 * BWPhone shared instance - This is a Singleton class
 */
+ (BWPhone *)sharedInstance;

/**
 * Initialize the BWPhone object. At this stage the framework has some default values hardcoded.
 */
- (void)initialize;

/**
 * Check if PJSUA2 native library is loaded and the endpoint is initialized.
 */
- (BOOL)isInitalized;

/**
 * Configure the transport for the current endpoint.
 */
- (void)createTransport;

/**
 * Deletes any PJSUA2 library. You must call this function when SIP connection is no longer used.
 */
- (void)close;

/**
 * Selects the device's audio output (Earpiece, Loudspeaker or Bluetooth).
 */
- (void)setAudioOutputRoute:(BWOutputRoute)outputRoute;

/**
 * Add an account to the account list.
 */
- (void)addAccount:(BWAccount *)account;

/**
 * Remove an account from the account list.
 */
- (void)removeAccount:(BWAccount *)account;

/**
 * Update the input volume (usually the device's mic).
 *
 * @param volume The volume level (0 - mute, 1 - normal, > 1 - louder than normal)
 */
- (void)setInputVolume:(float)volume;

/**
 * Update the output volume (usually the device's speaker).
 *
 * @param volume The volume level (0 - mute, 1 - normal, > 1 - louder than normal)
 */
- (void)setOutputVolume:(float)volume;

@end