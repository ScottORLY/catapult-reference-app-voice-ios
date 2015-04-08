//
//  BWCall.h
//  bwsip-framework
//
//  Created by Vinícius Egidio on 31/03/14.
//

#import <Foundation/Foundation.h>
#import "BWAccount.h"
#import "BWGps.h"
#import "BWConsts.h"
#import "pjsua2.hpp"

@class BWAccount;
@class BWCall;

#ifdef __cplusplus
using namespace pj;

/// C++ Class
class MyCall : public Call
{
private:
	BWAccount *bwAccount;
	
public:
	BWCall *bwCall;
	MyCall(Account &account);
	MyCall(Account &account, int callId);
	virtual void onCallState(OnCallStateParam &prm);
	virtual void onCallMediaState(OnCallMediaStateParam &prm);
	virtual void onDtmfDigit(OnDtmfDigitParam &prm);
	void muteCall(int enabled);
};
#endif

/// Protocol
@protocol BWCallDelegate <NSObject>

- (void)onCallStateChanged:(BWCall *)call;
- (void)onIncomingDTMF:(BWCall *)call andDigits:(NSString *)digits;

@end

/// Objective-C++ Class
@interface BWCall : NSObject

#ifdef __cplusplus
@property (nonatomic) MyCall *myCall;
#endif

@property (nonatomic, weak) id<BWCallDelegate> delegate;

@property (nonatomic, weak, readonly) BWAccount *parentAccount;
@property (nonatomic, strong, readonly) NSString *localUri;
@property (nonatomic, strong, readonly) NSString *remoteUri;
@property (nonatomic, strong, readonly) NSString *stateText;
@property (nonatomic, strong, readonly) NSMutableDictionary *sipHeaders;
@property (nonatomic) BWCallState lastState;
@property (nonatomic, readonly) BOOL isIncoming;
@property (nonatomic, readonly) BOOL isOnHold;
@property (nonatomic, readonly) BOOL isMuted;

/**
 * You must use the BWAccount that will make the call.
 *
 * @param account The BWAccount object.
 */
- (id)initWithAccount:(BWAccount *)account;

/**
 * You must use the BWAccount and the Call ID that will receive the call.
 *
 * @param account The BWAccount object.
 * @param callId The call ID.
 */
- (id)initWithAccount:(BWAccount *)account andCallId:(int)callId;

/**
 * Compare two BWCall objects.
 *
 * @param call The BWCall object that will be compared with the current BWCall object.
 * @return YES if the BWCall object has the same call ID, otherwhise NO.
 */
- (BOOL)isEqual:(BWCall *)call;

/**
 * Set the remote URI.
 * Force the non-usage of dot notation for the remoteUri field.
 *
 * @param toUri The 'To' field URI.
 */
- (void)setRemoteUri:(NSString *)remoteUri;

/**
 * Call the URI previously set in the method setToUri.
 */
- (void)makeCall;

/**
 * Answer the call.
 */
- (void)answerCall:(BWSipResponse)answerType;

/**
 * Hang-up the call.
 */
- (void)hangupCall;

/**
 * Add a header to the SIP INVITE.
 *
 * @param name The SIP header field name.
 * @param value The SIP header value.
 */
- (void)addSipHeader:(NSString *)name andValue:(NSString *)value;

/**
 * Add Geolocation information in the SIP Header.
 *
 * @param gps The BWGps object.
 */
- (void)addGeolocation:(BWGps *)gps;

/**
 * Mark if the it's an incoming or outgoing call
 *
 * @param enabled YES to mark the call as incoming or no to mark as outgoing.
 */
- (void)setIncoming:(BOOL)enabled;

/**
 * Put a call on hold or release the hold.
 *
 * @param enabled YES to put the call on hold or NO to realease the hold.
 */
- (void)setOnHold:(BOOL)enabled;

/**
 * Mute or un-mute a call.
 *
 * @param enabled yes to mute the call; NO to un-mute.
 */
- (void)setMute:(BOOL)enabled;

/**
 * Send DTMF digits using RFC 2833 payload formats.
 *
 * @param digits String digits.
 */
- (void)dialDTMF:(NSString *)digits;

/**
 * Get the call stats.
 */
- (NSDictionary *)stats;

@end