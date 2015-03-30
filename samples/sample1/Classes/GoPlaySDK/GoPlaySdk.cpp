//
//  GoPlaySdk.cpp
//  GoPlaySDK
//
//
//

#include "GoPlaySdk.h"
#include "CommonTypes.h"

NS_GTOKEN_BEGIN

// Supported Operations //

void GoPlaySdk::registerWith(string userName,
                         string password,
                         Guid gameId,
                         string email,
                         string nickName,
                         Gender gender,
                         string referal)
{
    if (onRegister != nullptr)
    {
        registerAsync(userName, password, gameId, email, nickName, gender, referal);
    }
}



void GoPlaySdk::registerAsync(string userName,
                          string password,
                          Guid gameId,
                          string email,
                          string nickName,
                          Gender gender,
                          string referal)
{
    WWWForm f = WWWForm();
    f.addFieldLowerCase(Constants::FIELD_USERNAME, userName);
    f.addFieldLowerCase(Constants::FIELD_PASSWORD, password);
    f.addField(Constants::FIELD_GAME_ID, gameId.ToString());
    f.addField(Constants::FIELD_GENDER, GenderToString[gender]);
    
    f.addFieldIfValid(Constants::FIELD_EMAIL, email);
    f.addFieldIfValid(Constants::FIELD_NICKNAME, nickName);
    f.addFieldIfValid(Constants::FIELD_REFERRAL_CODE, referal);
    
    string url = UrlExtension::ToURL(URLs::ACTION_REGISTER, _useLiveServer);
    WWW::CREATE( url, f, [&, gameId](WWW* www){
        // Compose the Result //
        RegisterResult * result = new RegisterResult();
        result->tryParse(www);
        
        if (result->getSucceeded()) _session = result->getSession();
        
        // Need to connect this user with Social platform ? //
        if (result->getSucceeded() && _requireOAuthData != nullptr)
        {   
            bindOauthAsync(gameId, _requireOAuthData, [&](IResult * r)
            {
                if (onRegister)
                {
                    onRegister(r);
                }

            });
            // YN: TO add this after testing and at RTM stage // SAFE_DELETE(_requireOAuthData);
        }else{
            // Trigger Event //
            if (onRegister) {
                onRegister(result);
            }
        }
        delete result;
    });
}


void GoPlaySdk::login(string userName, string password, Guid gameId)
{
    if (onLogin)
    {
        loginAsync(userName, password, gameId);
    }
}


void GoPlaySdk::loginAsync(string userName, string password, Guid gameId){
    WWWForm f = WWWForm();
    f.addFieldLowerCase(Constants::FIELD_USERNAME, userName);
    f.addFieldLowerCase(Constants::FIELD_PASSWORD, password);
    f.addField(Constants::FIELD_GAME_ID, gameId.ToString());
    string url = UrlExtension::ToURL(URLs::ACTION_LOGIN, _useLiveServer);
    
    WWW::CREATE(url, f, [&, gameId](WWW* www){
        // Compose the Result //
        LoginResult * result = new LoginResult();
        result->tryParse(www);
        
        if (result->getSucceeded()) _session = result->getSession();
        
        // Need to connect this user with Social platform ? //
        if (result->getSucceeded() && _requireOAuthData)
        {
            // Perform Oauth-Connect //
            bindOauthAsync(gameId, _requireOAuthData, [&](IResult * r){
                if (onLogin) {
                    onLogin(r);
                }

            });
            // YN: TO add this after testing and at RTM stage // SAFE_DELETE(_requireOAuthData);
        }else{
            // Trigger Event //
            onLogin(result);
        }
        
        delete result;
    });
}

//Login with third party such as FaceBook

void GoPlaySdk::login(SocialPlatforms platform, string token, Guid gameId)
{
    if (onLogin)
    {
        loginAsync(platform, token, gameId);
    }
}

void GoPlaySdk::loginAsync(SocialPlatforms platform, string token, Guid gameId)
{
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SERVICE, SocialPlatformsToString[platform]);
    f.addField(Constants::FIELD_TOKEN, token);
    f.addField(Constants::FIELD_GAME_ID, gameId.ToString());
    
    string url = UrlExtension::ToURL(URLs::ACTION_LOGIN_OAUTH, _useLiveServer);
    WWW::CREATE( url, f, [&, platform, token](WWW* www){
        
        // Compose the Result //
        LoginResult * result = new LoginResult();
        result->tryParse(www);
        
   
        if (result->getSucceeded())
        {
            _session = result->getSession();
            
            SAFE_DELETE(_requireOAuthData);
            _requireOAuthData = new OAuthDataObject();
            _requireOAuthData->setPlatform(platform);
            _requireOAuthData->setToken(token);
        }
        else
        {
            // UI to display the Login/Register screen //
            if (result->getErrorCode() == Error::OauthUserNotExist)
            {
                SAFE_DELETE(_requireOAuthData);
                _requireOAuthData = new OAuthDataObject();
                _requireOAuthData->setPlatform(platform);
                _requireOAuthData->setToken(token);
            }
        }
        
        // Trigger Event //
        onLogin(result);
        
        delete result;
    });
}


