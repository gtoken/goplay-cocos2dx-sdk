//
//  GoPlaySdk.cpp
//  GoPlaySDK
//
//
//

#include "GoPlaySdk.h"
#include "CommonTypes.h"

NS_GTOKEN_BEGIN

GoPlaySdk* GoPlaySdk::_instance = NULL;

GoPlaySdk* GoPlaySdk::GetInstance()
{
    if (_instance == NULL)
    {
        _instance = new GoPlaySdk();
    }
    return _instance;
}

void GoPlaySdk::destroy()
{
    if (_instance)
    {
        delete _instance;
        _instance = NULL;
    }
}

GoPlaySdk::GoPlaySdk() : CCNode(), _loginToken(""), _loginSocial(SocialPlatformNone),
_oauthDataCallback(NULL)
{
    retain();
    
    onbindOauthByRegister.target = this;
    onbindOauthByRegister.callback = callfuncResult_selector(GoPlaySdk::onbindOauthByRegisterCallback);
    
    onbindOauthByLogin.target = this;
    onbindOauthByLogin.callback = callfuncResult_selector(GoPlaySdk::onbindOauthByRegisterCallback);
}

bool GoPlaySdk::isHandlerEnable(GoPlaySDK::ResponseHandler *&callback)
{
    if (callback && callback->target && callback->callback)
        return true;
    return  false;
}

void GoPlaySdk::deleteAllResponseHandler()
{
    SAFE_DELETE(onLogin);
    SAFE_DELETE(onRegister);
    SAFE_DELETE(onGetProfile);
    SAFE_DELETE(onEditProfile);
    SAFE_DELETE(onGetProgress);
    SAFE_DELETE(onSaveProgress);
    SAFE_DELETE(onUpdateGameStats);
    SAFE_DELETE(onGetUnFullFilledExchanges);
    SAFE_DELETE(onFullFillExchange);
    SAFE_DELETE(onUpdateExternalExchange);
    SAFE_DELETE(onRejectExchange);
}

// Supported Operations //

void GoPlaySdk::registerWith(string userName,
                         string password,
                         string email,
                         string nickName,
                         Gender gender,
                         string referal)
{
    if (isHandlerEnable(onRegister))
    {
        registerAsync(userName, password, email, nickName, gender, referal);
    }
}



void GoPlaySdk::registerAsync(string userName,
                          string password,
                          string email,
                          string nickName,
                          Gender gender,
                          string referal)
{
    WWWForm f = WWWForm();
    f.addFieldLowerCase(Constants::FIELD_USERNAME, userName);
    f.addFieldLowerCase(Constants::FIELD_PASSWORD, password);
    f.addField(Constants::FIELD_GAME_ID, Guid(GAME_ID).ToString());
    f.addField(Constants::FIELD_GENDER, GenderToString[gender]);
    
    f.addFieldIfValid(Constants::FIELD_EMAIL, email);
    f.addFieldIfValid(Constants::FIELD_NICKNAME, nickName);
    f.addFieldIfValid(Constants::FIELD_REFERRAL_CODE, referal);
    
    string url = UrlExtension::ToURL(URLs::ACTION_REGISTER, _useLiveServer);
    WWW::CREATE( url, f, this, callfuncN_selector(GoPlaySdk::onRegisterAsyncCallback) );
}

void GoPlaySdk::onRegisterAsyncCallback(CCNode* node)
{
    WWW* www = dynamic_cast<WWW*>(node);
    if (!www)
        return;
    
    // Compose the Result //
    RegisterResult * result = new RegisterResult();
    result->tryParse(www);
    
    if (result->getSucceeded()) _session = result->getSession();
    
    // Need to connect this user with Social platform ? //
    if (result->getSucceeded() && _requireOAuthData != NULL)
    {
        bindOauthAsync(_requireOAuthData, onbindOauthByRegister);
        
    } else {
        // Trigger Event //
        if (isHandlerEnable(onRegister)) {
            (onRegister->target->*onRegister->callback)(result);
        }
    }
    
    delete result;
}

void GoPlaySdk::onbindOauthByRegisterCallback(GoPlaySDK::IResult* r)
{
    if (isHandlerEnable(onRegister))
    {
       (onRegister->target->*onRegister->callback)(r);
    }
}

void GoPlaySdk::logout()
{
    logoutAsync();
}


void GoPlaySdk::logoutAsync(){
    _session = "";
}

