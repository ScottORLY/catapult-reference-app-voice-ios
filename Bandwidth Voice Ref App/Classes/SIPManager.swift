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
    
    /**
        The current duration for the active call in seconds. Observable through KVO
    */
    dynamic private(set) var activeCallDuration: NSTimeInterval = 0.0
    
    // MARK: - Private properties
    
    private var phone: BWPhone!
    
    private var account: BWAccount!
    
    private var activeCall: BWCall?

    private var activeCallStartDate: NSDate?
    
    private var activeCallDurationTimer: NSTimer?
    
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
    
    func dialDTMFdigit(digit: String) {
    
        if activeCall != nil {
            
            activeCall!.dialDTMF(digit)
            
        } else {
            
            // If not on a call, we need to set our own audio session before playing the tone
            
            AVAudioSession.sharedInstance().setCategory(AVAudioSessionCategoryAmbient, error: nil)
        }

        BWTone.playDigit(digit, withVolume: kToneVolume)
    }
}

// MARK: - Private methods

private extension SIPManager {
    
    private func startCallDurationTimer() {
        
        activeCallStartDate = NSDate()
        
        activeCallDurationTimer = NSTimer.scheduledTimerWithTimeInterval(0.5, target: self, selector: "updateCallDuration", userInfo: nil, repeats: true)
        
        activeCallDuration = 0.0
    }
    
    private func updateCallDuration() {
        
        activeCallDuration = -activeCallStartDate!.timeIntervalSinceNow;
    }
    
    private func stopCallDurationTimer() {
        
        activeCallStartDate = nil
        
        activeCallDurationTimer!.invalidate()
        activeCallDurationTimer = nil
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
        
        if activeCall == nil {
            
            activeCall = call
            activeCall!.delegate = self
            
            activeCall!.answerCall(.Ringing)
            
        } else {
            
            activeCall!.answerCall(.BusyHere)
        }
    }
}

// MARK: - BWCallDelegate methods

extension SIPManager: BWCallDelegate {
    
    func onCallStateChanged(call: BWCall!) {
        
        println("***** Call State Changed *****")
        println("State code: \(call.lastState.rawValue)")
        println("Local URI.: \(call.localUri)")
        println("Remote URI: \(call.remoteUri)")
        
        if activeCall == call {
            
            switch call.lastState {
                
            case .Confirmed:
                
                startCallDurationTimer()
                
                break
                
            case .Disconnected:
                
                stopCallDurationTimer()
                
                activeCall = nil
                
            default:
                
                // Nothing to do for now
                break
            }
        }
    }
    
    func onIncomingDTMF(call: BWCall!, andDigits digits: String!) {
        
        println("***** Incoming DTMF *****")
        println("DTMF Received: \(digits)")
    }
}
