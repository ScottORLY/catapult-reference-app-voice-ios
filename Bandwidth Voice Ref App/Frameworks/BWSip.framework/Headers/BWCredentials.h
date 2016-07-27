//
//  BWCredentials.h
//  bwsip-framework
//
//  Created by Vinícius Egidio on 7/2/15.
//

#import <Foundation/Foundation.h>

@interface BWCredentials : NSObject

@property (nonatomic, strong, readonly) NSString *username;
@property (nonatomic, strong, readonly) NSString *password;
@property (nonatomic, strong, readonly) NSString *token;

/**
 * Create a BWCredentials with the username & password.
 *
 * @param username The username.
 * @param password The password.
 */
- (id)initWithUsername:(NSString *)username andPassword:(NSString *)password;

/**
 * Create a BWCredentials with the username & password.
 *
 * @param username The username.
 * @param token The token.
 */
- (id)initWithUsername:(NSString *)username andToken:(NSString *)token;

/**
 * Check if this is a token based or password based credentials.
 */
- (BOOL)isTokenBased;

@end