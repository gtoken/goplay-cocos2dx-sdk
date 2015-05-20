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

enum class SocialPlatforms
{
    None,
    FaceBook
};

static std::map<SocialPlatforms, std::string> SocialPlatformsToString = {
    {SocialPlatforms::None, "none"},
    {SocialPlatforms::FaceBook, "facebook"}
};

enum class Gender
{
    Description("male")       Male,
    Description("female")     Female,
    Description("other")      Other
};

static std::map<Gender, std::string> GenderToString = {
    {Gender::Male, "male"},
    {Gender::Female, "female"},
    {Gender::Other, "other"}
};

enum class Vip
{
    Description("null")       Null,
    Description("standard")   Standard,
    Description("gold")       Gold
};

static std::map<Vip, std::string> VipToString = {
    {Vip::Null, "null"},
    {Vip::Standard, "standard"},
    {Vip::Gold, "gold"}
};

enum class Error
{
    Description("No Error")                       None,
    Description("INVALID_GAME_ID")                InvalidGameId,
    Description("MISSING_FIELDS")                 MissingFields,
    Description("EXISTING_EMAIL")                 EmailExist,
    Description("EXISTING_USERNAME_EMAIL")        UserNameOrEmailExist,
    Description("USERNAME_LENGTH")                UserNameLengthMustBe3To20,
    Description("INVALID_USERNAME")               InvalidUserNameCharacters,
    Description("PASSWORD_LENGTH")                PasswordLengthMustAtLeast3,
    Description("INVALID_USN_PWD")                InvalidUserNameOrPassword,
    Description("NON_EXISTING_OAUTH")             OauthUserNotExist,
    Description("INVALID_SESSION")                InvalidSession,
    Description("INVALID_TRANSACTION_ID")         InvalidTransactionId,
    Description("TRANSACTION_ALREADY_PROCESSED")  TransactionAlreadyProcessed,
    Description("NOT_SUPPORTED_OAUTH_PROVIDER")   UnSupportedOauthProvider,
    Description("EXISTING_OAUTH")                 OauthUserAlreadyExist,
    Description("OAUTH_ALREADY_CONNECTED")        OauthAlreadyConnected,
    Description("OAUTH_USER_NOT_CONNECTED")       OauthAndUserNotConnected,
    
    Description("USER_ALREADY_LOGGED_IN")         UserAlreadyLoggedIn = 1000,
    Description("USER_NOT_LOGGED_IN")             UserNotLoggedIn,
    
     Description("HTTP_REQUEST_ERROR")            HttpRequestError=2000
    
    
};

static std::map<Error, std::string> ErrorToString = {
    {Error::None, "No Error"},
    {Error::InvalidGameId, "INVALID_GAME_ID"},
    {Error::MissingFields, "MISSING_FIELDS"},
    {Error::EmailExist, "EXISTING_EMAIL"},
    {Error::UserNameOrEmailExist, "EXISTING_USERNAME_EMAIL"},
    {Error::UserNameLengthMustBe3To20, "USERNAME_LENGTH"},
    {Error::InvalidUserNameCharacters, "INVALID_USERNAME"},
    {Error::PasswordLengthMustAtLeast3, "PASSWORD_LENGTH"},
    {Error::InvalidUserNameOrPassword, "INVALID_USN_PWD"},
    {Error::OauthUserNotExist, "NON_EXISTING_OAUTH"},
    {Error::InvalidSession, "INVALID_SESSION"},
    {Error::InvalidTransactionId, "INVALID_TRANSACTION_ID"},
    {Error::TransactionAlreadyProcessed, "TRANSACTION_ALREADY_PROCESSED"},
    {Error::UnSupportedOauthProvider, "NOT_SUPPORTED_OAUTH_PROVIDER"},
    {Error::OauthUserAlreadyExist, "EXISTING_OAUTH"},
    {Error::OauthAlreadyConnected, "OAUTH_ALREADY_CONNECTED"},
    {Error::OauthAndUserNotConnected, "OAUTH_USER_NOT_CONNECTED"}
};

enum class ExchangeOptionTypes
{
    Description("CreditType")     Credit,
    Description("Package")        Package
};

static std::map<ExchangeOptionTypes, std::string> ExchangeOptionToString = {
    {ExchangeOptionTypes::Credit, "CreditType"},
    {ExchangeOptionTypes::Package, "Package"}
};


NS_GTOKEN_END

#endif /* defined(__GoPlaySDK__CommonTypes__) */
