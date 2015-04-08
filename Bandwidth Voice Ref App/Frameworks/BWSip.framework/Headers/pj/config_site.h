/*
 * This file contains several sample settings especially for Windows
 * Mobile and Symbian targets. You can include this file in your
 * "pj/config_site.h" file.
 *
 * The Windows Mobile and Symbian settings will be activated
 * automatically if you include this file.
 *
 * In addition, you may specify one of these macros (before including
 * this file) to activate additional settings:
 *
 * #define PJ_CONFIG_NOKIA_APS_DIRECT
 *   Use this macro to activate the APS-Direct feature. Please see
 *   http://trac.pjsip.org/repos/wiki/Nokia_APS_VAS_Direct for more 
 *   info.
 *
 * #define PJ_CONFIG_WIN32_WMME_DIRECT
 *   Configuration to activate "APS-Direct" media mode on Windows or
 *   Windows Mobile, useful for testing purposes only.
 */

#define PJ_CONFIG_ANDROID   1
#define PJ_HAS_TCP          1

/*
 * Android sample settings.
 */
#if PJ_CONFIG_ANDROID

    #define PJ_ANDROID                          1

 	// Reduce the SIP header to bypass Android bug
    #define PJSIP_INCLUDE_ALLOW_HDR_IN_DLG      0
    #define PJMEDIA_ADD_RTPMAP_FOR_STATIC_PT    0
    #define PJMEDIA_ADVERTISE_RTCP              0

    /*
     * PJLIB settings.
     */

    /* Disable floating point support */
    #undef PJ_HAS_FLOATING_POINT
    #define PJ_HAS_FLOATING_POINT       0

    /*
     * PJMEDIA settings
     */

    /* We have our own OpenSL ES backend */
    #define PJMEDIA_AUDIO_DEV_HAS_PORTAUDIO     0
    #define PJMEDIA_AUDIO_DEV_HAS_WMME          0
    #define PJMEDIA_AUDIO_DEV_HAS_OPENSL        1
    #define PJMEDIA_AUDIO_DEV_HAS_ANDROID_JNI   0

    /* Disable some codecs */
    // #define PJMEDIA_HAS_L16_CODEC     0
    // #define PJMEDIA_HAS_G722_CODEC    0

    /* Fine tune Speex's default settings for best performance/quality */
    #define PJMEDIA_CODEC_SPEEX_DEFAULT_QUALITY    5

    /* DTMF */
    #define PJMEDIA_RTP_PT_TELEPHONE_EVENTS        101
    #define PJMEDIA_RTP_PT_TELEPHONE_EVENTS_STR    "101"

    /* Fixing the deadlock */
//    #define PJMEDIA_AUDIO_DEV_HAS_ANDROID_JNI    1
//    #define PJMEDIA_AUDIO_DEV_HAS_OPENSL         0

    /*
     * PJSIP settings.
     */

    /* Increase allowable packet size, just in case */
    //#define PJSIP_MAX_PKT_LEN         2000

    /*
     * PJSUA settings.
     */

    /* Default codec quality, previously was set to 5, however it is now
     * set to 4 to make sure pjsua instantiates resampler with small filter.
     */
    #define PJSUA_DEFAULT_CODEC_QUALITY    4

    /* Set maximum number of dialog/transaction/calls to minimum */
    #define PJSIP_MAX_TSX_COUNT       31
    #define PJSIP_MAX_DIALOG_COUNT    31
    #define PJSUA_MAX_CALLS           4

    /* Other pjsua settings */
    #define PJSUA_MAX_ACC           4
    #define PJSUA_MAX_PLAYERS       4
    #define PJSUA_MAX_RECORDERS     4
    #define PJSUA_MAX_CONF_PORTS    (PJSUA_MAX_CALLS+2*PJSUA_MAX_PLAYERS)
    #define PJSUA_MAX_BUDDIES       32
#endif