//
//  DialpadViewController.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/7/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import UIKit

/**
    Protocol for receiving dialpad events
*/
protocol DialpadViewControllerDelegate: class {
    
    /**
        Called when a button on the dialpad is pressed

        :param: dialpad the source dialpad view controller
        :param: digit the dialed digit
    */
    func dialpad(dialpad: DialpadViewController, didDialDigit digit: String)
}

/**
    View controller class that implements a phone's dialpad
*/
class DialpadViewController: UIViewController {

    // MARK: - Outlets
    
    @IBOutlet private var digitButtons: [UIButton]!
    
    // MARK: - Public properties
    
    weak var delegate: DialpadViewControllerDelegate?
    
    // MARK: - Superclass methods
    
    override func viewDidLoad() {
        
        super.viewDidLoad()
        
        let buttonLabels: [String] = [
            "1\n ",
            "2\nABC",
            "3\nDEF",
            "4\nGHI",
            "5\nJKL",
            "6\nMNO",
            "7\nPQRS",
            "8\nTUV",
            "9\nWXYZ",
            "\u{2217}", // Bigger asterisk
            "0\n+",
            "#"
        ]
        
        for button in digitButtons {
            
            button.titleLabel!.lineBreakMode = .ByWordWrapping
            button.titleLabel!.textAlignment = .Center

            let title = buttonLabels[button.tag - 1]
            
            // Configures the different parts of the text for each button
            
            if let newLineRange = title.rangeOfCharacterFromSet(NSCharacterSet.newlineCharacterSet()) {
                
                let middle = distance(title.startIndex, newLineRange.startIndex)
                let end = distance(newLineRange.startIndex, title.endIndex)
                
                let attTitleNormal = NSMutableAttributedString(string: title)
                attTitleNormal.setAttributes([NSFontAttributeName: UIFont.boldSystemFontOfSize(20)], range: NSMakeRange(0, middle))
                attTitleNormal.setAttributes([NSFontAttributeName: UIFont.systemFontOfSize(12), NSForegroundColorAttributeName: UIColor.darkGrayColor()], range: NSMakeRange(middle, end))
                
                button.setAttributedTitle(attTitleNormal, forState: .Normal)
                
            } else {
                
                let end = distance(title.startIndex, title.endIndex)
                
                let attTitleNormal = NSMutableAttributedString(string: title)
                attTitleNormal.setAttributes([NSFontAttributeName: UIFont.boldSystemFontOfSize(26)], range: NSMakeRange(0, end))
                
                button.setAttributedTitle(attTitleNormal, forState: .Normal)
            }
        }
    }
    
    override func viewDidLayoutSubviews() {
        
        super.viewDidLayoutSubviews()
        
        for button in digitButtons {
            
            // We force the layout for the buttons so that their correct size is known
            button.setNeedsLayout()
            button.layoutIfNeeded()
            
            // Configure rounded corners and border
            button.layer.cornerRadius = button.frame.size.width / 2
            button.layer.borderColor = button.tintColor!.CGColor
            button.layer.borderWidth = 1.0
            button.layer.masksToBounds = true
        }
    }
}

// MARK: - Action handlers

private extension DialpadViewController {
    
    @IBAction func onDigit(sender: UIButton) {
        
        var digit: String?
        
        switch sender.tag {
            
        case 1 ... 9:
            digit = sender.tag.description
            
        case 10:
            digit = "*"
            
        case 11:
            digit = "0"
            
        case 12:
            digit = "#"
            
        default:
            digit = nil
        }
        
        if digit != nil {
            
            delegate?.dialpad(self, didDialDigit: digit!)
        }
    }
}

