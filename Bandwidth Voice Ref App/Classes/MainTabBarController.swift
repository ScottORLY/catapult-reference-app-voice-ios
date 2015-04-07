//
//  MainTabBarController.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/6/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import UIKit

/**
    Tab bar controller for the main screen
*/
class MainTabBarController: UITabBarController {

    // MARK: - Superclass methods
    
    override func viewWillAppear(animated: Bool) {
        
        super.viewWillAppear(animated)
        
        navigationItem.hidesBackButton = true
        navigationItem.title = "Bandwidth Reference App"
    }
}
