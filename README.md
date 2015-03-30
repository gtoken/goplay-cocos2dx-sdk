# goplay-cocos2dx-sdk
Version 1.0

## (Really) Quick-start Guide
This tutorial goes through the step-by-step instructions to integrate the Login feature to a Cocos2d-X game project.

### Requirements
Cocos2D-X 3.0 or later
### Step 1: Import plugin
Download the GoPlaySDK and copy the content in folder Classes and paste it to the Cocos2d-X project. 
### Step 2: Determine to connect to which server
GoPlay provides dev server and production server. To setup the Live server build or Test server build, configure the Boolean variable `UseLiveServer` in `GoPlaySdk.h`. Set it to `True` to use production server, `False` for dev server.
```c++
SET_GET(bool, _useLiveServer, UseLiveServer, false)
```
### Step 3: Set up GAME_ID
To start using the SDK, we need to set up the `GAME_ID` of the current project. The value helps GoPlay server identify which game connecting to it.
Open the project and search for `GoPlaySDKConfig.h`. `Game_ID` is defined at:
```c++
#ifndef __GoPlaySDK_GTConfig__
#define __GoPlaySDK_GTConfig__

#define GAME_ID "9268dcab-aeef-4cae-828f-3ae951513ffa"
#endif /* defined(__GoPlaySDK_GTConfig__) */
```
### Step 4: Call Login API and get a response
GoPlay SDK uses a funciton Callback mechanism. Each API call is a HTTP asynchronous web request. When a response callback is available, a callback function will be called. Client application can easily subscribe to these callbacks.
```c++
GoPlaySDK:: GoPlaySdk::GetInstance()->onLogin = [&](GToken::IResult * r) {
	//Your code here
} 
```
For a full listing of the source code sample, please refer to the sample code in `samples` folder.

## API References
### API Calls
#### Login
```c++
void GoPlaySdk::login(string userName, string password, Guid gameId)
void GoPlaySdk::login(SocialPlatforms platform, string token, Guid gameId);
```
This method performs a log in to GoPlay using username and password. An user account must be available before calling this method.
A callback function will be triggered when server responses.

If user logs in via facebook account, the server will bind the user with the facebook account for future auto-login.
```c++
Example:

GoPlaySDK:: GoPlaySdk::GetInstance()->onLogin = [&](GoPlaySDK::IResult * r) {
	//Your code here
}

GoPlaySDK:: GoPlaySdk::GetInstance()->login(username,password GoPlaySDK::Guid(Id)); 
```
Input
| Paramter      | Type          | Notes              |
| ------------- |---------------| -------------------|
| username      | string        | Required parameter |
| password      | string        | Required parameter |