void GoPlaySdk::login(string userName, string password)
{
    if (isHandlerEnable(onLogin))
    {
        loginAsync(userName, password);
    }
}


void GoPlaySdk::loginAsync(string userName, string password){
    WWWForm f = WWWForm();
    f.addFieldLowerCase(Constants::FIELD_USERNAME, userName);
    f.addFieldLowerCase(Constants::FIELD_PASSWORD, password);
    f.addField(Constants::FIELD_GAME_ID, Guid(GAME_ID).ToString());
    string url = UrlExtension::ToURL(URLs::ACTION_LOGIN, _useLiveServer);
    
    WWW::CREATE(url, f, this, callfuncN_selector(GoPlaySdk::onLoginAsyncCallback));
}
                         
                         
void GoPlaySdk::onLoginAsyncCallback(cocos2d::CCNode *node)
{
    WWW* www = dynamic_cast<WWW*>(node);
    if (!www)
        return;

    // Compose the Result //
    LoginResult * result = new LoginResult();
    result->tryParse(www);
    
    if (result->getSucceeded()) _session = result->getSession();
    
    // Need to connect this user with Social platform ? //
    if (result->getSucceeded() && _requireOAuthData)
    {
        // Perform Oauth-Connect //
        bindOauthAsync(_requireOAuthData, onbindOauthByLogin);
        
    }else{
        // Trigger Event //
        if (isHandlerEnable(onLogin))
           (onLogin->target->*onLogin->callback)(result);
    }
    
    delete result;
}

void GoPlaySdk::onbindOauthByLoginCallback(GoPlaySDK::IResult *r)
{
    if (isHandlerEnable(onLogin)) {
        (onLogin->target->*onLogin->callback)(r);
    }
}


//Login with third party such as FaceBook

void GoPlaySdk::login(SocialPlatforms platform, string token)
{
    if (isHandlerEnable(onLogin))
    {
        loginAsync(platform, token);
    }
}

void GoPlaySdk::loginAsync(SocialPlatforms platform, string token)
{
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SERVICE, SocialPlatformsToString[platform]);
    f.addField(Constants::FIELD_TOKEN, token);
    f.addField(Constants::FIELD_GAME_ID, Guid(GAME_ID).ToString());
    
    _loginSocial = platform;
    _loginToken = token;
    
    string url = UrlExtension::ToURL(URLs::ACTION_LOGIN_OAUTH, _useLiveServer);
    WWW::CREATE( url, f, this, callfuncN_selector(GoPlaySdk::onLoginAsyncSocialCallback) );
}

void GoPlaySdk::onLoginAsyncSocialCallback(cocos2d::CCNode *node)
{
    WWW* www = dynamic_cast<WWW*>(node);
    if (!www)
        return;
        
    // Compose the Result //
    LoginResult * result = new LoginResult();
    result->tryParse(www);
    
    if (result->getSucceeded())
    {
        _session = result->getSession();
        
        SAFE_DELETE(_requireOAuthData);
        _requireOAuthData = new OAuthDataObject();
        _requireOAuthData->setPlatform(_loginSocial);
        _requireOAuthData->setToken(_loginToken);
    }
    else
    {
        // UI to display the Login/Register screen //
        if (result->getErrorCode() == ErrorOauthUserNotExist)
        {
            SAFE_DELETE(_requireOAuthData);
            _requireOAuthData = new OAuthDataObject();
            _requireOAuthData->setPlatform(_loginSocial);
            _requireOAuthData->setToken(_loginToken);
        }
    }
    
    // Trigger Event //
    if (isHandlerEnable(onLogin))
        (onLogin->target->*onLogin->callback)(result);
    
    delete result;
}

void GoPlaySdk::bindOauthAsync( OAuthDataObject * oData, ResponseHandler& callBack)
{
    _oauthDataCallback = oData;
    _bindOauthCallback = callBack;
    
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, getSession());
    f.addField(Constants::FIELD_GAME_ID, Guid(GAME_ID).ToString());
    f.addField(Constants::FIELD_SERVICE, SocialPlatformsToString[oData->getPlatform()]);
    f.addFieldIfValid(Constants::FIELD_TOKEN, oData->getToken());
    
    string url = UrlExtension::ToURL(URLs::ACTION_BIND_OAUTH, _useLiveServer);
    
    WWW::CREATE( url, f, this, callfuncN_selector(GoPlaySdk::onbindOauthAsyncCallback));
}