void GoPlaySdk::bindOauthAsync(Guid gameId, OAuthDataObject * oData, ResponseHandler callBack)
{
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, getSession());
    f.addField(Constants::FIELD_GAME_ID, gameId.ToString());
    f.addField(Constants::FIELD_SERVICE, SocialPlatformsToString[oData->getPlatform()]);
    f.addFieldIfValid(Constants::FIELD_TOKEN, oData->getToken());
    
    string url = UrlExtension::ToURL(URLs::ACTION_BIND_OAUTH, _useLiveServer);
    
    WWW::CREATE( url, f, [ oData, callBack](WWW* www){
        // Check out the result //
        oData->tryParse(www);
        if (callBack) {
            callBack(oData);
        }
    });
}

//GetProfile

void GoPlaySdk::getProfile(Guid gameId)
{
    if (onGetProfile && !_session.empty())
    {
        getProfileAsync(gameId);
    }
}


void GoPlaySdk::getProfileAsync(Guid gameId)
{
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, _session);
    f.addField(Constants::FIELD_GAME_ID, gameId.ToString());
    
    string url = UrlExtension::ToURL(URLs::ACTION_GET_PROFILE, _useLiveServer);
    
    WWW::CREATE( url, f, [&](WWW* www){
        // Compose the Result //
        ProfileResult * result = new ProfileResult();
        result->tryParse(www);
        
        // Trigger Event //
        onGetProfile(result);
        
        delete result;
    });
    
}


void GoPlaySdk::editProfile(Guid gameId, string email, string nickName, Gender gender )
{
    if (onEditProfile && !_session.empty())
    {
        editProfileAsync(gameId, email, nickName, gender);
    }
}


void GoPlaySdk::editProfileAsync(Guid gameId, string email, string nickName, Gender gender)
{
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, _session);
    f.addField(Constants::FIELD_GAME_ID, gameId.ToString());
    
    f.addFieldIfValid(Constants::FIELD_EMAIL, email);
    f.addFieldIfValid(Constants::FIELD_NICKNAME, nickName);
    
    if (gender != Gender::Other){
        f.addField(Constants::FIELD_GENDER, GenderToString[gender]);
    }
    
    string url = UrlExtension::ToURL(URLs::ACTION_EDIT_PROFILE, _useLiveServer);
    
    WWW::CREATE( url, f, [&](WWW* www){
        // Compose the Result //
        ProfileResult * result = new ProfileResult();
        result->tryParse(www);
        
        // Trigger Event //
        onEditProfile(result);
        
        delete result;
    });
}


void GoPlaySdk::getProgress(Guid gameId, bool sendData)
{
    if (onGetProgress && !_session.empty())
    {
        getProgressAsync(gameId, sendData);
    }
}


void GoPlaySdk::getProgressAsync(Guid gameId, bool sendData)
{
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, _session);
    f.addField(Constants::FIELD_GAME_ID, gameId.ToString());
    f.addField(Constants::FIELD_SEND_DATA, btoa(sendData)); // TODO: Test this bool value //
    
    string url = UrlExtension::ToURL(URLs::ACTION_GET_PROGRESS, _useLiveServer);

    WWW::CREATE( url, f, [&](WWW* www){
        // Compose the Result //
        GetProgressResult * result = new GetProgressResult();
        result->tryParse(www);
        
        // Trigger Event //
        onGetProgress(result);
        
        delete result;
    });
}

//SaveProgress
void GoPlaySdk::saveProgress(Guid gameId, string data, string meta)
{
    if (onSaveProgress && !_session.empty())
    {
        saveProgressAsync(gameId, data, meta);
    }
}

void GoPlaySdk::saveProgressAsync(Guid gameId, string data, string meta)
{
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, _session);
    f.addField(Constants::FIELD_GAME_ID, gameId.ToString());
    f.addField(Constants::FIELD_DATA, data);
    f.addFieldIfValid(Constants::FIELD_META, meta);
    
    string url = UrlExtension::ToURL(URLs::ACTION_SAVE_PROGRESS, _useLiveServer);
    
    WWW::CREATE( url, f, [&](WWW* www){
        // Compose the Result //
        Result * result = new Result();
        result->tryParse(www);
        
        // Trigger Event //
        onSaveProgress(result);
        
        delete result;
    });
}

//Update Game Stats
void GoPlaySdk::updateGameStats(Guid gameId, const GameStats& stats)
{
    if (onUpdateGameStats && !_session.empty())
    {
        updateGameStatsAsync(gameId, stats);
    }
}



void GoPlaySdk::updateGameStatsAsync(Guid gameId, const GameStats& starts)
{
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, _session);
    f.addField(Constants::FIELD_GAME_ID, gameId.ToString());
    string startsJson = starts.ToJson();
    f.addField(Constants::FIELD_STATS, startsJson);
    
    string url = UrlExtension::ToURL(URLs::ACTION_UPDATE_GAME_STATS, _useLiveServer);
    
    WWW::CREATE( url, f, [&](WWW* www){
        // Compose the Result //
        Result * result = new Result();
        result->tryParse(www);
        
        // Trigger Event //
        onUpdateGameStats(result);
        
        delete result;
    });
}

