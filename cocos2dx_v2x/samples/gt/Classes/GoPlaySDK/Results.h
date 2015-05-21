//
//  Results.h
//  GoPlaySDK
//
//
//

#ifndef __GoPlaySDK__Results__
#define __GoPlaySDK__Results__

#include "GoPlaySDkMacros.h"
#include "CommonTypes.h"
#include "Extensions.h"
#include "Constants.h"
#include <map>
#include <string>

using namespace std;

NS_GTOKEN_BEGIN

class IResult
{
public:
    SET_GET(bool, _succeeded, Succeeded, false)
    SET_GET(string, _message, Message, "")
    SET_GET(Error, _errorCode, ErrorCode, ErrorNone)
};

typedef void (CCObject::*SEL_CallFuncResult)(IResult*);
#define callfuncResult_selector(_SELECTOR) (GoPlaySDK::SEL_CallFuncResult)(&_SELECTOR)

struct ResponseHandler
{
    ResponseHandler() : target(NULL), callback(NULL)
    {
    }
    
    ResponseHandler(CCObject* target, SEL_CallFuncResult callback)
    {
        this->target = target;
        this->callback = callback;
    }
    
    SEL_CallFuncResult callback;
    CCObject* target;
};

/**
 @brief Result base class.
 */
class Result : public IResult
{
public:
    bool tryParse(WWW* www);
    
protected:
    virtual void onParse( JSONObject& json) {};
    
public:
    void copyFrom(IResult * result);
};


//Supporting Data structures - Begin
class UserProfile
{
public:
    UserProfile(){};
    SET_GET(int, _id, Id, 0)
    SET_GET(string, _userName, UserName, "")
    SET_GET(string, _nickName, NickName, "")
    SET_GET(string, _eMail, EMail, "")
    SET_GET(Gender, _gender, Gender, GenderMale)
    SET_GET(Vip, _vipStatus, VipStatus, VipNull)
    SET_GET(string, _countryCode, CountryCode, "")
    SET_GET(decimal, _goPlayToken, GoPlayToken, 0)    
    SET_GET(decimal, _freeBalance, FreeBalance, 0)
    
    static UserProfile Create(const JSONObject& json);
};
    


class GameStat {
public:
    GameStat(){}
    SET_GET(string, _title, Title, "")
    SET_GET(string, _value, Value, "")
    SET_GET(bool, _public, Public, "")
};

class GameStats : public vector<GameStat>{
public:
    
    string ToJson() const;
    
};

class Exchange {
public:
    Exchange(){}
    SET_GET(Guid, _transactionId, TransactionId, Guid())
    SET_GET(ExchangeOptionTypes, _exchangeType, ExchangeType, ExchangeOptionTypesCredit)
    SET_GET(string, _exchangeOptionId, ExchangeOptionId, "")
    SET_GET(decimal, _goPlayTokenValue, GoPlayTokenValue, 0)
    SET_GET(int, _quantity, Quantity, 0)
    SET_GET(bool, _isFree, IsFree, 0)
public:
    static Exchange Create(const JSONObject& json);
    

};

class Exchanges : public vector<Exchange>{
public:
    Exchanges(){}
    static Exchanges Create(const JSONObject& json);

};
//Supporting Data structures - End

class ProfileResult : public Result
{
public:
    SET_GET(UserProfile, _profile, Profile, UserProfile())
    
protected:
    virtual void onParse( JSONObject& json) override;
};

class RegisterResult : public ProfileResult
{
public:
    SET_GET(string, _session, Session, "")
    
protected:
    virtual void onParse( JSONObject& json) override;
};

class LoginResult : public RegisterResult
{
};

class GetProgressResult : public Result {

public:
    SET_GET(string, _data, Data, "")
    SET_GET(string, _meta, Meta, "")
protected:
    virtual void onParse( JSONObject& json) override;
};

class GetUnFullFilledExchangesResult : public Result{
public:
    SET_GET(Exchanges, _exchanges, Exchanges, Exchanges())
    
protected:
    virtual void onParse( JSONObject& json) override;
};

class FullFillExchangeResult : public Result{
public:
    SET_GET(Exchange, _exchange, Exchange, Exchange())
    
protected:
    virtual void onParse( JSONObject& json) override;
};

class RejectExchangeResult : public FullFillExchangeResult{};

class OAuthDataObject : public Result{
public:
    SET_GET(SocialPlatforms, _platform, Platform, SocialPlatformNone)
    SET_GET(string, _token, Token, "")
    
protected:
    virtual void onParse( JSONObject& json) override;
};

NS_GTOKEN_END

#endif /* defined(__GoPlaySDK__Results__) */
