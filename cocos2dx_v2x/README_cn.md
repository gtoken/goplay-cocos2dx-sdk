# goplay-cocos2dx-sdk
Version 1.0

## (Really) Quick-start Guide
This tutorial goes through the step-by-step instructions to integrate the Login feature to a Cocos2d-X game project.

### Requirements
Cocos2D-X 2.0 or later

### Step 1: Import plugin
Download the GoPlaySDK and copy the content in folder ``src`` and paste it to the Cocos2d-X project. 

### Step 2: Determine to connect to which server
GoPlay provides dev server and production server. To setup the Live server build or Test server build, configure the Boolean variable ``UseLiveServer`` in ``GoPlaySdk.h``. Set it to ``True`` to use production server, ``False`` for dev server.
```c++
SET_GET(bool, _useLiveServer, UseLiveServer, false)
```

### Step 3: Set up GAME_ID
To start using the SDK, first we need to set up the ``GAME_ID`` of the current project. The value helps GoPlay server identify which game connecting to it.
Open the project and search for ``GoPlaySDKConfig.h``. ``Game_ID`` is defined at:
```c++
#ifndef __GoPlaySDK_GTConfig__
#define __GoPlaySDK_GTConfig__

#define GAME_ID "9268dcab-aeef-4cae-828f-3ae951513ffa"
#endif /* defined(__GoPlaySDK_GTConfig__) */
```

### Step 4: Call Login API and get a response
GoPlay SDK uses a funciton Callback mechanism. Each API call is a HTTP asynchronous web request. When a response callback is available, a callback function will be called. Client application can easily subscribe to these callbacks.
```c++
# First, setup a callback function for the API call
void ClassName::onResponse(GoPlaySDK::IResult *result)  {
  //Your code here
}

# Then, create a response handler and hook it up with the callback
GoPlaySDK::ResponseHandler*handler=new GoPlaySDK::ResponseHandler(); 
handler->target = this;
handler->callback= callfuncResult_selector(ClassName::onResponse);
GoPlaySDK::GoPlaySdk::GetInstance()->setOnLoginHandler(handler);

# Lastly, call the API
GoPlaySDK::GoPlaySdk::GetInstance()->login(username,password); 
```

For a full listing of the source code sample, please refer to the sample code in `samples` folder.

