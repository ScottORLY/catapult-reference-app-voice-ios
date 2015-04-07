//
//  DialerViewController.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/6/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import UIKit

private let kEmbedDialpadSegue = "embedDialpad"

private let kClearTimeInvertal: NSTimeInterval = 1.0

/**
    View controller class for the dialer screen
*/
class DialerViewController: UIViewController {

    // MARK: - Outlets
    
    @IBOutlet private weak var dialedNumberLabel: UILabel!
    
    @IBOutlet private weak var deleteButton: UIButton!
    
    @IBOutlet private weak var callButton: UIButton!
    
    // MARK: - Private properties
    
    private var deleteTimer: NSTimer?
    
    // MARK: - Superclass methods
    
    override func viewDidLoad() {
        
        super.viewDidLoad()
        
        clearDialedNumber()
    }
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        
        if segue.identifier == kEmbedDialpadSegue {
            
            let dialpad = segue.destinationViewController as DialpadViewController
            dialpad.delegate = self
        }
    }
    
    // MARK: - Class methods
    
    func clearDialedNumber() {
        
        dialedNumberLabel.text = ""
        deleteButton.hidden = true
        callButton.enabled = false
    }
}

// MARK: - Action handlers

extension DialerViewController {
    
    @IBAction func onDeleteTouchDown(sender: AnyObject) {
        
        deleteTimer = NSTimer.scheduledTimerWithTimeInterval(kClearTimeInvertal, target: self, selector: "clearDialedNumber", userInfo: nil, repeats: false)
    }
    
    @IBAction func onDeleteTouchUp(sender: AnyObject) {
        
        deleteTimer!.invalidate()
        deleteTimer = nil
        
        if countElements(dialedNumberLabel.text!) > 0 {
            
            dialedNumberLabel.text!.removeAtIndex(dialedNumberLabel.text!.endIndex.predecessor())
            
            deleteButton.hidden = countElements(dialedNumberLabel.text!) == 0
            callButton.enabled = !deleteButton.hidden
        }
    }
    
    @IBAction func onCall(sender: AnyObject) {
        
    }
}

// MARK: - DialpadViewControllerDelegate methods

extension DialerViewController: DialpadViewControllerDelegate {
    
    func dialpad(dialpad: DialpadViewController, didDialDigit digit: String) {
        
        dialedNumberLabel.text! += digit
        
        deleteButton.hidden = countElements(dialedNumberLabel.text!) == 0
        callButton.enabled = !deleteButton.hidden
        
        SIPManager.sharedInstance.dialDTMFdigit(digit)
    }
}
