# Bandwidth Voice Reference iOS App

This project is provided as an example on how to add voice support to an iOS application
using the Catapult platform and the Acrobits SDK. This project is a fully functioning iOS app that leverages the Acrobits SDK to register users, and make and receive calls. Acrobits provides a fully featured commercial SDK for mobile applications that wish to integrate voice capabilities and is fully compatible with Bandwidth's endpoint. 

The following features are demonstrated:

* Provisioning a new user
* Making and receiving phone calls

![App screenshot](https://github.com/bandwidthcom/catapult-reference-app-voice-ios/raw/master/screenshot.png)

## Before you start
Before you get started, there are a few things that you will need to have set up first.

- A Mac running OS X Yosemite (10.10.3) or newer
- [Xcode 7.0 or newer](https://itunes.apple.com/us/app/xcode/id497799835?mt=12)
- A device running iOS 8.0 or newer (this is optional, you can test using the iOS Simulator only)
- An account set up on the Bandwidth Application Dashboard

### Setting up your Bandwidth account
- [Need to get detailed instructions for this part]

### Setting up your application server
- [Need to get detailed instructions for this part]

## Get the code
Just grab this repository by running:

    $ git clone https://github.com/bandwidthcom/catapult-reference-app-voice-ios.git

## Open the project
- Launch Xcode
- Click File -> Open...
- Navigate to the folder where the project was cloned and open the `.xcodeproj` file

## Replace server URL
If you try to Build and Run the app at this point you will get the following build error:

```
Error: you must set the server URL in the Config.swift file
```

To fix that, you need to open the `Config.swift` file (or click the error to be taken there
directly) and replace the `<replace me>` placeholder with the URL of the server you set up 
in the steps above.

> Make sure you do not have a trailing slash in the URL

## Running on the iOS Simulator
- Select a simulated device from the target list and click the "Play" button
- After a while, the app should launch in the iOS Simulator
- When the app launches, type in any username into the field and tap Create User
- After registration completes, you can go into the Settings tab and see your assigned phone number
- You can now receive phone calls at this phone number
- You can also dial a phone number and tap Call to place a call from the Dialer tab

## Running on a physical device
You must have an active Apple Developer Program membership and have already created the necessary 
certificates and provisioned your device(s) as per [Apple's instructions](https://developer.apple.com/library/ios/documentation/IDEs/Conceptual/AppStoreDistributionTutorial/Introduction/Introduction.html#//apple_ref/doc/uid/TP40013839). Apart from that, the instructions are the same
as the simulator.

## Adding voice support to your own app

In order to include the Acrobits SDK into your project, copy the ```libsoftphone``` folder to your application and also link with its dependencies:

*  Accelerate.framework
*  AddressBook.framework
*  AddressBookUI.framework
*  AudioToolbox.framework
*  AVFoundation.framework
*  CFNetwork.framework
*  CoreGraphics.framework
*  CoreMedia.framework
*  CoreTelephony.framework
*  CoreText.framework
*  CoreVideo.framework
*  Foundation.framework
*  MediaPlayer.framework
*  PushKit.framework
*  QuartzCore.framework
*  Security.framework
*  SystemConfiguration.framework
*  UIKit.framework
*  VideoToolbox.framework
*  libz.tbd

You also need to change the following properties on the Build Settings:

 - Add `$(PROJECT_DIR)/libsoftphone` to `FRAMEWORK_SEARCH_PATHS`
 - Add `-lsqlite3`, `-framework` and `Softphone` to `OTHER_LDFLAGS`

One additional requirement is a global variable with a build number that will be used by the SDK for logs and tracking:

```
unsigned long appBuildNumber = (42);
```

You can use a `buildNumber.cpp` file to hold this variable and ideally the number should be auto-generated for each build.

After this you ca use the code provided by this sample app as the basis for your own implementation. The 
framework can be used from Objective-C or Swift (using a [bridging header](https://developer.apple.com/library/ios/documentation/Swift/Conceptual/BuildingCocoaApps/MixandMatch.html#//apple_ref/doc/uid/TP40014216-CH10-XID_77)).

If you are writing a Swift application, you cannot interact with the SDK directly (or include any headers from it) from Swift code. You will have to create an Objective-C class for the interaction and then use it from Swift code. The `SIPManager` and `SoftphoneObserverProxy` are the responsibles for this abstraction in this reference application.

More Acrobits resources:

[Documentation](https://doc.acrobits.net/cloudsoftphone/index.html)
