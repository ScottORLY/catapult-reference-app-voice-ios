//
//  HTTPMethod.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/6/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import Foundation

/**
    Enum describing the possible HTTP methods
*/
enum HTTPMethod: String {
    
    case OPTIONS = "OPTIONS"
    
    case GET = "GET"
    
    case HEAD = "HEAD"
    
    case POST = "POST"
    
    case PUT = "PUT"
    
    case PATCH = "PATCH"
    
    case DELETE = "DELETE"
    
    case TRACE = "TRACE"
    
    case CONNECT = "CONNECT"
}
