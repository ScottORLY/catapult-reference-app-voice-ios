/*
 *  SoftphoneObserverProxy.h
 *  demophone
 *
 *  Created by jiri on 4/1/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include "Softphone.h"
#include "SoftphoneDelegate.h"
#include "ali_attribute.h"

/*! @class SoftphoneObserverProxy
 @brief A simple class which implements @ref Softphone::Observer and @ref Softphone::ObserverEx C++ interfaces
 and forwards their methods to @ref SoftphoneDelegate interface as Objective-C methods.
 
 The SoftphoneDelegate instance is passed to it in constructor.
 
 */

// ******************************************************************
class SoftphoneObserverProxy : public Softphone::Observer
// ******************************************************************
{
public:
	SoftphoneObserverProxy(NSObject<SoftphoneDelegate> * delegate);
	virtual ~SoftphoneObserverProxy();

    virtual void onNetworkChangeDetected(Softphone::Network::Type network) ALI_ATTRIBUTE_OVERRIDE;
#if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
    virtual void onRegistrationStateChanged(ali::string const& accountId, Registrator::State::Type state) ALI_ATTRIBUTE_OVERRIDE;
#else
	virtual void onRegistrationStateChanged(Registrator::State::Type state) ALI_ATTRIBUTE_OVERRIDE;
#endif
	virtual void onIncomingEvent(Softphone::EventHistory::Event::Pointer event) ALI_ATTRIBUTE_OVERRIDE;
    virtual void onCallStateChanged(Softphone::EventHistory::CallEvent::Pointer call,
                                    Call::State::Type state) ALI_ATTRIBUTE_OVERRIDE;
    
    virtual void onCallHoldStateChanged(Softphone::EventHistory::CallEvent::Pointer call,
                                        Call::HoldStates const& states) ALI_ATTRIBUTE_OVERRIDE;
    
    virtual void onMediaStatusChanged(Softphone::EventHistory::CallEvent::Pointer call,
                                      Call::MediaStatus const& media) ALI_ATTRIBUTE_OVERRIDE;
    
	virtual void onAudioRouteChanged(AudioRoute::Type route) ALI_ATTRIBUTE_OVERRIDE;
	
	virtual ali::string getRingtone(Softphone::EventHistory::Event::Pointer event) ALI_ATTRIBUTE_OVERRIDE;

    virtual void onEventsChanged(Softphone::EventHistory::ChangedEvents const& events,
                                 Softphone::EventHistory::ChangedStreams const& streams) ALI_ATTRIBUTE_OVERRIDE;

	virtual void onTransferOffered(Softphone::EventHistory::CallEvent::Pointer call) ALI_ATTRIBUTE_OVERRIDE;
    
    virtual void onTransferResult(Softphone::EventHistory::CallEvent::Pointer call,
                                  bool success) ALI_ATTRIBUTE_OVERRIDE;

    virtual void onSimulatedMicrophoneStopped() ALI_ATTRIBUTE_OVERRIDE;
    
#if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
    virtual void onVoicemail(ali::string const& accountId, Voicemail::Record const& voicemail) ALI_ATTRIBUTE_OVERRIDE;
#else
    virtual void onVoicemail(Voicemail::Record const& voicemail) ALI_ATTRIBUTE_OVERRIDE;
#endif
private:
	NSObject<SoftphoneDelegate> * _delegate;
};
