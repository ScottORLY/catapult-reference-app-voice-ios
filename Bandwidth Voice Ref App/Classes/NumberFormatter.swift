//
//  NumberFormatter.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/8/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import Foundation

private let kFormattedPhoneMask = "(###) ###-####"

/**
    Class with utility methods for phone number formatting
*/
class NumberFormatter {
    
    // MARK: - Class methods
    
    /**
        Formats an US phone number in E164 format (i.e. from "+15552221111" to "(555) 222-1111")
        :param: number the number to format
        :returns: the formatted number
    */
    class func formatE164Number(number: String) -> String {
        
        var numberToFormat = number
        
        if let countryCodeRange = numberToFormat.rangeOfString("+1") {
            
            numberToFormat.replaceRange(countryCodeRange, with: "")
        }
        
        if countElements(numberToFormat) == 10 {
            
            if let masked = applyPhoneMaskToNumber(numberToFormat) {
                
                return masked
            }
        }
        
        return number;
    }
    
    /**
        Given a partial formatted phone number, appends or deletes a single digit while keeping the formatting
        
        :param: digit the digit to append, or "" to delete the last digit
        :param: number the partial formatted number
        :returns: the formatted number, or nil if the digit could not be applied
    */
    class func inputDigit(digit: String, inFormattedNumber number: String) -> String? {
        
        switch digit {
            
        case "":
            
            if countElements(number) > 0 {
                
                let clean = removeFormatting(number)
                
                return applyPhoneMaskToNumber(clean.substringToIndex(clean.endIndex.predecessor()))
            }
            
        case "0", "1", "2", "3", "4", "5", "6", "7", "8", "9":
            
            let clean = removeFormatting(number) + digit
            
            return applyPhoneMaskToNumber(clean)
            
        default:
            
            break
        }
        
        return nil
    }
    
    /**
        Removes all characters other than numbers from a string
        
        :param: number the number whose formatting will be removed
        :returns: the clean number string
    */
    class func removeFormatting(number: String) -> String {
        
        var cleanNumber = number
        
        while let nonDigitRange = cleanNumber.rangeOfCharacterFromSet(NSCharacterSet.decimalDigitCharacterSet().invertedSet) {
            
            cleanNumber.replaceRange(nonDigitRange, with: "")
        }
        
        return cleanNumber
    }
}

// MARK: - Private methods

private extension NumberFormatter {
    
    private class func applyPhoneMaskToNumber(number: String) -> String? {
        
        var output = ""
        
        var digitCount = countElements(number)
        
        var maskIndex = kFormattedPhoneMask.startIndex
        
        var numberIndex = number.startIndex
        
        while (digitCount > 0 && distance(maskIndex, kFormattedPhoneMask.endIndex) > 0) {
            
            let maskChar = kFormattedPhoneMask[maskIndex]
            
            switch maskChar {
                
            case "#":
                
                output.append(number[numberIndex])
                
                numberIndex = numberIndex.successor()
                
                digitCount--
                
            default:
                
                output.append(maskChar)
            }
            
            maskIndex = maskIndex.successor()
        }
        
        if digitCount == 0 {
            
            return output
        }
        
        return nil
    }
}