//Get Unfullfilled Exchanges

void GoPlaySdk::getUnFullFilledExchanges(Guid gameId)
{
    if (onGetUnFullFilledExchanges && !_session.empty())
    {
        getUnFullFilledExchangesAsync(gameId);
    }
}

void GoPlaySdk::getUnFullFilledExchangesAsync(Guid gameId)
{
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, _session);
    f.addField(Constants::FIELD_GAME_ID, gameId.ToString());
    
    string url = UrlExtension::ToURL(URLs::ACTION_GET_UNFULLFILLED_EXCHANGE, _useLiveServer);
    
    WWW::CREATE( url, f, [&](WWW* www){
        // Compose the Result //
        GetUnFullFilledExchangesResult * result = new GetUnFullFilledExchangesResult();
        result->tryParse(www);
        
        // Trigger Event //
        onGetUnFullFilledExchanges(result);
        
        delete result;
    });
}

//Fullfilled Exchanges
void GoPlaySdk::fullFillExchange(Guid gameId, string transactionId)
{
    if (onFullFillExchange && !_session.empty())
    {
        fullFillExchangeAsync(gameId, transactionId);
    }
}


void GoPlaySdk::fullFillExchangeAsync(Guid gameId, string transactionId)
{
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, _session);
    f.addField(Constants::FIELD_GAME_ID, gameId.ToString());
    f.addField(Constants::FIELD_TRANSACTION_ID, transactionId);
    
    string url = UrlExtension::ToURL(URLs::ACTION_FULLFILL_EXCHANGE, _useLiveServer);
    
    WWW::CREATE( url, f, [&](WWW* www){
        // Compose the Result //
        FullFillExchangeResult * result = new FullFillExchangeResult();
        result->tryParse(www);
        
        // Trigger Event //
        onFullFillExchange(result);
        delete result;
        
    });
}


//Reject Exchanges
void GoPlaySdk::rejectExchange(Guid gameId, string transactionId)
{
    if (onRejectExchange && !_session.empty())
    {
        rejectExchangeAsync(gameId, transactionId);
    }
}

void GoPlaySdk::rejectExchangeAsync(Guid gameId, string transactionId)
{
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, _session);
    f.addField(Constants::FIELD_GAME_ID, gameId.ToString());
    f.addField(Constants::FIELD_TRANSACTION_ID, transactionId);
    
    string url = UrlExtension::ToURL(URLs::ACTION_REJECT_EXCHANGE, _useLiveServer);
    
    WWW::CREATE( url, f, [&](WWW* www){
        // Compose the Result //
        RejectExchangeResult * result = new RejectExchangeResult();
        result->tryParse(www);
        
        // Trigger Event //
        onRejectExchange(result);
        delete result;
        
    });
}


//end--Reject exchange----
void GoPlaySdk::updateExternalExchange(Guid gameId, Guid transactionId, string exchangeOptionIdentifier)
{
    if (onUpdateExternalExchange && !_session.empty())
    {
        updateExternalExchangeAsync(gameId, transactionId, exchangeOptionIdentifier);
    }
}

void GoPlaySdk::updateExternalExchangeAsync(Guid gameId, Guid transactionId, string exchangeOptionIdentifier){
    
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, _session);
    f.addField(Constants::FIELD_GAME_ID, gameId.ToString());
    f.addField(Constants::FIELD_TRANSACTION_ID, transactionId.ToString());
    f.addField(Constants::FIELD_EXCHANGE_OPTION_IDENTIFIER, exchangeOptionIdentifier);
    
    string url = UrlExtension::ToURL(URLs::ACTION_UPDATE_EXTERNAL_EXCHANGE, _useLiveServer);
    
    WWW::CREATE( url, f, [&](WWW* www){
        // Compose the Result //
        Result * result = new Result();
        result->tryParse(www);
        
        // Trigger Event //
        onUpdateExternalExchange(result);
        
        delete result;
    });
}

void GoPlaySdk::unBindOauth(SocialPlatforms platform, string token, Guid gameId){
    if (!_session.empty())
    {
        unBindOauthAsync(platform, token, gameId);
    }
}


void GoPlaySdk::unBindOauthAsync(SocialPlatforms platform, string token, Guid gameId){
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, _session);
    f.addField(Constants::FIELD_SERVICE, SocialPlatformsToString[platform]);
    f.addField(Constants::FIELD_TOKEN, token);
    f.addField(Constants::FIELD_GAME_ID, gameId.ToString());
    
    string url = UrlExtension::ToURL(URLs::ACTION_UNBIND_OAUTH, _useLiveServer);
    WWW::CREATE( url, f, [&, platform, token](WWW* www){
        
        Result * result = new Result();
        result->tryParse(www);
        
        if (result->getSucceeded())
        {
            
        }
        
        delete result;
    });

}

NS_GTOKEN_END