//
//  Constants.h
//  GoPlaySDK
//
//
//

#ifndef __GoPlaySDK__Constants__
#define __GoPlaySDK__Constants__

#include "GoPlaySDkMacros.h"
#include <map>
#include <string>

using namespace std;

NS_GTOKEN_BEGIN

class Constants{
public:
    // Parameter Fields //
    const static string FIELD_USERNAME;
    const static string FIELD_PASSWORD;
    const static string FIELD_EMAIL;
    const static string FIELD_NICKNAME;
    const static string FIELD_GENDER;
    const static string FIELD_GAME_ID;
    const static string FIELD_REFERRAL_CODE;
    const static string FIELD_SESSION;
    const static string FIELD_SEND_DATA;
    const static string FIELD_DATA;
    const static string FIELD_META;
    const static string FIELD_STATS;
    const static string FIELD_TRANSACTION_ID;
    const static string FIELD_SERVICE;
    const static string FIELD_TOKEN;
    const static string FIELD_EXCHANGE_OPTION_IDENTIFIER;
    
    
    // Responses //
    const static string RESPONSE_SUCCESS;
    const static string RESPONSE_MESSAGE;
    const static string RESPONSE_ERROR_CODE;
    const static string RESPONSE_SESSION;
    const static string RESPONSE_PROFILE;
    const static string RESPONSE_DATA;
    const static string RESPONSE_META;
    const static string RESPONSE_USER_ID;
    const static string RESPONSE_COUNTRY_CODE;
    const static string RESPONSE_GOPLAY_TOKEN;
    const static string RESPONSE_FREE_GOPLAY_TOKEN;
    const static string RESPONSE_GTOKEN;
    const static string RESPONSE_FREE_TOKEN;
    const static string RESPONSE_AVATAR;
    const static string RESPONSE_ACCOUNT;
    const static string RESPONSE_NICK_NAME;
    const static string RESPONSE_GENDER;
    const static string RESPONSE_EMAIL;
    const static string RESPONSE_JSON_TRUE;
    const static string RESPONSE_VIP;
    const static string RESPONSE_EXCHANGE;
    const static string RESPONSE_EXCHANGES;
    const static string RESPONSE_TRANSACTION_ID;
    const static string RESPONSE_EXCHANGE_OPTION_TYPE;
    const static string RESPONSE_EXCHANGE_OPTION_IDENTIFIER;
    const static string RESPONSE_GOPLAY_TOKEN_VALUE;
    const static string RESPONSE_GTOKEN_VALUE;
    const static string RESPONSE_QUANTITY;
    const static string RESPONSE_IS_FREE;
};

class URLs{
public:
    const static string BASE_DEV_SERVER;
    const static string BASE_LIVE_SERVER;
    
    const static string ACTION_REGISTER;
    const static string ACTION_LOGIN;
    const static string ACTION_LOGIN_OAUTH;
    const static string ACTION_BIND_OAUTH;
    const static string ACTION_UNBIND_OAUTH;
    const static string ACTION_CHECK_OAUTH_BINDING;
    const static string ACTION_GET_PROFILE;
    const static string ACTION_EDIT_PROFILE;
    const static string ACTION_GET_PROGRESS;
    const static string ACTION_SAVE_PROGRESS;
    const static string ACTION_UPDATE_GAME_STATS;
    const static string ACTION_FULLFILL_EXCHANGE;
    const static string ACTION_GET_UNFULLFILLED_EXCHANGE;
    const static string ACTION_UPDATE_EXTERNAL_EXCHANGE;
    const static string ACTION_REJECT_EXCHANGE;

};

NS_GTOKEN_END

#endif /* defined(__GoPlaySDK_Constants__) */
