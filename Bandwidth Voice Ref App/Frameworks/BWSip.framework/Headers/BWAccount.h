//
//  BWAccount.h
//  bwsip-framework
//
//  Created by Vinícius Egidio on 30/03/14.
//

#import <Foundation/Foundation.h>
#import "BWPhone.h"
#import "BWCall.h"
#import "BWConsts.h"
#import "BWCredentials.h"
#import "pjsua2.hpp"

@class BWPhone;
@class BWAccount;
@class BWCall;

#ifdef __cplusplus
using namespace pj;

/// C++ Class
class MyAccount : public Account
{
private:
	BWAccount *bwAccount;
	NSDictionary<NSString*, NSArray<NSString*>*> *parseSipHeaders(NSString *wholeMsg);
	
public:
	MyAccount(BWAccount *bwAccount);
	virtual void onRegState(OnRegStateParam &prm);
	virtual void onIncomingCall(OnIncomingCallParam &prm);
};
#endif

/// Protocol
@protocol BWAccountDelegate <NSObject>

- (void)onRegStateChanged:(BWAccount *)account;
- (void)onIncomingCall:(BWCall *)call;

@end

/// Objective-C Class
@interface BWAccount : NSObject

#ifdef __cplusplus
@property (nonatomic, readonly) MyAccount *myAccount;
#endif

@property (nonatomic, weak) id<BWAccountDelegate> delegate;

@property (nonatomic, readonly) BOOL iceEnabled;
@property (nonatomic, readonly) int registrationInterval;
@property (nonatomic, readonly) int registrationFirstRetryInterval;
@property (nonatomic, readonly) int registrationRetryInterval;
@property (nonatomic) int registrationRegistrarInterval;
@property (nonatomic) BWSipResponse lastState;
@property (nonatomic, strong, readonly) BWCredentials *credentials;
@property (nonatomic, strong, readonly) NSString *idUri;
@property (nonatomic, strong, readonly) NSString *displayName;
@property (nonatomic, strong, readonly) NSString *registrar;
@property (nonatomic, strong, readonly) NSMutableArray *calls;
@property (nonatomic) int serverPort;

/**
 * When a BWAccount object is initialized it checks if the BWPhone used as a parameter in the constructor is already
 * initialized (i.e. the PJSUA2 library is already loaded and Enpoint is not null).
 *
 * @param phone The BWPhone object.
 */
- (id)initWithPhone:(BWPhone *)phone;

/**
 * Compare two BWAccount objects.
 *
 * @param account The BWAccount object that will be compared with the current BWAccount object.
 * @return YES if the BWAccount object has the same account ID, otherwhise NO.
 */
- (BOOL)isEqual:(BWAccount *)account;

/**
 * Establish a connection with the SIP server. This method must be called after a BWAccount object is created and the
 * developer has already set all the connection parameters like registrar, user credentials, etc. Any change in the
 * BWAccount object only has affect when a new connection is done.
 */
- (void)connect;

/**
 * Closes all connections for that account.
 */
- (void)close;

/**
 * Add a header to the SIP INVITE.
 *
 * @param name The SIP header field name.
 * @param value The SIP header value.
 */
- (void)addSipHeader:(NSString *)name andValue:(NSString *)value;

/**
 * Get the SIP headers
 */
- (NSDictionary *)getSipHeaders;

/**
 * Clear the SIP headers.
 */
- (void)clearSipHeaders;

/**
 * Add a proxy.
 *
 * @param proxy String proxy URL.
 */
- (void)addProxy:(NSString *)proxy;

/**
 * Get the proxies.
 */
- (NSArray *)getProxies;

/**
 * Clear the proxies.
 */
- (void)clearProxies;

/**
 * Force the non-usage of dot notation for the iceEnabled field.
 *
 * @param iceEnabled Boolean: YES or NO
 */
- (void)setIceEnabled:(BOOL)iceEnabled;

/**
 * Flag to tell if the account is currently registered.
 *
 * @return YES if the account is currently registered; NO if it's not.
 */
- (BOOL)isRegistrationActive;

/**
 * Update registration or perform unregistration.
 *
 * @param renew true to renew the registration, false to cancel the registration.
 */
- (void)updateRegistration:(BOOL)renew;

/**
 * Force the non-usage of dot notation for the registrationInterval field.
 *
 * @param interval the interval, in seconds, when the registration will be sent again.
 */
- (void)setRegistrationInterval:(int)interval;

/**
 * The interval, in seconds, when we will try to do the registration again after it fails for the first time.
 */
- (void)setRegistrationFirstRetryInterval:(int)interval;

/**
 * The interval, in seconds, when we will try to do the registration again after it fails for the second time.
 */
- (void)setRegistrationRetryInterval:(int)interval;

/**
 * Force the non-usage of dot notation for the idUri field.
 *
 * @param idUri The ID URI
 */
- (void)setIdUri:(NSString *)idUri;

/**
 * Force the non-usage of dot notation for the displayName field.
 *
 * @param displayName The account display name.
 */
- (void)setDisplayName:(NSString *)displayName;

/**
 * Force the non-usage of dot notation for the registrar field.
 *
 * @param registrar The registrar URI.
 */
- (void)setRegistrar:(NSString *)registrar;

/**
 * Set the user credentials.
 */
- (void)setCredentials:(BWCredentials *)credentials;

/**
 * Add a call to the call list.
 */
- (void)addCall:(BWCall *)call;

/**
 * Remove a call from the call list.
 */
- (void)removeCall:(BWCall *)call;

@end