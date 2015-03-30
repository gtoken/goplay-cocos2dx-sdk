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
### Table of contents
1. [API Calls](#api_calls)
  1. [Login](#login)
  2. [Register](#register)
  3. [Get Profile](#get_profile)
  4. [Edit Profile](#edit_profile)
  5. [Get Progress](#get_progress)
  6. [Save Progress](#save_progress)
  7. [Update Game Stats](#update_game_stats)
  8. [Get Unfulfilled Exchanges](#get_unfulfilled_exchanges)
  9. [Fulfill Exchange](#fulfill_exchange)
  10. [Reject Exchange](#reject_exchange)
2. [Supporting Classes](#supporting_classes)
  1. [Session](#session)
  2. [UserProfile](#userprofile)
  3. [Exchange](#exchange)
  4. [GameStat](#game_stat)

### <a name='api_calls'></a>API Calls
#### <a name='login'></a>Login
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

#### <a name='register'></a>Register
#### <a name='get_profile'></a>Get Profile
#### <a name='edit_profile'></a>Edit Profile
#### <a name='get_progress'></a>Get Progress
#### <a name='save_progress'></a>Save Progress
#### <a name='update_game_stats'></a>Update Game Stats
#### <a name='get_unfulfilled_exchanges'></a>Get Unfulfilled Exchanges
#### <a name='fulfill_exchange'></a>Fulfill Exchange
#### <a name='reject_exchange'></a>Reject Exchange
### <a name='supporting_classes'></a>Supporting Classes
#### <a name='session'></a>Session
#### <a name='userprofile'></a>UserProfile
#### <a name='exchange'></a>Exchange
#### <a name='game_stat'></a>GameStat