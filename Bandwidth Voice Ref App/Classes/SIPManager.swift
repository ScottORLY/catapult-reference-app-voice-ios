//
//  SIPManager.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/6/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import UIKit
import AVFoundation

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
    static let sharedInstance = SIPManager()
    
    /**
        Current SIP registration state
    */
    dynamic private(set) var registrationState = RegistrationState.NotRegistered
    
    dynamic private(set) var registrationStateCode = 0
    
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
        
        - parameter user: the user object containing the credentials
    */
    func registerWithUser(user: User) {
        
        if phone == nil {
                
            phone = BWPhone.sharedInstance()
            
            // This is the most verbose log level, useful during development
            phone.logLevel = 9
            
            // Both TCP and UDP are supported, but TCP is recommended for background usage due
            // to special exemptions provided by the iOS SDK to TCP sockets
            phone.transportType = .TCP

            phone.initialize()
        }
        
        // Do not attempt to register again if a registration is already being processed
        
        if registrationState != .Registering {
            
            if account == nil {
            
                // If we don't have a BWAccount object, create a new registration
                
                registrationState = .Registering
                
                let credentials = BWCredentials(username: user.endpoint.credentials.username, andPassword: user.password)
                
                account = BWAccount(phone: phone!)
                account.delegate = self
                
                account.setRegistrationInterval(1200)
                account.setRegistrationFirstRetryInterval(8)
                account.setRegistrationRetryInterval(60)
                
                account.setRegistrar(user.endpoint.credentials.realm)
                account.setCredentials(credentials)
                
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
        
        - parameter number: the US phone number to call, without country code or formatting (i.e. "5552221111")
        - returns: the created call object, or nil if the call couldn't be created
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
        Enables or disables the loudspeaker (can also be used while in a call)
        
        - parameter enabled: true to enable the loudspeaker, false otherwise
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
        
        - parameter status: the new network status
    */
    private func updateConnectionStatus(status: NetworkStatus) {
        
        if currentNetworkStatus != status {
            
            beginBackgroundTaskIfNeeded()
            
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
        
        print("***** Registration State Changed *****")
        print("State code: \(account.lastState.rawValue)")
        print("Expiration: \(account.registrationRegistrarInterval)")
        
        registrationState = account.isRegistrationActive() ? .Registered : .NotRegistered
        registrationStateCode = account.lastState.rawValue
    }
    
    func onIncomingCall(call: BWCall!) {
        
        print("***** Incoming Call *****")
        print("State code.........: \(call.lastState.rawValue)")
        print("Receiving call from: \(call.remoteUri)")
        print("Parent account.....: \(call.parentAccount)")
        
        dispatch_async(dispatch_get_main_queue()) {
        
            // Make sure we are sending the notification in the main thread
            
            NSNotificationCenter.defaultCenter().postNotificationName(CallReceivedNotification.name, object: nil, userInfo: [CallReceivedNotification.callKey: call])
        }
    }
}
