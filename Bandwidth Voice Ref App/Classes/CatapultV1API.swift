//
//  CatapultV1API.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/6/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import Foundation

/**
    The default timeout value for requests in this API
*/
private let kDefaultTimeout: NSTimeInterval = 15.0

/**
    Enumeration describing the endpoints available in the Catapult V1 API
*/
enum CatapultV1API {
    
    /**
        Creates a new user if it does not exist and returns its SIP credentials
    */
    case CreateUser([String: AnyObject])
    
    /**
        Base URL for API access
    */
    static var baseURLString = Config.serverUrl
}

// MARK: - APIEndpoint protocol

extension CatapultV1API : APIEndpoint {
    
    var endpointDefinition: (HTTPMethod, String, [String: AnyObject]?) {
        
        switch self {
            
        case .CreateUser(let parameters):
            return (.POST, "/users", parameters)
            
        }
    }
    
    var request: NSURLRequest {
        
        let URL = NSURL(string: CatapultV1API.baseURLString)!
        
        let (method, path, parameters) = endpointDefinition
        
        let mutableURLRequest = NSMutableURLRequest(URL: URL.URLByAppendingPathComponent(path))
        mutableURLRequest.HTTPMethod = method.rawValue
        mutableURLRequest.timeoutInterval = kDefaultTimeout
                
        if parameters != nil {
            
            if let jsonData = NSJSONSerialization.dataWithJSONObject(parameters!, options: nil, error: nil) {
                
                mutableURLRequest.HTTPBody = jsonData;
                mutableURLRequest.setValue("application/json", forHTTPHeaderField: "Content-Type")
            }
        }
        
        return mutableURLRequest.copy() as NSURLRequest
    }
}

// MARK: - String extension for URL escaping

private extension String {
    
    var URLEscapedString: String? {
        
        return self.stringByAddingPercentEncodingWithAllowedCharacters(NSCharacterSet.URLHostAllowedCharacterSet())
    }
}