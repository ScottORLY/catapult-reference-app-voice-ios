//
//  RegistrationViewController.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/6/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import UIKit

private let kShowMainScreenSegue = "showMainScreen"

private let kPasswordLength = 16

private let kPasswordPossibleCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

/**
    View controller that allows the user to register a new account
*/
class RegistrationViewController: UIViewController {

    // MARK: - Outlets
    
    @IBOutlet private weak var usernameTextField: UITextField!

    @IBOutlet private weak var createUserButton: UIButton!
    
    @IBOutlet private weak var activityIndicator: UIActivityIndicatorView!
    
    // MARK: - Superclass methods
    
    override func viewWillAppear(animated: Bool) {
    
        super.viewWillAppear(animated)
                
        navigationItem.hidesBackButton = true
    }
    
    // MARK: - Class methods
    
    func generatePassword() -> String {
        
        let possibleCount = UInt32(countElements(kPasswordPossibleCharacters))
        
        var password = ""
        
        for i in 0 ..< kPasswordLength {
            
            let index = Int(arc4random_uniform(possibleCount))
            
            let position = advance(kPasswordPossibleCharacters.startIndex, index)
            
            password.append(kPasswordPossibleCharacters[position])
        }
        
        return password
    }
    
    func createUser() {
        
        let username = usernameTextField.text.stringByTrimmingCharactersInSet(NSCharacterSet.whitespaceAndNewlineCharacterSet())
        
        if countElements(username) > 0 {
            
            usernameTextField.enabled = false
            createUserButton.hidden = true
            
            activityIndicator.startAnimating()
            
            CatapultService.sharedInstance.createOrFetchUser(username, password: generatePassword()) { response in
                
                self.activityIndicator.stopAnimating()
                
                self.usernameTextField.enabled = true
                self.createUserButton.hidden = false
                
                switch response {
                    
                case .Success(let boxedValue):
                    
                    // Create a new session and move to the main screen
                    
                    Session.currentSession = Session(user: boxedValue.unbox)
                    
                    self.performSegueWithIdentifier(kShowMainScreenSegue, sender: nil)
                    
                case .Failure(let error):
                    
                    println("ERROR - could not create user: \(error)")
                    
                    self.showAlert("Error", message: "An unexpected error occured, please try again later.")
                }
            }
            
        } else {
            
            showAlert("Error", message: "Username cannot be empty.")
        }
    }
    
    func showAlert(title: String?, message: String) {
        
        let alertView = UIAlertView(title: title, message: message, delegate: nil, cancelButtonTitle: "OK")
        
        alertView.show()
    }
}

// MARK: - Action handlers

extension RegistrationViewController {
    
    @IBAction func onCreateUser(sender: AnyObject) {
        
        createUser()
    }
}

// MARK: - UITextFieldDelegate methods

extension RegistrationViewController: UITextFieldDelegate {
    
    func textFieldShouldReturn(textField: UITextField) -> Bool {
        
        if textField === usernameTextField {

            createUser()
            
            usernameTextField.resignFirstResponder()
        }
        
        return true
    }
}
