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

private let kToneVolume: Float = 0.006

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
            
            let dialpad = segue.destinationViewController as! DialpadViewController
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
        
        deleteTimer = NSTimer.scheduledTimerWithTimeInterval(kClearTimeInvertal, target: self, selector: #selector(DialerViewController.clearDialedNumber), userInfo: nil, repeats: false)
    }
    
    @IBAction func onDeleteTouchUp(sender: AnyObject) {
        
        deleteTimer!.invalidate()
        deleteTimer = nil
        
        if let newNumber = NumberFormatter.inputDigit("", inFormattedNumber: dialedNumberLabel.text!) {
            
            dialedNumberLabel.text = newNumber
        }
        
        deleteButton.hidden = (dialedNumberLabel.text!).characters.count == 0
        callButton.enabled = (dialedNumberLabel.text!).characters.count == 14
    }
    
    @IBAction func onCall(sender: AnyObject) {
        
        if let _ = tabBarController as? MainTabBarController {
            
            CallRouter.sharedInstance.makeCallTo(NumberFormatter.removeFormatting(dialedNumberLabel.text!))
        }
    }
}

// MARK: - DialpadViewControllerDelegate methods

extension DialerViewController: DialpadViewControllerDelegate {
    
    func dialpad(dialpad: DialpadViewController, didStartDialingDigit digit: String) {
        
        SIPManager.sharedManager().startDigit(digit)
    }
    
    func dialpad(dialpad: DialpadViewController, didEndDialingDigit digit: String, cancelled cancel: Bool) {
        
        SIPManager.sharedManager().stopDigit()
        
        if !cancel {
        
            if let newNumber = NumberFormatter.inputDigit(digit, inFormattedNumber: dialedNumberLabel.text!) {
                
                dialedNumberLabel.text = newNumber
            }
            
            deleteButton.hidden = (dialedNumberLabel.text!).characters.count == 0
            callButton.enabled = (dialedNumberLabel.text!).characters.count == 14
        }
    }
}
