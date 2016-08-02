//
//  SettingsViewController.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/6/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import UIKit

private let kInfoCellIdentifier = "InfoCell"

private let kRegistrationStateKey = "registrationState"

/**
    View controller class for the settings/user info screen
*/
class SettingsViewController: UIViewController {

    // MARK: - Outlets
    
    @IBOutlet private weak var activityIndicator: UIActivityIndicatorView!
    
    @IBOutlet private weak var tableView: UITableView!
    
    // MARK: - Properties
    
    private var infoEntries: [(String, String)] = []
    
    // MARK: - Superclass methods
    
    override func viewWillAppear(animated: Bool) {
        
        super.viewWillAppear(animated)
        
        ASIPManager.sharedManager().addObserver(self, forKeyPath: kRegistrationStateKey, options: .New, context: nil)
        
        updateEntries()
    }
    
    override func viewWillDisappear(animated: Bool) {
        
        super.viewWillDisappear(animated)
        
        ASIPManager.sharedManager().removeObserver(self, forKeyPath: kRegistrationStateKey)
    }
    
    override func observeValueForKeyPath(keyPath: String?, ofObject object: AnyObject?, change: [String : AnyObject]?, context: UnsafeMutablePointer<Void>) {
        
        if keyPath == kRegistrationStateKey {
            
            dispatch_async(dispatch_get_main_queue()) {
                
                self.updateEntries()
            }
        }
    }
    
    // MARK: - Class methods
    
    func updateEntries() {
        
        // Loads the values from the current session
        
        if let session = Session.currentSession {
                      
            let regStateStr = ASIPManager.regStateToString(ASIPManager.sharedManager().registrationState)
        
            infoEntries = [
                ("User name", session.user.username),
                ("Phone number", session.user.number),
                ("Endpoint name", session.user.endpoint.name),
                ("Registrar", session.user.endpoint.credentials.realm),
                ("SIP URI", session.user.endpoint.sipUri),
                ("SIP registration state", regStateStr),
                ("Server", Config.serverUrl)
            ]
            
        } else {
            
            infoEntries = []
        }
        
        tableView.reloadData()
    }
    
    func showAlert(title: String?, message: String) {
        
        let alertView = UIAlertView(title: title, message: message, delegate: nil, cancelButtonTitle: "OK")
        
        alertView.show()
    }
}

// MARK: - Action handlers

extension SettingsViewController {
    
    @IBAction func onRefreshUserInfo(sender: AnyObject) {
        
        if let session = Session.currentSession {
            
            tableView.hidden = true
            
            CatapultService.sharedInstance.createOrFetchUser(session.user.username, password: session.user.password!) { response in
                
                self.tableView.hidden = false
                
                switch response {
                    
                case .Success(let user):
                    
                    // Updates the session object
                    
                    Session.currentSession = Session(user: user)
                    
                    self.updateEntries()
                    
                case .Failure(let error):
                    
                    print("ERROR - could not refresh user: \(error)")
                    
                    self.showAlert("Error", message: "An unexpected error occurred, please try again later.")
                }
            }
        }
    }
    
    @IBAction func onClearSession(sender: AnyObject) {
        
        Session.currentSession = nil
        
        ASIPManager.sharedManager().unregister()
                
        self.navigationController!.popToRootViewControllerAnimated(true)
    }
}

// MARK: - UITableViewDataSource / UITableViewDelegate methods

extension SettingsViewController: UITableViewDataSource, UITableViewDelegate {
    
    func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        
        return 1
    }
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        
        return infoEntries.count
    }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        
        let cell = tableView.dequeueReusableCellWithIdentifier(kInfoCellIdentifier, forIndexPath: indexPath) 
        
        let (title, value) = infoEntries[indexPath.row]
        
        cell.textLabel!.text = title
        cell.detailTextLabel!.text = value
        
        return cell
    }
}
