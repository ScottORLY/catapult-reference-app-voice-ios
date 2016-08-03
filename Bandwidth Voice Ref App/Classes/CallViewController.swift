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
        
        SIPManager.sharedManager().setCallDelegate(self)

        var number: NSString?
        
        let bwCall = SIPManager.sharedManager().getCurrentCall()
        
        let scanner = NSScanner(string: bwCall!.remoteUri)
        
        scanner.scanString("<", intoString: nil)
        scanner.scanString("sip:", intoString: nil)
        scanner.scanUpToString("@nx:", intoString: &number)
        
        callerLabel.text = number != nil ? NumberFormatter.formatE164Number(number! as String) : "BLOCKED"
        
        if bwCall!.isIncoming {
            
            callTypeLabel.text = "Incoming call from"
            
            startPlayingRingtone()
            
        } else {
            
            callTypeLabel.text = "Calling"
        }

        updateScreenState(false)
    }
    
    override func viewWillDisappear(animated: Bool) {
        super.viewWillDisappear(animated)
        
        SIPManager.sharedManager().setCallDelegate(nil)
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
        
            callDuration = NSNumber(double: -callStartDate!.timeIntervalSinceNow)
            
            let seconds = lround(callDuration!.doubleValue)
            
            let mins: Int = (seconds % 3600) / 60
            let secs: Int = seconds % 60
            
            callDurationLabel.text = NSString(format: "%02d:%02d", mins, secs) as String
        }
    }
    
    func playRingtone() {
        
        AudioServicesPlayAlertSound(ringtoneSoundId)
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
                
                SIPManager.sharedManager().setSpeakerEnabled(false)
            }
            
            let bwCall = SIPManager.sharedManager().getCurrentCall()
            if !bwCall!.isIncoming || self.callDuration != nil {
                
                self.answerButton.enabled = false
                self.hangUpButtonWidth.constant = self.view.frame.size.width / 2
                
            } else {
                
                self.answerButton.enabled = true
                self.hangUpButtonWidth.constant = 0
            }
        }
        
        if animated {
        
            UIView.animateWithDuration(0.3) {
                
                applyChanges()
                
                self.view.layoutIfNeeded()
            }
            
        } else {
            
            applyChanges()
        }
    }
    
    private func startCallDurationTimer() {
        
        callStartDate = NSDate()
        
        callDurationTimer = NSTimer.scheduledTimerWithTimeInterval(0.5, target: self, selector: #selector(CallViewController.updateCallDuration), userInfo: nil, repeats: true)
        
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
                
                ringtoneTimer = NSTimer.scheduledTimerWithTimeInterval(kRingtoneInterval, target: self, selector: #selector(CallViewController.playRingtone), userInfo: nil, repeats: true)
                
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
        
        SIPManager.sharedManager().setMute(sender.selected)
    }
    
    @IBAction func onToggleSpeaker(sender: UIButton) {
        
        sender.selected = !sender.selected
        
        SIPManager.sharedManager().setSpeakerEnabled(sender.selected)
    }
    
    @IBAction func onHangUp(sender: AnyObject) {
        
        stopPlayingRingtone()
        
        let bwCall = SIPManager.sharedManager().getCurrentCall()
        if bwCall.isIncoming && (bwCall.lastState == .IncomingRinging || bwCall.lastState == .IncomingTrying) {
            SIPManager.sharedManager().rejectIncomingCall()
        } else {
            SIPManager.sharedManager().hangupCall()
        }
    }

    @IBAction func onAnswer(sender: AnyObject) {
        
        stopPlayingRingtone()
        
        SIPManager.sharedManager().answerIncomingCall()
    }
}

// MARK: - BWCallDelegate methods

extension CallViewController: CallDelegate {
    
    func onCallStateChanged(call: BWCall!) {
        
        print("***** Call State Changed *****")
        print("State: \(BWCall.callStateToString(call.lastState))")
        print("Remote URI: \(call.remoteUri)")
        
        dispatch_async(dispatch_get_main_queue()) {
            
            switch call.lastState {
                
                case .Established:
                    
                    UIDevice.currentDevice().proximityMonitoringEnabled = true
                    
                    self.startCallDurationTimer()
                    
                    break
                
                case .Terminated, .Unauthorized, .Busy, .Error, .Unknown:
                    
                    UIDevice.currentDevice().proximityMonitoringEnabled = false
                    
                    // Called here to handle the cases where a call was terminated by the remote side before
                    // we had a chance to answer it.
                    self.stopPlayingRingtone()
                    
                    self.stopCallDurationTimer()
                    
                    self.dismissViewControllerAnimated(true, completion: nil)
                    
                    break
                
                default:
                    
                    // Nothing to do for now
                    break
            }
                
            self.updateScreenState(true)
        }
    }

}

// MARK: - DialpadViewControllerDelegate methods

extension CallViewController: DialpadViewControllerDelegate {
    
    func dialpad(dialpad: DialpadViewController, didStartDialingDigit digit: String) {
       SIPManager.sharedManager().startDigit(digit)
    }
    
    func dialpad(dialpad: DialpadViewController, didEndDialingDigit digit: String, cancelled cancel: Bool) {
        SIPManager.sharedManager().stopDigit()
    }
}
