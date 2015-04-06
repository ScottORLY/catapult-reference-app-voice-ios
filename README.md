# Bandwidth Voice Reference iOS App

This project is provided as an example on how to add voice support to an iOS application
using the Catapult platform and the BWSip framework. The following features are demonstrated:

* Provisioning a new user
* Making and receiving phone calls

## System Requirements
* OS X Yosemite (10.10.2) or newer
* Xcode 6.2 or newer
* The app itself requires iOS 7.0 or newer

## Getting Started
The first step is to install Xcode from the App Store, open the ```.xcodeproj``` file and make sure
you can Build and Run using the iOS simulator. The code should compile without errors or warnings,
but a build error should still be generated:

```
Error: you must set the server URL in the Config.swift file
```

As the error message indicates, you must set up a server according to these instructions:
[TO BE DEFINED]

Once the server is running, fill in the correct URL in the ```Config.swift``` file. After this is 
done, you can Build and Run again and this time you should see the app start up on the simulator.

## Runing on a Physical Device
All the features provided by the sample app should work correctly in the iOS simulator, but you can 
also run it on an iPhone running iOS 7.0 or later. To do that, you must have an active Apple Developer 
Program membership and have already created the necessary certificates and provisioned your device(s) 
as per [Apple's instructions](https://developer.apple.com/library/ios/documentation/IDEs/Conceptual/AppStoreDistributionTutorial/Introduction/Introduction.html#//apple_ref/doc/uid/TP40013839).

## Adding Voice Support to Your App
The only file you will need to add to your app is the ```BWSip.framework``` and also link with 
its dependencies:

* libstdc++.dylib
* libresolv.dylib
* CoreLocation.framework
* SystemConfiguration.framework
* AVFoundation.framework
* AudioToolbox.framework

You can then use the code provided by this sample app as the basis for your own implementation. The 
framework can be used from Objective-C or Swift (using a [bridging header](https://developer.apple.com/library/ios/documentation/Swift/Conceptual/BuildingCocoaApps/MixandMatch.html#//apple_ref/doc/uid/TP40014216-CH10-XID_77)).

