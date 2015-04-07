//
//  SessionCheckViewController.swift
//  Bandwitdh Voice Ref App
//
//  Created by Fabio Rodella on 4/6/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import UIKit

private let kShowRegistrationScreenSegue = "showRegistrationScreen"

private let kShowMainScreenSegue = "showMainScreen"

/**
    View controller responsible for checking if an existing session is available, and
    redirecting the application flow as necessary.
*/
class SessionCheckViewController: UIViewController {

    // MARK: - Superclass methods
    
    override func viewDidAppear(animated: Bool) {
        
        super.viewDidAppear(animated)
        
        if Session.currentSession != nil {
            
            SIPManager.sharedInstance.registerWithUser(Session.currentSession!.user)
            
            self.performSegueWithIdentifier(kShowMainScreenSegue, sender: nil)
            
        } else {
            
            self.performSegueWithIdentifier(kShowRegistrationScreenSegue, sender: nil)
        }
    }
}
