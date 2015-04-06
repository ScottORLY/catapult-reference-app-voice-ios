//
//  SettingsViewController.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/6/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import UIKit

private let kInfoCellIdentifier = "InfoCell"

private let kRegistrationViewControllerIdentifier = "RegistrationViewController"

class SettingsViewController: UIViewController {

    // MARK: - Outlets
    
    @IBOutlet private weak var activityIndicator: UIActivityIndicatorView!
    
    @IBOutlet private weak var tableView: UITableView!
    
    // MARK: - Properties
    
    /**
        List of entries to be displayed
    */
    private var infoEntries: [(String, String)] = []
    
    // MARK: - Superclass methods
    
    override func viewWillAppear(animated: Bool) {
        
        super.viewWillAppear(animated)
        
        updateDataFromSession()
    }
    
    // MARK: - Class methods
    
    func updateDataFromSession() {
        
        // Loads the values from the current session
        
        if let session = Session.currentSession {
            
            infoEntries = [
                ("User name", session.user.username),
                ("Phone number", session.user.number),
                ("Endpoint name", session.user.endpoint.name),
                ("SIP URI", session.user.endpoint.sipUri)
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
                    
                case .Success(let boxedValue):
                    
                    // Updates the session object
                    
                    Session.currentSession = Session(user: boxedValue.unbox)
                    
                    self.updateDataFromSession()
                    
                case .Failure(let error):
                    
                    println("ERROR - could not refresh user: \(error)")
                    
                    self.showAlert("Error", message: "An unexpected error occured, please try again later.")
                }
            }
        }
    }
    
    @IBAction func onClearSession(sender: AnyObject) {
        
        Session.currentSession = nil
                
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
        
        let cell = tableView.dequeueReusableCellWithIdentifier(kInfoCellIdentifier, forIndexPath: indexPath) as UITableViewCell
        
        let (title, value) = infoEntries[indexPath.row]
        
        cell.textLabel!.text = title
        cell.detailTextLabel!.text = value
        
        return cell
    }
}
