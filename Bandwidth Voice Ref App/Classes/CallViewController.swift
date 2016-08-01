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

private let kToneVolume: Float = 0.006

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
    var call: NSNumber? {
        
        didSet {
/*FIXME
            oldValue?.delegate = nil
            call?.delegate = self*/
        }
    }
    
    // MARK: - Private properties
    
    private var callDuration: NSNumber?
    
    private var callStartDate: NSDate?
    
    private var callDurationTimer: NSTimer?
    
    private var ringtoneSoundId: SystemSoundID!
    
    private var ringtoneTimer: NSTimer?
    
    private var callNotification: UILocalNotification?
    
    // MARK: - Superclass methods
    
    override func viewWillAppear(animated: Bool) {
        
        super.viewWillAppear(animated)

        var number: NSString?
        /* FIXME:
        let scanner = NSScanner(string: call!.remoteUri)
        
        scanner.scanString("<", intoString: nil)
        scanner.scanString("sip:", intoString: nil)
        scanner.scanUpToString("@", intoString: &number)
        
        callerLabel.text = number != nil ? NumberFormatter.formatE164Number(number! as String) : "BLOCKED"
        
        if call!.isIncoming {
            
            callTypeLabel.text = "Incoming call from"
            
            call!.answerCall(.Ringing)
            
            startPlayingRingtone()
            
        } else {
            
            callTypeLabel.text = "Calling"
        }
        */
        updateScreenState(false)
    }
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        
        if segue.identifier == kEmbedDialpadSegue {
            
            let dialpad = segue.destinationViewController as! DialpadViewController
            dialpad.delegate = self
        }
    }
    
    // MARK: - Class methods
    
    func updateCallDuration() {
        
        if callStartDate != nil {
        
            callDuration = NSNumber(double: -callStartDate!.timeIntervalSinceNow);
            
            let seconds = lround(callDuration!.doubleValue);
            
            let mins: Int = (seconds % 3600) / 60
            let secs: Int = seconds % 60
            
            callDurationLabel.text = NSString(format: "%02d:%02d", mins, secs) as String
        }
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
                
                // FIXME: SIPManager.sharedInstance.setSpeakerEnabled(false)
            }
            /* FIXME:
            if !self.call!.isIncoming || self.callDuration != nil {
                
                self.answerButton.enabled = false
                self.hangUpButtonWidth.constant = self.view.frame.size.width / 2
                
            } else {
                
                self.answerButton.enabled = true
                self.hangUpButtonWidth.constant = 0
            }*/
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
        
        if UIApplication.sharedApplication().applicationState == .Active {
        
            let ringtoneUrl = NSBundle.mainBundle().URLForResource("ringtone", withExtension: "wav")
            
            var soundId: SystemSoundID = 0
            
            AudioServicesCreateSystemSoundID(ringtoneUrl!, &soundId)
            
            ringtoneSoundId = soundId
            
            do {
                try AVAudioSession.sharedInstance().setCategory(AVAudioSessionCategoryPlayback)
                try AVAudioSession.sharedInstance().setActive(true, withOptions: .NotifyOthersOnDeactivation)
                
                playRingtone()
                
                ringtoneTimer = NSTimer.scheduledTimerWithTimeInterval(kRingtoneInterval, target: self, selector: "playRingtone", userInfo: nil, repeats: true)
                
            } catch let sessionError as NSError {
                
                print("Error playing ringtone: \(sessionError)")
            }
            
        } else {
            
            let caller = callerLabel.text ?? "BLOCKED"
            
            callNotification = UILocalNotification()
            callNotification!.soundName = "ringtoneRepeat.wav"
            callNotification!.alertAction = "Answer"
            callNotification!.alertBody = "Incoming call from \(caller)"
            
            UIApplication.sharedApplication().presentLocalNotificationNow(callNotification!)
        }
    }
    
    private func stopPlayingRingtone() {
        
        if ringtoneSoundId != nil {
        
            AudioServicesRemoveSystemSoundCompletion(ringtoneSoundId)
            
            AudioServicesDisposeSystemSoundID(ringtoneSoundId)
            
            ringtoneSoundId = nil
            
            ringtoneTimer!.invalidate()
            ringtoneTimer = nil
            
            do {
                try AVAudioSession.sharedInstance().setActive(false, withOptions: .NotifyOthersOnDeactivation)
            } catch _ {
            }
        }
        
        if callNotification != nil {
            
            UIApplication.sharedApplication().cancelLocalNotification(callNotification!)
            
            callNotification = nil
        }
    }
}

// MARK: - Action handlers

extension CallViewController {
    
    @IBAction func onToggleMute(sender: UIButton) {
        
        sender.selected = !sender.selected
        
        // FIXME: call?.setMute(sender.selected)
    }
    
    @IBAction func onToggleSpeaker(sender: UIButton) {
        
        sender.selected = !sender.selected
        
        // FIXME: SIPManager.sharedInstance.setSpeakerEnabled(sender.selected)
    }
    
    @IBAction func onHangUp(sender: AnyObject) {
        
        stopPlayingRingtone()
        
        // FIXME: call?.hangupCall()
    }

    @IBAction func onAnswer(sender: AnyObject) {
        
        stopPlayingRingtone()
        
        // FIXME: call?.answerCall(.OK)
    }
}

// MARK: - BWCallDelegate methods
/* FIXME:
extension CallViewController: BWCallDelegate {
    
    func onCallStateChanged(call: BWCall!) {
        
        print("***** Call State Changed *****")
        print("State code: \(call.lastState.rawValue)")
        print("Local URI.: \(call.localUri)")
        print("Remote URI: \(call.remoteUri)")
        
        if self.call == call {
            
            dispatch_async(dispatch_get_main_queue()) {
            
                switch call.lastState {
                
                case .Confirmed:
                    
                    UIDevice.currentDevice().proximityMonitoringEnabled = true
                    
                    self.startCallDurationTimer()
                    
                    break
                    
                case .Disconnected:
                    
                    UIDevice.currentDevice().proximityMonitoringEnabled = false
                    
                    // Called here to handle the cases where a call was terminated by the remote side before
                    // we had a chance to answer it.
                    self.stopPlayingRingtone()
                    
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
        
        print("***** Incoming DTMF *****")
        print("DTMF Received: \(digits)")
    }
}*/

// MARK: - DialpadViewControllerDelegate methods

extension CallViewController: DialpadViewControllerDelegate {
    
    func dialpad(dialpad: DialpadViewController, didStartDialingDigit digit: String) {
        
        // FIXME: tone.startDigit(digit, withVolume: kToneVolume)
    }
    
    func dialpad(dialpad: DialpadViewController, didEndDialingDigit digit: String, cancelled cancel: Bool) {
        
        // FIXME: tone.stopDigit()
        
        if !cancel {
        
            // FIXME: call?.dialDTMF(digit)
        }
    }
}