## API References
### Table of contents
1. [API Calls](#api_calls)
  1. [Login](#login)
  2. [Register](#register)
  3. [Logout](#logout)
  4. [Get Profile](#get_profile)
  5. [Edit Profile](#edit_profile)
  6. [Get Progress](#get_progress)
  7. [Save Progress](#save_progress)
  8. [Update Game Stats](#update_game_stats)
  9. [Get Unfulfilled Exchanges](#get_unfulfilled_exchanges)
  10. [Fulfill Exchange](#fulfill_exchange)
  11. [Reject Exchange](#reject_exchange)
2. [Supporting Classes](#supporting_classes)
  1. [Session](#session)
  2. [UserProfile](#userprofile)
  3. [Exchange](#exchange)
  4. [GameStat](#game_stat)

### <a name='api_calls'></a>API Calls
#### <a name='login'></a>Login
```c++
void GoPlaySdk::login(string userName, string password)
void GoPlaySdk::login(SocialPlatforms platform, string token);
```

This method perform a log-in to GoPlay using a username and password. A user account must be available before calling this method.
A callback function will be triggered when server callbacks.
If user login via facebook account, the server will bind the user with the facebook account for future auto-login.



Example:
First, setup a callback function for the API call
```c++
void ClassName::onResponse(GoPlaySDK::IResult *result)  {
  //TODO
}
```

Then, create a response handler and hook it up with the callback
```c++
GoPlaySDK::ResponseHandler*handler=new GoPlaySDK::ResponseHandler(); 
handler->target = this;
handler->callback= callfuncResult_selector(ClassName::onResponse);
GoPlaySDK::GoPlaySdk::GetInstance()->setOnLoginHandler(handler);
```
Lastly, call the API
```c++
GoPlaySDK:: GoPlaySdk::
GetInstance()->login(username,password); 

```

##### Input:

| Paramter      | Type          | Notes              |
| ------------- |---------------| -------------------|
| username      | string        | Required parameter |
| password      | string        | Required parameter |

##### Output (LoginResult):

| Paramter      | Type          | Notes                         |
| ------------- |---------------| -------------------           |
| Succeeded     | bool          |                               |
| Message       | string        | Human-readable error message  |
| ErrorCode     | string        | Error Code                    |
| Session       | string        | Access token for later requests |
| Profile       | UserProfile   | See `UserProfile` definition in Supporting Classes section |

##### Error Messages:

* INVALID_USN_PWD - ‘Username or Password is incorrect’
* MISSING_FIELDS - ‘Required field(s) is blank’
* INVALID_GAME_ID - ‘Invalid Game ID’  

#### <a name='register'></a>Register

```c++
void GoPlaySdk::registerWith(string userName, string password, string email, string nickName, Gender gender, string referal)
```

This method is used to explicitly register a new customer account. 
A callback function will be triggered when server callbacks.
If user login via facebook account, the server will bind the user with the facebook account for future auto-login.


Example:
First, setup a callback function for the API call

```c++
   void ClassName::onResponse(GoPlaySDK::IResult *result)  {
      //TODO
    }
```
Then, create a response handler and hook it up with the callback
```c++
GoPlaySDK::ResponseHandler*handler=new GoPlaySDK::ResponseHandler(); 
handler->target = this;
handler->callback= callfuncResult_selector(ClassName::onResponse);
GoPlaySDK::GoPlaySdk::GetInstance()->setOnRegisterHandler (handler);
```
Lastly, call the API
```c++
GoPlaySDK:: GoPlaySdk::GetInstance()->registerWith( username,password,nickname,gender,referral);

```

##### Input:

| Paramter      | Type          | Notes              
| ------------- |---------------| -------------------
| username      | string        | Required parameter. Must be unique|
| password      | string        | Required parameter 
| email         | string        |  
| nickName      | string        |
| gender        | string        | `male`, `female`, or `other`
| referal       | string        | 


##### Output (RegisterResult):

| Paramter      | Type          | Notes              
| ------------- |---------------| -------------------
| Succeeded     | bool          | 
| Message       | string        | Human-readable error message
| ErrorCode     | string        | Error Code 
| Session       | string        | Access token for later requests
| Profile       | UserProfile   | See `UserProfile` definition in Supporting Classes section

##### Error Messages:

* EXISTING_USERNAME_EMAIL- ‘Account with such username/email already exists’
* MISSING_FIELDS - ‘Required field(s) is blank’
* INVALID_GAME_ID - ‘Invalid Game ID’
* USERNAME_LENGTH - ‘Username is between 3-20 characters’
* INVALID_USERNAME - ‘Username does not accept special characters’
* PASSWORD_LENGTH - ‘Password must be more than 3 characters’

#### <a name='logout'></a>Logout

```c++
void GoPlaySdk::logout()
```

Call this method to terminate current session.
Example:

```c++
  GoPlaySDK:: GoPlaySdk::logout()
```

#### <a name='get_profile'></a>Get Profile

```c++
void GoPlaySdk::getProfile()
```

Call this method to retrieve the profile of a user. 
A callback function will be triggered when server callbacks. 

Example:
First, setup a callback function for the API call
```c++
void ClassName::onResponse(GoPlaySDK::IResult *result)  {
   //TODO
}
```
Then, create a response handler and hook it up with the callback
```c++
GoPlaySDK::ResponseHandler*handler=new GoPlaySDK::ResponseHandler(); 
handler->target = this;
handler->callback= callfuncResult_selector(ClassName::onResponse);
GoPlaySDK::GoPlaySdk::GetInstance()->setOnGetProfileHandler (handler);
```
Lastly, call the API
```c++
GoPlaySDK:: GoPlaySdk::GetInstance()->getProfile();
```

##### Input:

| Paramter      | Type          | Notes              
| ------------- |---------------| -------------------
|               |               | 



##### Output (ProfileResult):

| Paramter      | Type          | Notes              
| ------------- |---------------| -------------------
| Succeeded     | bool          | 
| Message       | string        | Human-readable error message
| ErrorCode     | string        | Error Code 
| Profile       | UserProfile   | See `UserProfile` definition in Supporting Classes section

##### Error Messages:

* INVALID_SESSION - ‘Invalid Session’
* INVALID_GAME_ID - ‘Invalid Game ID’

#### <a name='edit_profile'></a>Edit Profile

```c++
void GoPlaySdk::editProfile(string email, string nickName, Gender gender)
```

Call this method to updates the profile of a user. Parameters may be omitted, and those fields will be unchanged.
A callback function will be triggered when server callbacks. 
Example:
First, setup a callback function for the API call

```c++
void ClassName::onResponse(GoPlaySDK::IResult *result)  {
  //TODO
}
```
Then, create a response handler and hook it up with the callback
```c++
GoPlaySDK::ResponseHandler*handler=new GoPlaySDK::ResponseHandler(); 
handler->target = this;
handler->callback= callfuncResult_selector(ClassName::onResponse);
GoPlaySDK::GoPlaySdk::GetInstance()->setOnEditProfile (handler);
```

Lastly, call the API
```c++
GoPlaySDK::GoPlaySdk::GetInstance()->editProfile(email,nickname,gender); 
```

##### Input:

| Paramter      | Type          | Notes              
| ------------- |---------------| -------------------
| email         | string        |  Must be unique
| nickName      | string        |
| gender        | string        | `male`, `female`, or `other`

##### Output (EditProfileResult):

| Paramter      | Type          | Notes              
| ------------- |---------------| -------------------
| Succeeded     | bool          | 
| Message       | string        | Human-readable error message
| ErrorCode     | string        | Error Code 
| Profile       | UserProfile   | See `UserProfile` definition in Supporting Classes section

##### Error Messages:

* INVALID_SESSION - ‘Invalid Session’
* INVALID_GAME_ID - ‘Invalid Game ID’

#### <a name='get_progress'></a>Get Progress

```c++
void GoPlaySdk::getProgress(bool sendData)
```

Call this method to retrieve game progress directly from GoPlay server. The progress is saved in a string field, either as xml or json. The progress is saved together with a meta field. 

A callback function will be triggered when server callbacks. 
Example:
First, setup a callback function for the API call


```c++
void ClassName::onResponse(GoPlaySDK::IResult *result)  {
  //TODO
}
```
Then, create a response handler and hook it up with the callback
```c++
GoPlaySDK::ResponseHandler*handler=new GoPlaySDK::ResponseHandler(); 
handler->target = this;
handler->callback= callfuncResult_selector(ClassName::onResponse);
GoPlaySDK::GoPlaySdk::GetInstance()->setOnGetProgress (handler);
```
Lastly, call the API
```c++
GoPlaySDK::GoPlaySdk::GetInstance()->getProgress(true); 
```

##### Input:

| Paramter      | Type          | Notes              
| ------------- |---------------| -------------------
| sendData      | bool          | *True* or *False*. Optional, default *True*. Choose if the response should include the game save data, decrease traffic on server and client when only meta data and *save_at* timestamp are needed.

##### Output (GetProgressResult):

| Paramter      | Type          | Notes              
| ------------- |---------------| -------------------
| Succeeded     | bool          | 
| Message       | string        | Human-readable error message
| ErrorCode     | string        | Error Code 
| Data          | string        | 
| Meta          | string        | 

##### Error Messages:

* MISSING_FIELDS - ‘Required field(s) is blank’
* INVALID_SESSION - ‘Invalid Session’
* INVALID_GAME_ID - ‘Invalid Game ID’

#### <a name='save_progress'></a>Save Progress

```c++
void GoPlaySdk::saveProgress(string data,string meta)
```

Call this method to save game progress to GoPlay server. 

A callback function will be triggered when server callbacks. 
Example:
First, setup a callback function for the API call
```c++
void ClassName::onResponse(GoPlaySDK::IResult *result)  {
  //TODO
}
```
Then, create a response handler and hook it up with the callback
```c++
GoPlaySDK::ResponseHandler*handler=new GoPlaySDK::ResponseHandler(); 
handler->target = this;
handler->callback= callfuncResult_selector(ClassName::onResponse);
GoPlaySDK::GoPlaySdk::GetInstance()->setOnSaveProgress(handler);
```
Lastly, call the API
```c++
GoPlaySDK::GoPlaySdk::GetInstance()->saveProgress(data,meta); 
```

##### Input:

| Paramter      | Type          | Notes              
| ------------- |---------------| -------------------
| data          | string        | Data to be sent to server
| meta          | string        |

##### Output (GetProgressResult):

| Paramter      | Type          | Notes              
| ------------- |---------------| -------------------
| Succeeded     | bool          | 
| Message       | string        | Human-readable error message
| ErrorCode     | string        | Error Code 

##### Error Messages:

* MISSING_FIELDS - ‘Required field(s) is blank’
* INVALID_SESSION - ‘Invalid Session’
* INVALID_GAME_ID - ‘Invalid Game ID’

#### <a name='update_game_stats'></a>Update Game Stats

```c++
void GoPlaySdk::updateGameStats(GameStats stats)
```

Call this method in game client to save game stats directly to GoPlay server. The stat is saved as a string.
A callback function will be triggered when server callbacks. 
Example:
First, setup a callback function for the API call
```c++
void ClassName::onResponse(GoPlaySDK::IResult *result)  {
  //TODO
}
```
Then, create a response handler and hook it up with the callback
```c++
GoPlaySDK::ResponseHandler*handler=new GoPlaySDK::ResponseHandler(); 
handler->target = this;
handler->callback= callfuncResult_selector(ClassName::onResponse);
GoPlaySDK::GoPlaySdk::GetInstance()->setOnUpdateGameStats (handler);
```
Lastly, call the API
```c++
GoPlaySDK::GoPlaySdk::GetInstance()->updateGameStats(stats);
```
##### Input:

| Paramter      | Type          | Notes              
| ------------- |---------------| -------------------
| stats         | array         | Required Parameter.Array of **Game Stat**. See **Game Stat** definition in Supporting Classes section

##### Output (UpdateGameStatsResult):

| Paramter      | Type          | Notes              
| ------------- |---------------| -------------------
| Succeeded     | bool          | 
| Message       | string        | Human-readable error message
| ErrorCode     | string        | Error Code 

##### Error Messages:

* MISSING_FIELDS - ‘Required field(s) is blank’
* INVALID_SESSION - ‘Invalid Session’
* INVALID_GAME_ID - ‘Invalid Game ID’
* INVALID_GAME_STAT - ‘Invalid stat format. A stat JSON must include 3 keys title, value and public with their values.’

#### <a name='get_unfulfilled_exchanges'></a>Get Unfulfilled Exchanges

```c++
void GoPlaySdk::getUnFullFilledExchanges()
```

Returns a list of unfulfilled exchanges made on GoPlay website
A callback function will be triggered when server callbacks. 
Example:
First, setup a callback function for the API call
```c++
void ClassName::onResponse(GoPlaySDK::IResult *result)  {
  //TODO
}
```
Then, create a response handler and hook it up with the callback
```c++
GoPlaySDK::ResponseHandler*handler=new GoPlaySDK::ResponseHandler(); 
handler->target = this;
handler->callback= callfuncResult_selector(ClassName::onResponse);
GoPlaySDK::GoPlaySdk::GetInstance()->setOnGetUnFullFilledExchanges (handler);
```
Lastly, call the API
```c++
GoPlaySDK::GoPlaySdk::GetInstance()->getUnFullFilledExchanges (); 
```
##### Input:

| Paramter      | Type          | Notes              
| ------------- |---------------| -------------------
|               |               | 



##### Output (GetUnFullFillExchangesResult):

| Paramter      | Type          | Notes              
| ------------- |---------------| -------------------
| Succeeded     | bool          | 
| Message       | string        | Human-readable error message
| Exchanges     | array         | Array of **Exchange**. See **Exchange** definition in Supporting Classes section

##### Error Messages:

* INVALID_GAME_ID - ‘Invalid Game ID’
* INVALID_SESSION - ‘Invalid Session’

#### <a name='fulfill_exchange'></a>Fulfill Exchange

```c++
void GoPlaySdk::fullFillExchange(Guid transactionId)
```

Call this method to fulfill an exchange made on GoPlay website.
A callback function will be triggered when server callbacks. 
Example:
First, setup a callback function for the API call
```c++
void ClassName::onResponse(GoPlaySDK::IResult *result)  {
  //TODO
}
```
Then, create a response handler and hook it up with the callback
```c++
GoPlaySDK::ResponseHandler*handler=new GoPlaySDK::ResponseHandler(); 
handler->target = this;
handler->callback= callfuncResult_selector(ClassName::onResponse);
GoPlaySDK::GoPlaySdk::GetInstance()->setOnFullFillExchange (handler);
```
Lastly, call the API
```c++
GoPlaySDK::GoPlaySdk::GetInstance()->fullFillExchange(transactionId);
```
##### Input:

| Paramter      | Type          | Notes              
| ------------- |---------------| -------------------
| transactionId | uuid          | Required Parameter.

##### Output (FullFillExchangeResult):

| Paramter      | Type          | Notes              
| ------------- |---------------| -------------------
| Succeeded     | bool          | 
| Message       | string        | Human-readable error message
| ErrorCode     | string        | Error Code 
| Exchanges     | Exchange      | The detail of the fulfilled exchange, for double checking. See **Exchange** definition in Supporting Classes section

##### Error Messages:

* INVALID_GAME_ID - ‘Invalid Game ID’
* INVALID_SESSION - ‘Invalid Session’
* INVALID_TRANSACTION_ID - ‘Invalid Transaction ID’
* TRANSACTION_ALREADY_PROCESSED - ‘Transaction has already been processed’

#### <a name='reject_exchange'></a>Reject Exchange

```c++
void GoPlaySdk::rejectExchange(string transactionId)
```

Reject an exchange made on GoPlay website. The rejected exchange’s status will be changed to failure and the user’s balance is redeemed.

Example:
First, setup a callback function for the API call
```c++
void ClassName::onResponse(GoPlaySDK::IResult *result)  {
  //TODO
}
```
Then, create a response handler and hook it up with the callback
```c++
GoPlaySDK::ResponseHandler*handler=new GoPlaySDK::ResponseHandler(); 
handler->target = this;
handler->callback= callfuncResult_selector(ClassName::onResponse);
GoPlaySDK::GoPlaySdk::GetInstance()->setOnRejectExchange (handler);
```
Lastly, call the API
```c++
GoPlaySDK::GoPlaySdk::GetInstance()->rejectExchange (transactionId); 
```
##### Input:

| Paramter      | Type          | Notes              
| ------------- |---------------| -------------------
| transactionId | uuid          | Required Parameter.

##### Output (RejectExchangeResult):

| Paramter      | Type          | Notes              
| ------------- |---------------| -------------------
| Succeeded     | bool          | 
| Message       | string        | Human-readable error message
| ErrorCode     | string        | Error Code 
| Exchange      | Exchange      | The detail of the rejected exchange, for double checking. See **Exchange** definition in Supporting Classes section

##### Error Messages:

* INVALID_GAME_ID - ‘Invalid Game ID’
* INVALID_SESSION - ‘Invalid Session’
* INVALID_TRANSACTION_ID - ‘Invalid Transaction ID’
* TRANSACTION_ALREADY_PROCESSED - ‘Transaction has already been processed’

### <a name='supporting_classes'></a>Supporting Classes
#### <a name='session'></a>Session

Contains all information regarding the player’s session, such as session ID, game ID and current user.

| Paramter      | Type          | Notes              
| ------------- |---------------| -------------------
| gameId        | GTGameID      | Hold GameID class
| GameId        | string        | GameID guid type
| HasLoggedIn   | boolean       | Whether the user has login before 
| CurrentUser   | UserProfile   | The current user
| SessionId     | string        | Id of current session
| GOPLAY_Session| string        | store cache key for session

#### <a name='userprofile'></a>UserProfile

All the information about user such as username, password, Gender, etc.

| Paramter      | Type          | Notes              
| ------------- |---------------| -------------------
| Id            | int           | 
| UserName      | string        | 
| NickName      | string        | 
| Email         | string        | 
| Gender        | Gender        | 
| VipStatus     | Vip           | 
| CountryCode   | string        | 
| GoPlayToken   | decimal       | 
| FreeBalance   | decimal       | 



#### <a name='exchange'></a>Exchange

This class holds information about user’s transactions.

| Paramter          | Type                  | Notes              
| ----------------- |---------------------  | -------------------
| TransactionId     | Guid                  | 
| ExchangeType      | ExchangeOptionTypes   | 
| ExchangeOptionId  | string                | 
| Quantity          | int                   | 
| IsFree            | bool                  | 
| GoPlayTokenValue  | Decimal               | 

#### <a name='game_stat'></a>GameStat

A JSON object representing a game stat, with the following keys:.

| Paramter      | Type          | Notes              
| ------------- |---------------| -------------------
| Title         | string        | 
| Value         | string        | 
| Public        | bool          | 
