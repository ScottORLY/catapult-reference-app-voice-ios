//
//  CallViewController.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/8/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import UIKit
import AudioToolbox
import AVFoundation

private let kRingtoneInterval: NSTimeInterval = 2.5

private let kCallDurationLabelVisibleWidth: CGFloat = 80

private let kCallTypeLabelDefaultTop: CGFloat = 76

private let kEmbedDialpadSegue = "embedDialpad"

/**
    View controller class for an active call
*/
class CallViewController: UIViewController {

    // MARK: - Outlets
    
    @IBOutlet private weak var callTypeLabel: UILabel!
    
    @IBOutlet private weak var callTypeLabelTop: NSLayoutConstraint!
    
    @IBOutlet private weak var callerLabel: UILabel!
    
    @IBOutlet private weak var callDurationLabel: UILabel!
    
    @IBOutlet private weak var callDurationLabelWidth: NSLayoutConstraint!
    
    @IBOutlet private weak var callControlsView: UIView!
    
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
    
    private var ringtoneSoundId: SystemSoundID!
    
    private var ringtoneTimer: NSTimer?
    
    // MARK: - Superclass methods
    
    override func viewWillAppear(animated: Bool) {
        
        super.viewWillAppear(animated)

        var number: NSString?
        
        let scanner = NSScanner(string: call!.remoteUri)
        
        scanner.scanString("<", intoString: nil)
        scanner.scanString("sip:", intoString: nil)
        scanner.scanUpToString("@", intoString: &number)
        
        callerLabel.text = number != nil ? NumberFormatter.formatE164Number(number!) : "BLOCKED"
        
        if call!.isIncoming {
            
            callTypeLabel.text = "Incoming call from"
            
            SIPManager.sharedInstance.respondToCall(call!, busy: false)
            
            startPlayingRingtone()
            
        } else {
            
            callTypeLabel.text = "Calling"
        }
        
        updateScreenState(false)
    }
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        
        if segue.identifier == kEmbedDialpadSegue {
            
            let dialpad = segue.destinationViewController as DialpadViewController
            dialpad.delegate = self
        }
    }
    
    // MARK: - Class methods
    
    func updateCallDuration() {
        
        callDuration = NSNumber(double: -callStartDate!.timeIntervalSinceNow);
        
        let seconds = lround(callDuration!.doubleValue);
        
        let mins: Int = (seconds % 3600) / 60
        let secs: Int = seconds % 60
        
        callDurationLabel.text = NSString(format: "%02d:%02d", mins, secs)
    }
    
    func playRingtone() {
        
        AudioServicesPlayAlertSound(ringtoneSoundId);
    }
}

// MARK: - Private methods

private extension CallViewController {
    
    private func updateScreenState(animated: Bool) {
        
        let applyChanges = { () -> Void in
            
            if self.callDuration != nil {
                
                self.callTypeLabel.hidden = true
                self.callTypeLabelTop.constant = 0.0
                self.callerLabel.textAlignment = .Left
                self.callDurationLabelWidth.constant = kCallDurationLabelVisibleWidth
                self.callControlsView.alpha = 1.0
                
            } else {
                
                self.callTypeLabel.hidden = false
                self.callTypeLabelTop.constant = kCallTypeLabelDefaultTop
                self.callerLabel.textAlignment = .Center
                self.callDurationLabelWidth.constant = 0.0
                self.callControlsView.alpha = 0.0
                
                SIPManager.sharedInstance.setSpeakerEnabled(false)
            }
            
            if !self.call!.isIncoming || self.callDuration != nil {
                
                self.answerButton.enabled = false
                self.hangUpButtonWidth.constant = self.view.frame.size.width / 2
                
            } else {
                
                self.answerButton.enabled = true
                self.hangUpButtonWidth.constant = 0
            }
        }
        
        if animated {
        
            UIView.animateWithDuration(0.3) {
                
                applyChanges();
                
                self.view.layoutIfNeeded()
            }
            
        } else {
            
            applyChanges()
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
    
    private func startPlayingRingtone() {
        
        let ringtoneUrl = NSBundle.mainBundle().URLForResource("ringtone", withExtension: "wav")
        
        var soundId: SystemSoundID = 0
        
        AudioServicesCreateSystemSoundID(ringtoneUrl, &soundId)
        
        ringtoneSoundId = soundId
        
        var error: NSError?
        
        AVAudioSession.sharedInstance().setCategory(AVAudioSessionCategoryPlayback, error: &error)
        
        AVAudioSession.sharedInstance().setActive(true, withOptions: .OptionNotifyOthersOnDeactivation, error: &error)
        
        if error == nil {
            
            playRingtone()
            
            ringtoneTimer = NSTimer.scheduledTimerWithTimeInterval(kRingtoneInterval, target: self, selector: "playRingtone", userInfo: nil, repeats: true)
        }
    }
    
    private func stopPlayingRingtone() {
        
        if ringtoneSoundId != nil {
        
            AudioServicesRemoveSystemSoundCompletion(ringtoneSoundId)
            
            AudioServicesDisposeSystemSoundID(ringtoneSoundId)
            
            ringtoneSoundId = nil
            
            ringtoneTimer!.invalidate()
            ringtoneTimer = nil
            
            AVAudioSession.sharedInstance().setActive(false, withOptions: .OptionNotifyOthersOnDeactivation, error: nil)
        }
    }
}

// MARK: - Action handlers

extension CallViewController {
    
    @IBAction func onToggleMute(sender: UIButton) {
        
        sender.selected = !sender.selected
        
        if call != nil {
            
            SIPManager.sharedInstance.muteCall(call!, mute: sender.selected)
        }
    }
    
    @IBAction func onToggleSpeaker(sender: UIButton) {
        
        sender.selected = !sender.selected
        
        SIPManager.sharedInstance.setSpeakerEnabled(sender.selected)
    }
    
    @IBAction func onHangUp(sender: AnyObject) {
        
        stopPlayingRingtone()
        
        if call != nil {
            
            SIPManager.sharedInstance.hangUpCall(call!)
        }
    }

    @IBAction func onAnswer(sender: AnyObject) {
        
        stopPlayingRingtone()
        
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
                    
                    UIDevice.currentDevice().proximityMonitoringEnabled = true
                    
                    self.startCallDurationTimer()
                    
                    break
                    
                case .Disconnected:
                    
                    UIDevice.currentDevice().proximityMonitoringEnabled = false
                    
                    self.stopCallDurationTimer()
                    
                    self.dismissViewControllerAnimated(true, completion: nil)
                    
                default:
                    
                    // Nothing to do for now
                    break
                }
                
                self.updateScreenState(true)
            }
        }
    }
    
    func onIncomingDTMF(call: BWCall!, andDigits digits: String!) {
        
        println("***** Incoming DTMF *****")
        println("DTMF Received: \(digits)")
    }
}

// MARK: - DialpadViewControllerDelegate methods

extension CallViewController: DialpadViewControllerDelegate {
    
    func dialpad(dialpad: DialpadViewController, didDialDigit digit: String) {
        
        SIPManager.sharedInstance.dialDTMFDigit(call!, digit: digit)
    }
}
