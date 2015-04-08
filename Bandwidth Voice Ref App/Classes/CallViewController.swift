//
//  CallViewController.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/8/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import UIKit

/**
    View controller class for an active call
*/
class CallViewController: UIViewController {

    // MARK: - Outlets
    
    @IBOutlet private weak var callTypeLabel: UILabel!
    
    @IBOutlet private weak var callerLabel: UILabel!
    
    @IBOutlet private weak var callDurationLabel: UILabel!
    
    @IBOutlet private weak var callConnectingIndicator: UIActivityIndicatorView!
    
    @IBOutlet private weak var hangUpButton: UIButton!
    
    @IBOutlet private weak var hangUpButtonWidth: NSLayoutConstraint!
    
    @IBOutlet private weak var answerButton: UIButton!
    
    // MARK: - Public properties
    
    var call: BWCall? {
        
        didSet {
            
            oldValue?.delegate = nil
            call?.delegate = self
        }
    }
    
    // MARK: - Private properties
    
    private var callDuration: NSNumber?
    
    private var callStartDate: NSDate?
    
    private var callDurationTimer: NSTimer?
    
    // MARK: - Superclass methods
    
    override func viewWillAppear(animated: Bool) {
        
        super.viewWillAppear(animated)
        
        if call!.isIncoming {
            
            callTypeLabel.text = "Incoming call from"
            
            callerLabel.text = call!.remoteUri
            
            SIPManager.sharedInstance.respondToCall(call!, busy: false)
            
        } else {
            
            callTypeLabel.text = "Calling"
            
            callerLabel.text = call!.remoteUri
        }
        
        updateScreenState()
    }
    
    // MARK: - Class methods
    
    func updateCallDuration() {
        
        callDuration = NSNumber(double: -callStartDate!.timeIntervalSinceNow);
        
        let seconds = lround(callDuration!.doubleValue);
        
        let mins: Int = (seconds % 3600) / 60
        let secs: Int = seconds % 60
        
        callDurationLabel.text = NSString(format: "%02d:%02d", mins, secs)
    }
}

// MARK: - Private methods

private extension CallViewController {
    
    private func updateScreenState() {
        
        if callDuration != nil {
            
            callDurationLabel.hidden = false
            
            callConnectingIndicator.stopAnimating()
            
        } else {
            
            callDurationLabel.hidden = true
            
            callConnectingIndicator.startAnimating()
        }
        
        if !call!.isIncoming || callDuration != nil {
            
            answerButton.enabled = false
            
            hangUpButtonWidth.constant = view.frame.size.width / 2
            
        } else {
            
            answerButton.enabled = true
            
            hangUpButtonWidth.constant = 0
        }
    }
    
    private func startCallDurationTimer() {
        
        callStartDate = NSDate()
        
        callDurationTimer = NSTimer.scheduledTimerWithTimeInterval(0.5, target: self, selector: "updateCallDuration", userInfo: nil, repeats: true)
        
        callDuration = NSNumber(double: 0.0)
    }
    
    private func stopCallDurationTimer() {
        
        callStartDate = nil
        callDuration = nil
        
        callDurationTimer?.invalidate()
        callDurationTimer = nil
    }
}

// MARK: - Action handlers

extension CallViewController {
    
    @IBAction func onHangUp(sender: AnyObject) {
        
        if call != nil {
            
            SIPManager.sharedInstance.hangUpCall(call!)
        }
    }

    @IBAction func onAnswer(sender: AnyObject) {
        
        if call != nil {
            
            SIPManager.sharedInstance.answerCall(call!)
        }
    }
}

// MARK: - BWCallDelegate methods

extension CallViewController: BWCallDelegate {
    
    func onCallStateChanged(call: BWCall!) {
        
        println("***** Call State Changed *****")
        println("State code: \(call.lastState.rawValue)")
        println("Local URI.: \(call.localUri)")
        println("Remote URI: \(call.remoteUri)")
        
        if self.call == call {
            
            dispatch_async(dispatch_get_main_queue()) {
            
                switch call.lastState {
                    
                case .Confirmed:
                    
                    self.startCallDurationTimer()
                    
                    break
                    
                case .Disconnected:
                    
                    self.stopCallDurationTimer()
                    
                    self.dismissViewControllerAnimated(true, completion: nil)
                    
                default:
                    
                    // Nothing to do for now
                    break
                }
                
                self.updateScreenState()
            }
        }
    }
    
    func onIncomingDTMF(call: BWCall!, andDigits digits: String!) {
        
        println("***** Incoming DTMF *****")
        println("DTMF Received: \(digits)")
    }
}
