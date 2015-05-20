//
//  CommonTypes.h
//  GoPlaySdk
//
//
//

#ifndef __GoPlaySDK__CommonTypes__
#define __GoPlaySDK__CommonTypes__

#include "GoPlaySDkMacros.h"
#include <map>
#include <string>

using namespace std;

NS_GTOKEN_BEGIN

class Converter {
public:
    template<typename T>
    static T EnumFromDescription(std::map< T, std::string> map, std::string value){
        for (int i = 0; i < map.size(); i++) {
            if(map[(T)i] == value){
                return (T)i;
            }
        }
        return (T)0;
    }
};

// Template create map
template <typename T, typename U>
class create_map
{
private:
    std::map<T, U> m_map;
public:
    create_map(const T& key, const U& val)
    {
        m_map[key] = val;
    }
    
    create_map<T, U>& operator()(const T& key, const U& val)
    {
        m_map[key] = val;
        return *this;
    }
    
    operator std::map<T, U>()
    {
        return m_map;
    }
};

enum SocialPlatforms
{
    SocialPlatformNone,
    SocialPlatformFaceBook
};

static std::map<SocialPlatforms, std::string> SocialPlatformsToString = create_map<SocialPlatforms, std::string>
    (SocialPlatformNone, "none")
    (SocialPlatformFaceBook, "facebook");

enum Gender
{
    Description("male")       GenderMale,
    Description("female")     GenderFemale,
    Description("other")      GenderOther
};

static std::map<Gender, std::string> GenderToString = create_map<Gender, std::string>
    (GenderMale, "male")
    (GenderFemale, "female")
    (GenderOther, "other");

enum Vip
{
    Description("null")       VipNull,
    Description("standard")   VipStandard,
    Description("gold")       VipGold
};

static std::map<Vip, std::string> VipToString = create_map<Vip, std::string>
    (VipNull, "null")
    (VipStandard, "standard")
    (VipGold, "gold");

enum Error
{
    Description("No Error")                       ErrorNone,
    Description("INVALID_GAME_ID")                ErrorInvalidGameId,
    Description("MISSING_FIELDS")                 ErrorMissingFields,
    Description("EXISTING_EMAIL")                 ErrorEmailExist,
    Description("EXISTING_USERNAME_EMAIL")        ErrorUserNameOrEmailExist,
    Description("USERNAME_LENGTH")                ErrorUserNameLengthMustBe3To20,
    Description("INVALID_USERNAME")               ErrorInvalidUserNameCharacters,
    Description("PASSWORD_LENGTH")                ErrorPasswordLengthMustAtLeast3,
    Description("INVALID_USN_PWD")                ErrorInvalidUserNameOrPassword,
    Description("NON_EXISTING_OAUTH")             ErrorOauthUserNotExist,
    Description("INVALID_SESSION")                ErrorInvalidSession,
    Description("INVALID_TRANSACTION_ID")         ErrorInvalidTransactionId,
    Description("TRANSACTION_ALREADY_PROCESSED")  ErrorTransactionAlreadyProcessed,
    Description("NOT_SUPPORTED_OAUTH_PROVIDER")   ErrorUnSupportedOauthProvider,
    Description("EXISTING_OAUTH")                 ErrorOauthUserAlreadyExist,
    Description("OAUTH_ALREADY_CONNECTED")        ErrorOauthAlreadyConnected,
    Description("OAUTH_USER_NOT_CONNECTED")       ErrorOauthAndUserNotConnected,
    
    Description("USER_ALREADY_LOGGED_IN")         ErrorUserAlreadyLoggedIn = 1000,
    Description("USER_NOT_LOGGED_IN")             ErrorUserNotLoggedIn,
    
    Description("HTTP_REQUEST_ERROR")             ErrorHttpRequestError=2000
};

static std::map<Error, std::string> ErrorToString = create_map<Error, std::string>
    (ErrorNone, "No Error")
    (ErrorInvalidGameId, "INVALID_GAME_ID")
    (ErrorMissingFields, "MISSING_FIELDS")
    (ErrorEmailExist, "EXISTING_EMAIL")
    (ErrorUserNameOrEmailExist, "EXISTING_USERNAME_EMAIL")
    (ErrorUserNameLengthMustBe3To20, "USERNAME_LENGTH")
    (ErrorInvalidUserNameCharacters, "INVALID_USERNAME")
    (ErrorPasswordLengthMustAtLeast3, "PASSWORD_LENGTH")
    (ErrorInvalidUserNameOrPassword, "INVALID_USN_PWD")
    (ErrorOauthUserNotExist, "NON_EXISTING_OAUTH")
    (ErrorInvalidSession, "INVALID_SESSION")
    (ErrorInvalidTransactionId, "INVALID_TRANSACTION_ID")
    (ErrorTransactionAlreadyProcessed, "TRANSACTION_ALREADY_PROCESSED")
    (ErrorUnSupportedOauthProvider, "NOT_SUPPORTED_OAUTH_PROVIDER")
    (ErrorOauthUserAlreadyExist, "EXISTING_OAUTH")
    (ErrorOauthAlreadyConnected, "OAUTH_ALREADY_CONNECTED")
    (ErrorOauthAndUserNotConnected, "OAUTH_USER_NOT_CONNECTED");

enum ExchangeOptionTypes
{
    Description("CreditType")     ExchangeOptionTypesCredit,
    Description("Package")        ExchangeOptionTypesPackage
};

static std::map<ExchangeOptionTypes, std::string> ExchangeOptionToString = create_map<ExchangeOptionTypes, std::string>
    (ExchangeOptionTypesCredit, "CreditType")
    (ExchangeOptionTypesPackage, "Package");


NS_GTOKEN_END

#endif /* defined(__GoPlaySDK__CommonTypes__) */
