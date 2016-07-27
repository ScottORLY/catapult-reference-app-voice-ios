//
//  Session.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/6/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import Foundation

private let kServerKey = "com.bandwidth.AppServer"

private let kUserKey = "com.bandwidth.CurrentUser"

/**
    Structure representing an active user session
*/
struct Session {
    
    // MARK: - Public Properties
    
    /**
        The user associated to this session
    */
    let user: User
    
    static var currentSession: Session? {
        
        get {
        
            if _currentSession == nil {
            
                // If a session is not yet available, attempt to load it from NSUserDefaults
                
                let defaults = NSUserDefaults.standardUserDefaults()
                
                // First we check if the server was the same as the one we are using now
                
                if let savedServer = defaults.objectForKey(kServerKey) as? String where savedServer == Config.serverUrl {
                
                    // We then attempt to create the user structure from the stored JSON
                    
                    if let
                        userData = defaults.objectForKey(kUserKey) as? NSData,
                        userDic = (try? NSJSONSerialization.JSONObjectWithData(userData, options: [])) as? [String: AnyObject],
                        user = User.fromJSON(userDic) {
                    
                        // Finally, we create a new session with that user
                            
                        _currentSession = Session(user: user)
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
                
                if let userData = try? NSJSONSerialization.dataWithJSONObject(userDic, options: []) {
                    
                    defaults.setObject(Config.serverUrl, forKey: kServerKey)
                    
                    defaults.setObject(userData, forKey: kUserKey)
                }
                
            } else {
                
                defaults.removeObjectForKey(kUserKey)
            }
            
            defaults.synchronize()
        }
    }
    
    // MARK: - Private properties
    
    private static var _currentSession: Session?
}
