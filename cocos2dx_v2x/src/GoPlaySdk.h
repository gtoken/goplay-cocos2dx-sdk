//
//  GoPlaySdk.h
//  GoPlaySDK
//
//
//

#ifndef __GoPlaySDK__GoPlaySdk__
#define __GoPlaySDK__GoPlaySdk__

#include "Singleton.h"
#include "Results.h"
#include "GoPlaySDKConfig.h"

using namespace std;

NS_GTOKEN_BEGIN

class GoPlaySdk : public CCNode {
private:
    GoPlaySdk();
    
    virtual ~GoPlaySdk(){
        SAFE_DELETE(_requireOAuthData);
        deleteAllResponseHandler();
    }
    
    SET_GET(bool, _useLiveServer, UseLiveServer, false)
    SET_GET(string, _session, Session, "")
    SET_GET(OAuthDataObject *, _requireOAuthData, RequireOAuthData, NULL)
    
    static GoPlaySdk* _instance;
    
protected:
    bool isHandlerEnable(ResponseHandler* &callback);
    
public:
    
    static GoPlaySdk* GetInstance();
    static void destroy();
    
    SET_GET_HANDLER (ResponseHandler*, onLogin, OnLoginHandler, NULL)
    SET_GET_HANDLER (ResponseHandler*, onRegister, OnRegisterHandler, NULL)
    SET_GET_HANDLER (ResponseHandler*, onGetProfile, OnGetProfileHandler, NULL)
    SET_GET_HANDLER (ResponseHandler*, onEditProfile, OnEditProfile, NULL)
    SET_GET_HANDLER (ResponseHandler*, onGetProgress, OnGetProgress, NULL)
    SET_GET_HANDLER (ResponseHandler*, onSaveProgress, OnSaveProgress, NULL)
    SET_GET_HANDLER (ResponseHandler*, onUpdateGameStats, OnUpdateGameStats, NULL)
    SET_GET_HANDLER (ResponseHandler*, onGetUnFullFilledExchanges, OnGetUnFullFilledExchanges, NULL)
    SET_GET_HANDLER (ResponseHandler*, onFullFillExchange, OnFullFillExchange, NULL)
    SET_GET_HANDLER (ResponseHandler*, onUpdateExternalExchange, OnUpdateExternalExchange, NULL)
    SET_GET_HANDLER (ResponseHandler*, onRejectExchange, OnRejectExchange, NULL)
    
    void deleteAllResponseHandler();
    
    // Supported Operations //
public:
    void registerWith(string userName, string password, string email = "", string nickName = "", Gender gender = GenderOther, string referal = "");
    
private:
    void registerAsync(string userName, string password, string email = "", string nickName = "", Gender gender = GenderOther, string referal = "");
    
    void onRegisterAsyncCallback(CCNode* node);
    
    void onbindOauthByRegisterCallback(IResult* r);

public:
    void logout();
    
private:
    void logoutAsync();

public:
    void login(string userName, string password);
    
private:
    void loginAsync(string userName, string password);
    
    void onLoginAsyncCallback(CCNode* node);
    
    void onbindOauthByLoginCallback(IResult* r);
    
    //Login with third party such as FaceBook
public:
    void login(SocialPlatforms platform, string token);
    
private:
    void loginAsync(SocialPlatforms platform, string token);
    
    void onLoginAsyncSocialCallback(CCNode* node);
    
private:
    void bindOauthAsync(OAuthDataObject * oData, ResponseHandler& callBack);
    
    void onbindOauthAsyncCallback(CCNode* node);
    
    //GetProfile
public:
    void getProfile();
    
private:
    void getProfileAsync();
    
    void onGetProfileAsyncCallback(CCNode* node);
    
public:
    void editProfile(string email = "", string nickName = "", Gender gender = GenderOther );
    
private:
    void editProfileAsync(string email = "", string nickName = "", Gender gender = GenderOther);
    
    void onEditProfileAsyncCallbackk(CCNode* node);
    
public:
    void getProgress(bool sendData);
    
private:
    void getProgressAsync(bool sendData);
    
    void onGetProgressAsyncCallback(CCNode* node);
    
    //SaveProgress
public:
    void saveProgress(string data, string meta = "");
    
private:
    void saveProgressAsync( string data, string meta = "");
    
    void onSaveProgressAsyncCallback(CCNode* node);
    
    //Update Game Stats
public:
    void updateGameStats(const GameStats& stats);
    
private:
    void updateGameStatsAsync( const GameStats& stats);
    
    void onUpdateGameStatsAsyncCallback(CCNode* node);
    
    //Get Unfullfilled Exchanges
public:
    void getUnFullFilledExchanges();
    
private:
    void getUnFullFilledExchangesAsync();
    
    void onGetUnFullFilledExchangesAsyncCallback(CCNode* node);
    
    //Fullfilled Exchanges
public:
    void fullFillExchange(string transactionId);
    
private:
    void fullFillExchangeAsync(string transactionId);
    
    void onFullFillExchangeAsyncCallback(CCNode* node);
    
    //Reject Exchanges
public:
    void rejectExchange(string transactionId);
    
private:
    void rejectExchangeAsync(string transactionId);
    
    void onRejectExchangeAsyncCallback(CCNode* node);
   
    //Update External Exchange
public:
    void updateExternalExchange(Guid transactionId, string exchangeOptionIdentifier);
    
    void onUpdateExternalExchangeCallback(CCNode* node);
    
private:
    void updateExternalExchangeAsync(Guid transactionId, string exchangeOptionIdentifier);

public:
    void unBindOauth(SocialPlatforms platform, string token);
    
private:
    void unBindOauthAsync(SocialPlatforms platform, string token);
    
    void onUnBindOauthAsyncCallback(CCNode* node);
    
private:
    ResponseHandler onbindOauthByRegister;
    ResponseHandler onbindOauthByLogin;
    
    // Temp data
    SocialPlatforms _loginSocial;
    std::string _loginToken;
    ResponseHandler _bindOauthCallback;
    OAuthDataObject* _oauthDataCallback;
};

NS_GTOKEN_END

#endif /* defined(__GoPlaySDK__GoPlaySdk__) */
