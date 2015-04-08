//
//  CallRouter.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/8/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import UIKit

private let kCallViewControllerIdentifier = "CallViewController"

/**
    Class responsible for monitoring incoming calls and presenting the call interface as needed
*/
class CallRouter: NSObject {
    
    // MARK: - Public properties
    
    /**
        Shared instance through which this class must be accessed
    */
    class var sharedInstance: CallRouter {
        
        struct Singleton {
            
            static let instance = CallRouter()
        }
        
        return Singleton.instance
    }
    
    // MARK: - Superclass methods
    
    private override init() {
        
        super.init()
        
        NSNotificationCenter.defaultCenter().addObserver(self, selector: "whenCallIsReceived:", name: SIPManager.CallReceivedNotification.name, object: nil)
    }
    
    deinit {
        
        NSNotificationCenter.defaultCenter().removeObserver(self, name: SIPManager.CallReceivedNotification.name, object: nil)
    }
    
    // MARK: - Class methods
    
    func makeCallTo(number: String) {
        
        if let call = SIPManager.sharedInstance.makeCallTo(number) {
            
            presentCallViewController(call)
        }
    }
}

// MARK: - Private methods

private extension CallRouter {
    
    private func isPresentingCallViewController() -> Bool {
        
        if let appDelegate = UIApplication.sharedApplication().delegate as? AppDelegate {
            
            if let mainNav = appDelegate.window?.rootViewController as? UINavigationController {
                
                return mainNav.presentedViewController != nil && mainNav.presentedViewController! is CallViewController
            }
        }
        
        return false
    }
    
    private func presentCallViewController(call: BWCall) {
        
        if let appDelegate = UIApplication.sharedApplication().delegate as? AppDelegate {
            
            if let mainNav = appDelegate.window?.rootViewController as? UINavigationController {
                
                let storyboard = UIStoryboard(name: "Main", bundle: nil)
                
                let callVc = storyboard.instantiateViewControllerWithIdentifier(kCallViewControllerIdentifier) as CallViewController
                callVc.call = call
                
                mainNav.presentViewController(callVc, animated: true, completion: nil)
            }
        }
    }
}

// MARK: - Notification handlers

extension CallRouter {
    
    func whenCallIsReceived(notification: NSNotification) {
        
        // Check if the notification includes a BWCall object
        
        if let call = notification.userInfo?[SIPManager.CallReceivedNotification.callKey] as? BWCall {
            
            // Check if we already have a call in progress
            
            if isPresentingCallViewController() {
                
                // If we do, respond to the call as busy
                
                SIPManager.sharedInstance.respondToCall(call, busy: true)
                
            } else {
                
                // If we don't, present the incoming call interface
                
                presentCallViewController(call)
            }
        }
    }
}
