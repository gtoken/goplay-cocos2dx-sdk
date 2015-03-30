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

class GoPlaySdk : public Singleton<GoPlaySdk>{
private:
    
    virtual ~GoPlaySdk(){
        SAFE_DELETE(_requireOAuthData);
    }
    
    SET_GET(bool, _useLiveServer, UseLiveServer, false)
    SET_GET(string, _session, Session, "")
    SET_GET(OAuthDataObject *, _requireOAuthData, RequireOAuthData, nullptr)
    
    typedef std::function<void(IResult * r)> ResponseHandler;
    
    // Events //
public:
    ResponseHandler onLogin = nullptr;
    ResponseHandler onLogOut = nullptr;
    ResponseHandler onRegister = nullptr;
    ResponseHandler onGetProfile = nullptr;
    ResponseHandler onEditProfile = nullptr;
    ResponseHandler onGetProgress = nullptr;
    ResponseHandler onSaveProgress = nullptr;
    ResponseHandler onUpdateGameStats = nullptr;
    ResponseHandler onGetUnFullFilledExchanges = nullptr;
    ResponseHandler onFullFillExchange = nullptr;
    ResponseHandler onUpdateExternalExchange = nullptr;
    ResponseHandler onRejectExchange = nullptr;
    
    // Supported Operations //
public:
    void registerWith(string userName, string password, Guid gameId, string email = "", string nickName = "", Gender gender = Gender::Other, string referal = "");
    
private:
    void registerAsync(string userName, string password, Guid gameId, string email = "", string nickName = "", Gender gender = Gender::Other, string referal = "");
    
public:
    void login(string userName, string password, Guid gameId);
    
private:
    void loginAsync(string userName, string password, Guid gameId);
    
    //Login with third party such as FaceBook
public:
    void login(SocialPlatforms platform, string token, Guid gameId);
    
private:
    void loginAsync(SocialPlatforms platform, string token, Guid gameId);
    
private:
    void bindOauthAsync(Guid gameId, OAuthDataObject * oData, ResponseHandler callBack);
    
    //GetProfile
public:
    void getProfile(Guid gameId);
    
private:
    void getProfileAsync(Guid gameId);
    
public:
    void editProfile(Guid gameId, string email = "", string nickName = "", Gender gender = Gender::Other );
    
private:
    void editProfileAsync(Guid gameId, string email = "", string nickName = "", Gender gender = Gender::Other);
    
public:
    void getProgress(Guid gameId, bool sendData);
    
private:
    void getProgressAsync(Guid gameId, bool sendData);
    
    //SaveProgress
public:
    void saveProgress(Guid gameId, string data, string meta = "");
    
private:
    void saveProgressAsync(Guid gameId, string data, string meta = "");
    
    //Update Game Stats
public:
    void updateGameStats(Guid gameId, const GameStats& stats);
    
private:
    void updateGameStatsAsync(Guid gameId, const GameStats& stats);
    
    //Get Unfullfilled Exchanges
public:
    void getUnFullFilledExchanges(Guid gameId);
    
private:
    void getUnFullFilledExchangesAsync(Guid gameId);
    
    //Fullfilled Exchanges
public:
    void fullFillExchange(Guid gameId, string transactionId);
    
private:
    void fullFillExchangeAsync(Guid gameId, string transactionId);
    
    //Reject Exchanges
public:
    void rejectExchange(Guid gameId, string transactionId);
    
private:
    void rejectExchangeAsync(Guid gameId, string transactionId);
   
    //Update External Exchange
public:
    void updateExternalExchange(Guid gameId, Guid transactionId, string exchangeOptionIdentifier); 
    
private:
    void updateExternalExchangeAsync(Guid gameId, Guid transactionId, string exchangeOptionIdentifier);

public:
    void unBindOauth(SocialPlatforms platform, string token, Guid gameId);
    
private:
    void unBindOauthAsync(SocialPlatforms platform, string token, Guid gameId);
    

};

NS_GTOKEN_END

#endif /* defined(__GoPlaySDK__GoPlaySdk__) */
