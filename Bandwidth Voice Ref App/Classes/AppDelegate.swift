//
//  AppDelegate.swift
//  Bandwitdh Voice Ref App
//
//  Created by Fabio Rodella on 4/2/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import UIKit

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {

    var window: UIWindow?

    func application(application: UIApplication, didFinishLaunchingWithOptions launchOptions: [NSObject: AnyObject]?) -> Bool {

        // Only asks the user for notification permissions on iOS 8 (local notifications do not require permissions on earlier versions)
        
        if application.respondsToSelector("registerUserNotificationSettings:") {
            
            let settings = UIUserNotificationSettings(forTypes: .Badge | .Sound | .Alert, categories: nil)
            
            application.registerUserNotificationSettings(settings)
        }
        
        return true
    }
}