void GoPlaySdk::onbindOauthAsyncCallback(CCNode* node)
{
    WWW* www = dynamic_cast<WWW*>(node);
    if (!www)
        return;
    
    // Check out the result //
    _oauthDataCallback->tryParse(www);
    
    if (_bindOauthCallback.target && _bindOauthCallback.callback) {
        (_bindOauthCallback.target->*_bindOauthCallback.callback)(_oauthDataCallback);
    }
}


//GetProfile

void GoPlaySdk::getProfile()
{
    if (isHandlerEnable(onGetProfile) && !_session.empty())
    {
        getProfileAsync();
    }
}


void GoPlaySdk::getProfileAsync()
{
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, _session);
    f.addField(Constants::FIELD_GAME_ID, Guid(GAME_ID).ToString());
    
    string url = UrlExtension::ToURL(URLs::ACTION_GET_PROFILE, _useLiveServer);
    
    WWW::CREATE( url, f, this, callfuncN_selector(GoPlaySdk::onGetProfileAsyncCallback));
}

void GoPlaySdk::onGetProfileAsyncCallback(CCNode* node)
{
    WWW* www = dynamic_cast<WWW*>(node);
    if (!www)
        return;
    
    // Compose the Result //
    ProfileResult * result = new ProfileResult();
    result->tryParse(www);
    
    // Trigger Event //
    if (isHandlerEnable(onGetProfile))
        (onGetProfile->target->*onGetProfile->callback)(result);
        
    delete result;
}

void GoPlaySdk::editProfile(string email, string nickName, Gender gender )
{
    if (isHandlerEnable(onEditProfile) && !_session.empty())
    {
        editProfileAsync(email, nickName, gender);
    }
}


void GoPlaySdk::editProfileAsync(string email, string nickName, Gender gender)
{
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, _session);
    f.addField(Constants::FIELD_GAME_ID, Guid(GAME_ID).ToString());
    
    f.addFieldIfValid(Constants::FIELD_EMAIL, email);
    f.addFieldIfValid(Constants::FIELD_NICKNAME, nickName);
    
    if (gender != GenderOther){
        f.addField(Constants::FIELD_GENDER, GenderToString[gender]);
    }
    
    string url = UrlExtension::ToURL(URLs::ACTION_EDIT_PROFILE, _useLiveServer);
    
    WWW::CREATE( url, f, this, callfuncN_selector(GoPlaySdk::onEditProfileAsyncCallbackk));
}

void GoPlaySdk::onEditProfileAsyncCallbackk(cocos2d::CCNode *node)
{
    WWW* www = dynamic_cast<WWW*>(node);
    if (!www)
        return;
    
    // Compose the Result //
    ProfileResult * result = new ProfileResult();
    result->tryParse(www);
    
    // Trigger Event //
    if (isHandlerEnable(onEditProfile))
        (onEditProfile->target->*onEditProfile->callback)(result);
    
    delete result;
}


void GoPlaySdk::getProgress(bool sendData)
{
    if (isHandlerEnable(onGetProgress) && !_session.empty())
    {
        getProgressAsync(sendData);
    }
}


void GoPlaySdk::getProgressAsync(bool sendData)
{
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, _session);
    f.addField(Constants::FIELD_GAME_ID, Guid(GAME_ID).ToString());
    f.addField(Constants::FIELD_SEND_DATA, btoa(sendData)); // TODO: Test this bool value //
    
    string url = UrlExtension::ToURL(URLs::ACTION_GET_PROGRESS, _useLiveServer);

    WWW::CREATE(url, f, this, callfuncN_selector(GoPlaySdk::onGetProgressAsyncCallback));
}

void GoPlaySdk::onGetProgressAsyncCallback(cocos2d::CCNode *node)
{
    WWW* www = dynamic_cast<WWW*>(node);
    if (!www)
        return;
    
    // Compose the Result //
    GetProgressResult * result = new GetProgressResult();
    result->tryParse(www);
    
    // Trigger Event //
    if (isHandlerEnable(onGetProgress))
        (onGetProgress->target->*onGetProgress->callback)(result);
    
    delete result;
    
}

//SaveProgress
void GoPlaySdk::saveProgress(string data, string meta)
{
    if (isHandlerEnable(onSaveProgress) && !_session.empty())
    {
        saveProgressAsync(data, meta);
    }
}

