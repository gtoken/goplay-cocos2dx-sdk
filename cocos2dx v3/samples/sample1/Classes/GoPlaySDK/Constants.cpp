//
//  Constants.cpp
//  GoPlaySDK
//
//
//

#include "Constants.h"
#include <map>
#include <string>

using namespace std;

NS_GTOKEN_BEGIN

// Parameter Fields //
const string Constants::FIELD_USERNAME      = "username";
const string Constants::FIELD_PASSWORD      = "password";
const string Constants::FIELD_EMAIL         = "email";
const string Constants::FIELD_NICKNAME      = "nickname";
const string Constants::FIELD_GENDER        = "gender";
const string Constants::FIELD_GAME_ID       = "game_id";
const string Constants::FIELD_REFERRAL_CODE = "referral_code";
const string Constants::FIELD_SESSION       = "session";
const string Constants::FIELD_SEND_DATA     = "send_data";
const string Constants::FIELD_DATA          = "data";
const string Constants::FIELD_META          = "meta";
const string Constants::FIELD_STATS         = "stats";
const string Constants::FIELD_TRANSACTION_ID = "transaction_id";
const string Constants::FIELD_SERVICE       = "service";
const string Constants::FIELD_TOKEN         = "token";
const string Constants::FIELD_EXCHANGE_OPTION_IDENTIFIER = "exchange_option_identifier";

// Responses //
const string Constants::RESPONSE_SUCCESS    = "success";
const string Constants::RESPONSE_MESSAGE    = "message";
const string Constants::RESPONSE_ERROR_CODE = "error_code";
const string Constants::RESPONSE_SESSION    = "session";
const string Constants::RESPONSE_PROFILE    = "profile";
const string Constants::RESPONSE_DATA       = "data";
const string Constants::RESPONSE_META       = "meta";

const string Constants::RESPONSE_USER_ID                    = "uid";
const string Constants::RESPONSE_COUNTRY_CODE               = "country_code";
const string Constants::RESPONSE_GOPLAY_TOKEN               = "goplay_token";
const string Constants::RESPONSE_FREE_GOPLAY_TOKEN          = "free_goplay_token";
const string Constants::RESPONSE_GTOKEN                     = "gtoken";
const string Constants::RESPONSE_FREE_TOKEN                 = "free_gtoken";
const string Constants::RESPONSE_AVATAR                     = "avatar";
const string Constants::RESPONSE_ACCOUNT                    = "account";
const string Constants::RESPONSE_NICK_NAME                  = "nickname";
const string Constants::RESPONSE_GENDER                     = "gender";
const string Constants::RESPONSE_EMAIL                      = "email";
const string Constants::RESPONSE_JSON_TRUE                  = "true";
const string Constants::RESPONSE_VIP                        = "vip";
const string Constants::RESPONSE_EXCHANGES                  = "exchanges";
const string Constants::RESPONSE_EXCHANGE                   = "exchange";
const string Constants::RESPONSE_TRANSACTION_ID             = "transaction_id";
const string Constants::RESPONSE_EXCHANGE_OPTION_TYPE       = "exchange_option_type";
const string Constants::RESPONSE_EXCHANGE_OPTION_IDENTIFIER = "exchange_option_identifier";
const string Constants::RESPONSE_GOPLAY_TOKEN_VALUE         = "goplay_token_value";
const string Constants::RESPONSE_GTOKEN_VALUE               = "gtoken_value";
const string Constants::RESPONSE_QUANTITY                   = "quantity";
const string Constants::RESPONSE_IS_FREE                    = "is_free";


// URLs //
const string URLs::BASE_DEV_SERVER          = "https://dev.goplay.la/api/1/";
const string URLs::BASE_LIVE_SERVER         = "https://goplay.gtoken.com/api/1/";

const string URLs::ACTION_REGISTER          = "account/register";
const string URLs::ACTION_LOGIN             = "account/login-password";
const string URLs::ACTION_LOGIN_OAUTH       = "account/login-oauth";
const string URLs::ACTION_BIND_OAUTH        = "account/connect-oauth";
const string URLs::ACTION_UNBIND_OAUTH      = "account/disconnect-oauth";
const string URLs::ACTION_CHECK_OAUTH_BINDING = "account/check-oauth-connection";
const string URLs::ACTION_GET_PROFILE       = "account/profile";
const string URLs::ACTION_EDIT_PROFILE      = "account/profile-edit";
const string URLs::ACTION_GET_PROGRESS      = "game/get-progress";
const string URLs::ACTION_SAVE_PROGRESS     = "game/save-progress";
const string URLs::ACTION_UPDATE_GAME_STATS = "game/update-game-stats";
const string URLs::ACTION_FULLFILL_EXCHANGE = "transaction/fulfill-exchange";
const string URLs::ACTION_GET_UNFULLFILLED_EXCHANGE = "transaction/get-unfulfilled-exchanges";
const string URLs::ACTION_UPDATE_EXTERNAL_EXCHANGE  = "transaction/update-external-exchange";
const string URLs::ACTION_REJECT_EXCHANGE   = "transaction/reject-exchange";

NS_GTOKEN_END