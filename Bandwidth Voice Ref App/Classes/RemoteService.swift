//
//  RemoteService.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/6/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import Foundation

/**
    Service class for interfacing with REST APIs
*/
class RemoteService {
    
    /**
        Enumeration describing the possible data formats for responses from the server
    */
    enum ResponseData {
        
        case Binary(NSData)
        
        case Dictionary([String: AnyObject])
        
        case Array([AnyObject])
    }
    
    /**
        Enumeration describing the possible server responses
    */
    enum Response {
        
        case Success(data: ResponseData?, httpResponse: NSHTTPURLResponse)
        
        case Failure(error: NSError, httpResponse: NSHTTPURLResponse?)
    }
    
    /**
        Shared instance through which this class must be accessed
    */
    class var sharedInstance: RemoteService {
        
        struct Singleton {
            
            static let instance = RemoteService()
        }
        
        return Singleton.instance
    }
    
    /**
        The NSURLSession object to be used for requests
    */
    private let urlSession: NSURLSession
    
    /**
        Default initializer
    */
    private init() {
        
        urlSession = NSURLSession(configuration: NSURLSessionConfiguration.defaultSessionConfiguration())
    }
    
    /**
        Performs an asynchronous request to an endpoint conforming to the APIEndpoint protocol
    
        :param: endpoint the endpoint where the request will be sent
        :param: completion callback closure to be called when the request is completed
    */
    func performRequest(endpoint: APIEndpoint, completion: (Response) -> Void) {
        
        let request = endpoint.request
        
        let task = urlSession.dataTaskWithRequest(request, completionHandler: { data, response, error in
            
            let httpResponse = response as? NSHTTPURLResponse
            
            if error != nil {
                
                completion(Response.Failure(error: error, httpResponse: httpResponse))
                
            } else {
                
                let responseData: ResponseData? = {
                    
                    if data != nil {
                        
                        if let jsonObj: AnyObject = NSJSONSerialization.JSONObjectWithData(data! as NSData, options: nil, error: nil) {
                            
                            if let jsonDic = jsonObj as? [String: AnyObject] {
                                
                                return .Dictionary(jsonDic)
                                
                            } else if let jsonArr = jsonObj as? [AnyObject] {
                                
                                return .Array(jsonArr)
                            }
                        }
                        
                        return .Binary(data! as NSData)
                        
                    } else {
                        
                        return nil
                    }
                }()
                
                completion(Response.Success(data: responseData, httpResponse: httpResponse!))
            }
        })
        
        task.resume()
    }
}
