//
//  Session.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/6/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import Foundation

private let kUserKey = "com.bandwidth.CurrentUser"

/**
    Structure representing an active user session
*/
struct Session {
    
    let user: User
    
    private static var _currentSession: Session?
    
    static var currentSession: Session? {
        
        get {
        
            if _currentSession == nil {
        
                // If a session is not yet available, attempt to load it from NSUserDefaults
        
                let defaults = NSUserDefaults.standardUserDefaults()
                
                if let userData = defaults.objectForKey(kUserKey) as? NSData {
                    
                    if let userDic = NSJSONSerialization.JSONObjectWithData(userData, options: nil, error: nil) as? [String: AnyObject] {
                        
                        if let user = User.fromJSON(userDic) {
                            
                            _currentSession = Session(user: user)
                        }
                    }
                }
                
            }
                
            return _currentSession
        }
        
        set {
            
            _currentSession = newValue
            
            // Saves the new session (or removes the existing session if the new value is nil) in NSUserDefaults
            
            let defaults = NSUserDefaults.standardUserDefaults()
            
            if _currentSession != nil {
                
                let userDic = _currentSession!.user.toJSON()
                
                if let userData = NSJSONSerialization.dataWithJSONObject(userDic, options: nil, error: nil) {
                    
                    defaults.setObject(userData, forKey: kUserKey)
                }

            } else {
                
                defaults.removeObjectForKey(kUserKey)
            }
            
            defaults.synchronize()
        }
    }
}
