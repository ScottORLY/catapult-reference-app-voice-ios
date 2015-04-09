//
//  SIPManager.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/6/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import Foundation
import AVFoundation

private let kToneVolume: Float = 0.006

/**
    Manager class responsible for interacting with the BWSip framework
*/
class SIPManager: NSObject {
    
    // MARK: - Inner objects
    
    struct CallReceivedNotification {
        
        static let name = "SIPManager.CallReceivedNotification"
        static let callKey = "call"
    }
    
    // MARK: - Public properties
    
    /**
        Shared instance through which this class must be accessed
    */
    class var sharedInstance: SIPManager {
        
        struct Singleton {
            
            static let instance = SIPManager()
        }
        
        return Singleton.instance
    }
    
    /**
        Flag indicating if there's an active SIP registration. Observable through KVO
    */
    dynamic private(set) var registered = false
    
    // MARK: - Private properties
    
    private var phone: BWPhone!
    
    private var account: BWAccount!
    
    private var activeCall: BWCall?
    
    // MARK: - Class methods
    
    /**
        Default initializer
    */
    private override init() {
        
    }
    
    /**
        Register with the SIP registrar using the credentials stored in the user
        
        :param: user the user object containing the credentials
    */
    func registerWithUser(user: User) {
        
        if phone == nil {
                
            phone = BWPhone.sharedInstance()
            
            // This is the most verbose log level, useful during development
            phone.logLevel = 9
            
            // Both TCP and UDP are suppoted, but TCP is recommended for background usage due
            // to special exemptions provided by the iOS SDK to TCP sockets
            phone.transportType = .UDP
            
            phone.initialize()
        }
        
        if account == nil {
            
            account = BWAccount(phone: phone!)
            account.delegate = self
            
            account.setRegistrationInterval(1200)
            account.setRegistrationFirstRetryInterval(8)
            account.setRegistrationRetryInterval(60)
            
            account.setRegistrar(user.endpoint.credentials.realm)
            account.setUsername(user.endpoint.credentials.username, andPassword: user.password)
            
            account.connect()
        }
    }
    
    /**
        Unregisters the user from the SIP registrar, if a registration is active
    */
    func unregister() {
        
        if account != nil {
            
            account.close()
            account = nil
        }
        
        if phone != nil {
            
            phone.close()
            phone = nil
        }
        
        registered = false
    }
    
    /**
        Makes a call to a phone number
        
        :param: number the US phone number to call, without country code or formatting (i.e. "5552221111")
        :returns: the created call object, or nil if the call couldn't be created
    */
    func makeCallTo(number: String) -> BWCall? {
        
        if registered {
            
            let call = BWCall(account: account!)
            call.setRemoteUri("+1\(number)@\(Session.currentSession!.user.endpoint.credentials.realm)")
            
            call.makeCall()
            
            return call
        }
        
        return nil
    }
    
    /**
        Acknowledges an incoming call
            
        :param: call the incoming call object
        :param: busy if true a busy signal will be sent, otherwise the other side will know this extension is ringing
    */
    func respondToCall(call: BWCall, busy: Bool) {
        
        call.answerCall(busy ? .BusyHere : .Ringing)
    }
    
    /**
        Answers an incoming call
    
        :param: call the incoming call object
    */
    func answerCall(call: BWCall) {
        
        call.answerCall(.OK)
    }
    
    /**
        Hangs up an active call
        
        :param: call the call object
    */
    func hangUpCall(call: BWCall) {
        
        call.hangupCall()
    }
    
    func muteCall(call: BWCall, mute: Bool) {
        
        call.setMute(mute)
    }
    
    /**
        Produces an audible DTMF tone, and optionally sends the DTMF signal through a call
        
        :param: call call to send the DTMF through, or nil
        :param: digit the digit to produce the DTMF tone
    */
    func dialDTMFDigit(call: BWCall?, digit: String) {
    
        if call != nil {
            
            call!.dialDTMF(digit)
            
        } else {
            
            // If not on a call, we need to set our own audio session before playing the tone
            
            AVAudioSession.sharedInstance().setCategory(AVAudioSessionCategoryAmbient, error: nil)
        }

        BWTone.playDigit(digit, withVolume: kToneVolume)
    }
    
    func setSpeakerEnabled(enabled: Bool) {
        
        phone.setAudioOutputRoute(enabled ? .Loudspeaker : .Earpiece)
    }
}

// MARK: - BWAccountDelegate methods

extension SIPManager: BWAccountDelegate {
    
    func onRegStateChanged(account: BWAccount!) {
        
        println("***** Registration State Changed *****")
        println("State code: \(account.lastState.rawValue)")
        println("Expiration: \(account.registrationRegistrarInterval)")
        
        registered = account.isRegistrationActive()
    }
    
    func onIncomingCall(call: BWCall!) {
        
        println("***** Incoming Call *****")
        println("State code.........: \(call.lastState.rawValue)")
        println("Receiving call from: \(call.remoteUri)")
        println("Parent account.....: \(call.parentAccount)")
        
        dispatch_async(dispatch_get_main_queue()) {
        
            // Make sure we are sending the notification in the main thread
            
            NSNotificationCenter.defaultCenter().postNotificationName(CallReceivedNotification.name, object: nil, userInfo: [CallReceivedNotification.callKey: call])
        }
    }
}


