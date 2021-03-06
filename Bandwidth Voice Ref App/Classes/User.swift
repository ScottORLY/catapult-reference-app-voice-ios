//
//  User.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/6/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import Foundation

/**
    Class describing an user
*/
@objc class User: NSObject {
    let username: String
    
    var password: String?
    
    let number: String
    
    let endpoint: Endpoint
    
    private init(username: String, password: String?, number: String, endpoint: Endpoint) {
        self.username = username;
        self.password = password;
        self.number = number;
        self.endpoint = endpoint;
    }
   
    static func fromJSON(json: [String: AnyObject]) -> User? {
        
        let username        = json["userName"] as? String
        let password        = json["password"] as? String
        let number          = json["phoneNumber"] as? String
        let endpointJson    = json["endpoint"] as? [String: AnyObject]
        
        if (username != nil &&
            number != nil &&
            endpointJson != nil) {
            
            if let endpoint = Endpoint.fromJSON(endpointJson!) {
                    
                return User(username: username!, password: password, number: number!, endpoint: endpoint)
            }
        }
        
        return nil
    }
    
    func toJSON() -> [String: AnyObject] {
        
        return ["userName"      : username,
                "password"      : password ?? NSNull(),
                "phoneNumber"   : number,
                "endpoint"      : endpoint.toJSON()]
    }
}
