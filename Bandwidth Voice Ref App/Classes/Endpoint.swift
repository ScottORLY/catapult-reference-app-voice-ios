//
//  Endpoint.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/6/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import Foundation

/**
    Structure describing an endpoint
*/
struct Endpoint {
    
    let endpointId: String
    
    let domainId: String
    
    let applicationId: String
    
    let enabled: Bool
    
    let name: String
        
    let sipUri: String
        
    let credentials: Credentials
    
    static func fromJSON(json: [String: AnyObject]) -> Endpoint? {
        
        let endpointId      = json["id"] as? String
        let domainId        = json["domainId"] as? String
        let applicationId   = json["applicationId"] as? String
        let enabled         = json["enabled"] as? Bool
        let name            = json["name"] as? String
        let sipUri          = json["sipUri"] as? String
        let credentialsJson = json["credentials"] as? [String: AnyObject]
        
        if (endpointId != nil &&
            domainId != nil &&
            applicationId != nil &&
            enabled != nil &&
            name != nil &&
            sipUri != nil &&
            domainId != nil &&
            credentialsJson != nil) {
                
            if let credentials = Credentials.fromJSON(credentialsJson!) {
                    
                return Endpoint(endpointId: endpointId!, domainId: domainId!, applicationId: applicationId!, enabled: enabled!, name: name!, sipUri: sipUri!, credentials: credentials)
            }
        }
        
        return nil
    }
    
    func toJSON() -> [String: AnyObject] {
        
        return ["id"            : endpointId,
                "domainId"      : domainId,
                "applicationId" : applicationId,
                "enabled"       : enabled,
                "name"          : name,
                "sipUri"        : sipUri,
                "credentials"   : credentials.toJSON()]
    }
}
