//
//  APIEndpoint.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/6/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import Foundation

/**
    Protocol that defines an API endpoint.
*/
protocol APIEndpoint {
    
    /**
        Tuple containing the following elements that define an endpoint:
        - HTTP method to be used
        - Path
        - Request parameters (can be nil if none are necessary)
    */
    var endpointDefinition: (HTTPMethod, String, [String: AnyObject]?) { get }
    
    /**
        The request object to call an endpoint
    */
    var request: NSURLRequest { get }
}
