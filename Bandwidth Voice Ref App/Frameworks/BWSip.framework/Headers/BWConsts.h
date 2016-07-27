//
//  BWConsts.h
//  bwsip-framework
//
//  Created by Vinícius Egidio on 4/3/14.
//

// Audio output route
typedef NS_ENUM(NSInteger, BWOutputRoute) {
	BWOutputRouteEarpiece = 0,
	BWOutputRouteLoudspeaker
};

// Transport type
typedef NS_ENUM(NSInteger, BWTransport) {
	BWTransportUDP = 0,
	BWTransportTCP,
	BWTransportTLS
};

// Call states
typedef NS_ENUM(NSInteger, BWCallState) {
	BWCallStateNull = 0,
	BWCallStateCalling,
	BWCallStateIncoming,
	BWCallStateEarly,
	BWCallStateConnecting,
	BWCallStateConfirmed,
	BWCallStateDisconnected
};

// SIP Responses
typedef NS_ENUM(NSInteger, BWSipResponse) {
	BWSipResponseTrying             = 100,
	BWSipResponseRinging            = 180,
	BWSipResponseCallBeingForwarded = 181,
	BWSipResponseQueued             = 182,
	BWSipResponseProgress           = 183,
	
	BWSipResponseOK       = 200,
	BWSipResponseAccepted = 202,
	
	BWSipResponseMultipleChoices    = 300,
	BWSipResponseMovedPermanently   = 301,
	BWSipResponseMovedTemporarily   = 302,
	BWSipResponseUseProxy           = 305,
	BWSipResponseAlternativeService = 380,
	
	BWSipResponseBadRequest                  = 400,
	BWSipResponseUnauthorized                = 401,
	BWSipResponsePaymentRequired             = 402,
	BWSipResponseForbidden                   = 403,
	BWSipResponseNotFound                    = 404,
	BWSipResponseMethodNotAllowed            = 405,
	BWSipResponseNotAcceptable               = 406,
	BWSipResponseProxyAuthenticationRequired = 407,
	BWSipResponseRequestTimeout              = 408,
	BWSipResponseGone                        = 410,
	BWSipResponseRequestEntityTooLarge       = 413,
	BWSipResponseRequestUriTooLong           = 414,
	BWSipResponseUnsupportedMediaType        = 415,
	BWSipResponseUnsupportedUriScheme        = 416,
	BWSipResponseBadExtension                = 420,
	BWSipResponseExtensionRequired           = 421,
	BWSipResponseSessionTimerTooSmall        = 422,
	BWSipResponseIntervalTooBrief            = 423,
	BWSipResponseTemporarilyUnavailable      = 480,
	BWSipResponseCallTsxDoesNotExist         = 481,
	BWSipResponseLoopDetected                = 482,
	BWSipResponseTooManyHops                 = 483,
	BWSipResponseAddressIncomplete           = 484,
	BWSipResponseAmbiguous                   = 485,
	BWSipResponseBusyHere                    = 486,
	BWSipResponseRequestTerminated           = 487,
	BWSipResponseNotAcceptableHere           = 488,
	BWSipResponseBadEvent                    = 489,
	BWSipResponseRequestUpdated              = 490,
	BWSipResponseRequestPending              = 491,
	BWSipResponseUndecipherable              = 493,
	
	BWSipResponseInternalServerError = 500,
	BWSipResponseNotImplemented      = 501,
	BWSipResponseBadGateway          = 502,
	BWSipResponseServiceUnavailable  = 503,
	BWSipResponseServerTimeout       = 504,
	BWSipResponseVersionNotSupported = 505,
	BWSipResponseMessageTooLarge     = 513,
	BWSipResponsePreconditionFailure = 580,
	
	BWSipResponseBusyEverywhere        = 600,
	BWSipResponseDecline               = 603,
	BWSipResponseDoesNotExistAnywhere  = 604,
	BWSipResponseNotAcceptableAnywhere = 606
};