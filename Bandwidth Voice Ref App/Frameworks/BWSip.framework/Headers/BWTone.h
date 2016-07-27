//
//  BWTone.h
//  bwsip-framework
//
//  Created by Vinícius Egidio on 9/15/14.
//

#import <Foundation/Foundation.h>

@interface BWTone : NSObject

/**
 * Starts the ringback tone.
 */
- (void)startRingback;

/**
 * Stops the ringback tone.
 */
- (void)stopRingback;

/**
 * Return YES if the ringback tone is playing; NO if it's not playing
 */
- (BOOL)isRingbackPlaying;

/**
 * Starts a tone with an indefinite duration.
 *
 * @param digit the DTMF digit to be played.
 */
- (void)startDigit:(NSString *)digit;

/**
 * Starts a tone with an indefinite duration and custom volume.
 *
 * @param digit the DTMF digit to be played.
 * @param volume the tone volume [0 - mute; 1 - full volume].
 */
- (void)startDigit:(NSString *)digit withVolume:(float)volume;

/**
 * Stops any tone currently playing.
 */
- (void)stopDigit;

/**
 * Plays a DTMF tone (default duration = 200 ms).
 *
 * @param digit the DTMF digit to be played.
 */
- (void)playDigit:(NSString *)digit;

/**
* Plays a DTMF tone (default duration = 200 ms).
*
* @param digit the DTMF digit to be played.
* @param volume the tone volume [0 - mute; 1 - full volume].
*/
- (void)playDigit:(NSString *)digit withVolume:(float)volume;

/**
 * Plays a DTMF tone for X ms.
 *
 * @param digit the DTMF digit to be played.
 * @param duration the number of milliseconds to play the tone.
 */
- (void)playDigit:(NSString *)digit withDuration:(int)duration;

/**
* Plays a DTMF tone for X ms.
*
* @param digit the DTMF digit to be played.
* @param duration the number of milliseconds to play the tone.
* @param volume the tone volume [0 - mute; 1 - full volume].
*/
- (void)playDigit:(NSString *)digit withDuration:(int)duration andVolume:(float)volume;

@end