//
//  SIPManager.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/6/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import UIKit
import AVFoundation

private let kToneVolume: Float = 0.006

/**
    Manager class responsible for interacting with the BWSip framework
*/
class SIPManager: NSObject {
    
    // MARK: - Inner objects
    
    /**
        The possible states for the registration with the SIP registrar
    */
    @objc enum RegistrationState: Int {
        
        case NotRegistered
        
        case Registering
        
        case Registered
    }
    
    /**
        Notification sent when a call is received
    */
    struct CallReceivedNotification {
        
        /**
            Notification name
        */
        static let name = "SIPManager.CallReceivedNotification"
        
        /**
            The key for the call object inside the notification's userInfo dictionary
        */
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
        Current SIP registration state
    */
    dynamic private(set) var registrationState = RegistrationState.NotRegistered
    
    // MARK: - Private properties
    
    private var phone: BWPhone!
    
    private var account: BWAccount!
    
    private var lastRegisteredUser: User?
    
    private let reachability: Reachability
    
    private var currentNetworkStatus: NetworkStatus
    
    private var backgroundTaskId: UIBackgroundTaskIdentifier?
    
    // MARK: - Class methods
    
    /**
        Default initializer
    */
    private override init() {
        
        reachability = Reachability.reachabilityForInternetConnection()
        
        currentNetworkStatus = reachability.currentReachabilityStatus()
        
        super.init()
        
        reachability.reachableBlock = { reach in
            
            dispatch_async(dispatch_get_main_queue()) {
                
                self.updateConnectionStatus(reach.currentReachabilityStatus())
            }
        }
        
        reachability.unreachableBlock = { reach in
            
            dispatch_async(dispatch_get_main_queue()) {
                
                self.updateConnectionStatus(reach.currentReachabilityStatus())
            }
        }
        
        reachability.startNotifier()
        
        NSNotificationCenter.defaultCenter().addObserver(self, selector: "whenApplicationDidEnterBackground:", name: UIApplicationDidEnterBackgroundNotification, object: nil)
        NSNotificationCenter.defaultCenter().addObserver(self, selector: "whenApplicationWillEnterForeground:", name: UIApplicationWillEnterForegroundNotification, object: nil)
    }
    
    /**
        Default deinitializer
    */
    deinit {
        
        reachability.stopNotifier()
        
        NSNotificationCenter.defaultCenter().removeObserver(self, name: UIApplicationDidEnterBackgroundNotification, object: nil)
        NSNotificationCenter.defaultCenter().removeObserver(self, name: UIApplicationWillEnterForegroundNotification, object: nil)
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
            phone.transportType = .TCP
            
            phone.initialize()
        }
        
        // Do not attempt to register again if a registration is already being processed
        
        if registrationState != .Registering {
            
            if account == nil {
            
                // If we don't have a BWAccount object, create a new registration
                
                registrationState = .Registering
                
                account = BWAccount(phone: phone!)
                account.delegate = self
                
                account.setRegistrationInterval(1200)
                account.setRegistrationFirstRetryInterval(8)
                account.setRegistrationRetryInterval(60)
                
                account.setRegistrar(user.endpoint.credentials.realm)
                account.setUsername(user.endpoint.credentials.username, andPassword: user.password)
                
                account.connect()
                
                // Initiates the iOS-managed keep-alive handler (minimum allowed interval is 600 seconds)
                
                UIApplication.sharedApplication().setKeepAliveTimeout(600) {
                 
                    self.keepAlive()
                }
                
            } else {
                
                // Otherwise, just force-update the registration
                
                account.updateRegistration(true)
            }
        }
        
        lastRegisteredUser = user
    }
    
    /**
        Unregisters the user from the SIP registrar, if a registration is active
    */
    func unregister() {
        
        UIApplication.sharedApplication().clearKeepAliveTimeout()
        
        account?.close()
        account = nil
        
        phone?.close()
        phone = nil
        
        lastRegisteredUser = nil
        
        registrationState = .NotRegistered
    }
    
    /**
        Makes a call to a phone number
        
        :param: number the US phone number to call, without country code or formatting (i.e. "5552221111")
        :returns: the created call object, or nil if the call couldn't be created
    */
    func makeCallTo(number: String) -> BWCall? {
        
        if registrationState == .Registered {
            
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
    
    /**
        Mutes the microphone while in an active call

        :param: call the call object
        :param: mute true to mute the call, false otherwise
    */
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
    
    /**
        Enables or disables the loudspeaker (can also be used while in a call)
        
        :param: enabled true to enable the loudspeaker, false otherwise
    */
    func setSpeakerEnabled(enabled: Bool) {
        
        phone.setAudioOutputRoute(enabled ? .Loudspeaker : .Earpiece)
    }
}

// MARK: - Private methods

private extension SIPManager {
    
    /**
        This method is called by the Reachability watcher whenever the network status changes, and will reconnect to
        the SIP registrar as appropriate.
        
        :param: status the new network status
    */
    private func updateConnectionStatus(status: NetworkStatus) {
        
        if currentNetworkStatus != status {
            
            registrationState = .NotRegistered
            
            account?.close()
            account = nil
            
            phone?.close()
            phone = nil
            
            if lastRegisteredUser != nil && status != .NotReachable {
                
                registerWithUser(lastRegisteredUser!)
            }
            
            currentNetworkStatus = status
        }
    }
    
    /**
        Initiates a new background task if one is not already in progress
    */
    private func beginBackgroundTaskIfNeeded() {
        
        if backgroundTaskId == nil {
            
            backgroundTaskId = UIApplication.sharedApplication().beginBackgroundTaskWithExpirationHandler() {
                
                self.endBackgroundTaskIfNeeded()
            }
        }
    }
    
    /**
        Ends a background task currently in progress, if available
    */
    private func endBackgroundTaskIfNeeded() {
        
        if backgroundTaskId != nil {
            
            UIApplication.sharedApplication().endBackgroundTask(backgroundTaskId!)
            
            backgroundTaskId = nil
        }
    }
    
    /**
        Called by the keep-alive handler to maintain the connection to the SIP registrar while the app
        is in the background
    */
    private func keepAlive() {
        
        if lastRegisteredUser != nil {
            
            // Starts a new background task to allow time for the registrar to respond
            
            beginBackgroundTaskIfNeeded()
            
            // Updates the registration
            
            registerWithUser(lastRegisteredUser!)
        }
    }
}

// MARK: - Notification handlers

extension SIPManager {
    
    func whenApplicationDidEnterBackground(notification: NSNotification) {
        
        if lastRegisteredUser != nil {
            
            // Starts a new background task to allow time for the registrar to respond
            
            beginBackgroundTaskIfNeeded()
            
            // Updates the registration
            
            registerWithUser(lastRegisteredUser!)
        }
    }
    
    func whenApplicationWillEnterForeground(notification: NSNotification) {
        
        endBackgroundTaskIfNeeded()
    }
}

// MARK: - BWAccountDelegate methods

extension SIPManager: BWAccountDelegate {
    
    func onRegStateChanged(account: BWAccount!) {
        
        println("***** Registration State Changed *****")
        println("State code: \(account.lastState.rawValue)")
        println("Expiration: \(account.registrationRegistrarInterval)")
        
        registrationState = account.isRegistrationActive() ? .Registered : .NotRegistered
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


