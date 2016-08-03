 //
 //  SoftphoneObserverProxy.h
 //  Bandwidth Voice Ref App
 //
 //  Copyright © 2016 Bandwidth. All rights reserved.
 //
#pragma once

#include "Softphone.h"
#include "SIPManager.h"
#include "ali_attribute.h"

/*! @class SoftphoneObserverProxy
 @brief A simple class which implements @ref Softphone::Observer and @ref Softphone::ObserverEx C++ interfaces
 and forwards their methods to @ref SipManager as Objective-C methods.
 
 */
class SoftphoneObserverProxy : public Softphone::Observer {
public:
	SoftphoneObserverProxy(SIPManager* delegate);
	virtual ~SoftphoneObserverProxy();

    virtual void onNetworkChangeDetected(Softphone::Network::Type network) ALI_ATTRIBUTE_OVERRIDE;

    virtual void onRegistrationStateChanged(ali::string const& accountId, Registrator::State::Type state) ALI_ATTRIBUTE_OVERRIDE;

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
    
    virtual void onVoicemail(ali::string const& accountId, Voicemail::Record const& voicemail) ALI_ATTRIBUTE_OVERRIDE;
    
    static RegistrationState acrobbitsRegStateToBWRegState(Registrator::State::Type state);
    static CallState acrobbitsCallStateToBWCallState(Call::State::Type state);

private:
    
    SIPManager* sipManager;
};