void GoPlaySdk::saveProgressAsync(string data, string meta)
{
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, _session);
    f.addField(Constants::FIELD_GAME_ID, Guid(GAME_ID).ToString());
    f.addField(Constants::FIELD_DATA, data);
    f.addFieldIfValid(Constants::FIELD_META, meta);
    
    string url = UrlExtension::ToURL(URLs::ACTION_SAVE_PROGRESS, _useLiveServer);
    
    WWW::CREATE( url, f, this, callfuncN_selector(GoPlaySdk::onSaveProgressAsyncCallback));
}

void GoPlaySdk::onSaveProgressAsyncCallback(cocos2d::CCNode *node)
{
    WWW* www = dynamic_cast<WWW*>(node);
    if (!www)
        return;
    
    // Compose the Result //
    Result * result = new Result();
    result->tryParse(www);
    
    // Trigger Event //
    if (isHandlerEnable(onSaveProgress))
        (onSaveProgress->target->*onSaveProgress->callback)(result);
    
    delete result;
}

//Update Game Stats
void GoPlaySdk::updateGameStats(const GameStats& stats)
{
    if (isHandlerEnable(onUpdateGameStats) && !_session.empty())
    {
        updateGameStatsAsync(stats);
    }
}



void GoPlaySdk::updateGameStatsAsync(const GameStats& starts)
{
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, _session);
    f.addField(Constants::FIELD_GAME_ID, Guid(GAME_ID).ToString());
    string startsJson = starts.ToJson();
    f.addField(Constants::FIELD_STATS, startsJson);
    
    string url = UrlExtension::ToURL(URLs::ACTION_UPDATE_GAME_STATS, _useLiveServer);
    
    WWW::CREATE( url, f, this, callfuncN_selector(GoPlaySdk::onUpdateGameStatsAsyncCallback));
}

void GoPlaySdk::onUpdateGameStatsAsyncCallback(cocos2d::CCNode *node)
{
    WWW* www = dynamic_cast<WWW*>(node);
    if (!www)
        return;
    
    // Compose the Result //
    Result * result = new Result();
    result->tryParse(www);
    
    // Trigger Event //
    if (isHandlerEnable(onUpdateGameStats))
        (onUpdateGameStats->target->*onUpdateGameStats->callback)(result);
    
    delete result;

}

//Get Unfullfilled Exchanges

void GoPlaySdk::getUnFullFilledExchanges()
{
    if (isHandlerEnable(onGetUnFullFilledExchanges) && !_session.empty())
    {
        getUnFullFilledExchangesAsync();
    }
}

void GoPlaySdk::getUnFullFilledExchangesAsync()
{
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, _session);
    f.addField(Constants::FIELD_GAME_ID, Guid(GAME_ID).ToString());
    
    string url = UrlExtension::ToURL(URLs::ACTION_GET_UNFULLFILLED_EXCHANGE, _useLiveServer);
    
    WWW::CREATE( url, f, this, callfuncN_selector(GoPlaySdk::onGetUnFullFilledExchangesAsyncCallback));
}

void GoPlaySdk::onGetUnFullFilledExchangesAsyncCallback(cocos2d::CCNode *node)
{
    WWW* www = dynamic_cast<WWW*>(node);
    if (!www)
        return;
    
    // Compose the Result //
    GetUnFullFilledExchangesResult * result = new GetUnFullFilledExchangesResult();
    result->tryParse(www);
    
    // Trigger Event //
    if (isHandlerEnable(onGetUnFullFilledExchanges))
        (onGetUnFullFilledExchanges->target->*onGetUnFullFilledExchanges->callback)(result);
    
    delete result;
}

//Fullfilled Exchanges
void GoPlaySdk::fullFillExchange(string transactionId)
{
    if (isHandlerEnable(onFullFillExchange) && !_session.empty())
    {
        fullFillExchangeAsync(transactionId);
    }
}


void GoPlaySdk::fullFillExchangeAsync(string transactionId)
{
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, _session);
    f.addField(Constants::FIELD_GAME_ID, Guid(GAME_ID).ToString());
    f.addField(Constants::FIELD_TRANSACTION_ID, transactionId);
    
    string url = UrlExtension::ToURL(URLs::ACTION_FULLFILL_EXCHANGE, _useLiveServer);
    
    WWW::CREATE( url, f, this, callfuncN_selector(GoPlaySdk::onFullFillExchangeAsyncCallback));
}

