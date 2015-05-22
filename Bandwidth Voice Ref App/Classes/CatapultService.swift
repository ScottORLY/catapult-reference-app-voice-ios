//
//  CatapultService.swift
//  Bandwidth Voice Ref App
//
//  Created by Fabio Rodella on 4/6/15.
//  Copyright (c) 2015 Bandwidth. All rights reserved.
//

import Foundation

/**
    Boxing class, necessary for now due to a limitation in the Swift compiler with regard
    to generic enums
*/
final class Box<T> {
    
    let unbox: T
    
    init(_ value: T) { self.unbox = value }
}

/**
    Enumeration describing possible results for Catapult API calls
*/
enum CatapultServiceResponse<T> {
    
    case Success(Box<T>)
    
    case Failure(NSError)
}

/**
    Service class for interfacing with the Catapult REST API
*/
class CatapultService {
    
    /**
        Describes the error domain for the CatapultService class
    */
    struct ErrorDomain {
        
        static let name = "com.bandwidth.CatapultService.ErrorDomain"
        
        enum Code: Int {
            
            case EmptyResponse = 1
            
            case UnexpectedResponseData
            
            case UnmarshallingError
        }
    }
    
    /**
        Shared instance through which this class must be accessed
    */
    static let sharedInstance = CatapultService()
    
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
        Creates a new user and returns its info
        
        :param: name the username for the new user
        :param: password the user's password
        :param: completion callback closure to be called when the request completes
    */
    func createOrFetchUser(name: String, password: String, completion: (CatapultServiceResponse<User>) -> Void) {
        
        let endpoint = CatapultV1API.CreateOrFetchUser(["userName": name, "password": password])
        
        performRequest(endpoint) { responseData, error, httpResponse in
            
            var response: CatapultServiceResponse<User>?
            
            if error != nil {
                
                response = CatapultServiceResponse.Failure(error!)
                
            } else {
                
                if let actualResponseData = responseData {
                    
                    switch actualResponseData {
                        
                    case .Dictionary(let dictionary):
                        
                        if var user = User.fromJSON(dictionary) {
                            
                            user.password = password
                            
                            response = CatapultServiceResponse.Success(Box(user))
                            
                        } else {
                            
                            let error = NSError(domain: ErrorDomain.name, code: ErrorDomain.Code.UnmarshallingError.rawValue, userInfo: [NSLocalizedDescriptionKey: "Could not create objects from JSON data: \(dictionary)"])
                            
                            response = CatapultServiceResponse.Failure(error)
                        }
                        
                    default:
                        
                        let error = NSError(domain: ErrorDomain.name, code: ErrorDomain.Code.UnexpectedResponseData.rawValue, userInfo: [NSLocalizedDescriptionKey: "Response data was not a JSON object"])
                        
                        response = CatapultServiceResponse.Failure(error)
                    }
                    
                } else {
                 
                    let error = NSError(domain: ErrorDomain.name, code: ErrorDomain.Code.UnexpectedResponseData.rawValue, userInfo: [NSLocalizedDescriptionKey: "Response was empty"])
                    
                    response = CatapultServiceResponse.Failure(error)
                }
            }
            
            completion(response!)
        }
    }
}

// MARK: - HTTP Request methods

private extension CatapultService {
    
    /**
        Enumeration describing the possible data formats for responses from the server
    */
    private enum ResponseData {
        
        case Binary(NSData)
        
        case Dictionary([String: AnyObject])
        
        case Array([AnyObject])
    }
    
    /**
        Performs an asynchronous request to an endpoint conforming to the APIEndpoint protocol
    
        :param: endpoint the endpoint where the request will be sent
        :param: completion callback closure to be called when the request is completed
    */
    private func performRequest(endpoint: APIEndpoint, completion: (ResponseData?, NSError?, NSHTTPURLResponse?) -> Void) {
        
        let request = endpoint.request
        
        let task = urlSession.dataTaskWithRequest(request, completionHandler: { data, response, error in
            
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
            
            
            // Make sure the response is handled in the main thread
            
            dispatch_async(dispatch_get_main_queue()) {
                
                completion(responseData, error, response as? NSHTTPURLResponse)
            }
        })
        
        task.resume()
    }
}