void GoPlaySdk::onFullFillExchangeAsyncCallback(cocos2d::CCNode *node)
{
    WWW* www = dynamic_cast<WWW*>(node);
    if (!www)
        return;
    
    // Compose the Result //
    FullFillExchangeResult * result = new FullFillExchangeResult();
    result->tryParse(www);
    
    // Trigger Event //
    if (isHandlerEnable(onFullFillExchange))
        (onFullFillExchange->target->*onFullFillExchange->callback)(result);
    
    delete result;
}

//Reject Exchanges
void GoPlaySdk::rejectExchange(string transactionId)
{
    if (isHandlerEnable(onRejectExchange) && !_session.empty())
    {
        rejectExchangeAsync(transactionId);
    }
}

void GoPlaySdk::rejectExchangeAsync(string transactionId)
{
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, _session);
    f.addField(Constants::FIELD_GAME_ID, Guid(GAME_ID).ToString());
    f.addField(Constants::FIELD_TRANSACTION_ID, transactionId);
    
    string url = UrlExtension::ToURL(URLs::ACTION_REJECT_EXCHANGE, _useLiveServer);
    
    WWW::CREATE( url, f, this, callfuncN_selector(GoPlaySdk::onRejectExchangeAsyncCallback));
}

void GoPlaySdk::onRejectExchangeAsyncCallback(cocos2d::CCNode *node)
{
    WWW* www = dynamic_cast<WWW*>(node);
    if (!www)
        return;
    
    // Compose the Result //
    RejectExchangeResult * result = new RejectExchangeResult();
    result->tryParse(www);
    
    // Trigger Event //
    if (isHandlerEnable(onRejectExchange))
        (onRejectExchange->target->*onRejectExchange->callback)(result);
    
    delete result;
}


//end--Reject exchange----
void GoPlaySdk::updateExternalExchange(Guid transactionId, string exchangeOptionIdentifier)
{
    if (isHandlerEnable(onUpdateExternalExchange) && !_session.empty())
    {
        updateExternalExchangeAsync(transactionId, exchangeOptionIdentifier);
    }
}

void GoPlaySdk::updateExternalExchangeAsync(Guid transactionId, string exchangeOptionIdentifier){
    
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, _session);
    f.addField(Constants::FIELD_GAME_ID, Guid(GAME_ID).ToString());
    f.addField(Constants::FIELD_TRANSACTION_ID, transactionId.ToString());
    f.addField(Constants::FIELD_EXCHANGE_OPTION_IDENTIFIER, exchangeOptionIdentifier);
    
    string url = UrlExtension::ToURL(URLs::ACTION_UPDATE_EXTERNAL_EXCHANGE, _useLiveServer);
    
    WWW::CREATE( url, f, this, callfuncN_selector(GoPlaySdk::onUpdateExternalExchangeCallback));
}

void GoPlaySdk::onUpdateExternalExchangeCallback(cocos2d::CCNode *node)
{
    WWW* www = dynamic_cast<WWW*>(node);
    if (!www)
        return;
    
    // Compose the Result //
    Result * result = new Result();
    result->tryParse(www);
    
    // Trigger Event //
    if (isHandlerEnable(onUpdateExternalExchange))
        (onUpdateExternalExchange->target->*onUpdateExternalExchange->callback)(result);
    
    delete result;
}

void GoPlaySdk::unBindOauth(SocialPlatforms platform, string token){
    if (!_session.empty())
    {
        unBindOauthAsync(platform, token);
    }
}


void GoPlaySdk::unBindOauthAsync(SocialPlatforms platform, string token){
    WWWForm f = WWWForm();
    f.addField(Constants::FIELD_SESSION, _session);
    f.addField(Constants::FIELD_SERVICE, SocialPlatformsToString[platform]);
    f.addField(Constants::FIELD_TOKEN, token);
    f.addField(Constants::FIELD_GAME_ID, Guid(GAME_ID).ToString());
    
    string url = UrlExtension::ToURL(URLs::ACTION_UNBIND_OAUTH, _useLiveServer);
    WWW::CREATE( url, f, this, callfuncN_selector(GoPlaySdk::onUnBindOauthAsyncCallback));

}

void GoPlaySdk::onUnBindOauthAsyncCallback(cocos2d::CCNode *node)
{
    WWW* www = dynamic_cast<WWW*>(node);
    if (!www)
        return;
    
    Result * result = new Result();
    result->tryParse(www);
    
    if (result->getSucceeded())
    {
        
    }
    
    delete result;
}

NS_GTOKEN_END