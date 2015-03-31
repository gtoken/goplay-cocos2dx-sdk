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
    void registerWith(string userName, string password, string email = "", string nickName = "", Gender gender = Gender::Other, string referal = "");
    
private:
    void registerAsync(string userName, string password, string email = "", string nickName = "", Gender gender = Gender::Other, string referal = "");

public:
    void logout();
    
private:
    void logoutAsync();

public:
    void login(string userName, string password);
    
private:
    void loginAsync(string userName, string password);
    
    //Login with third party such as FaceBook
public:
    void login(SocialPlatforms platform, string token);
    
private:
    void loginAsync(SocialPlatforms platform, string token);
    
private:
    void bindOauthAsync(OAuthDataObject * oData, ResponseHandler callBack);
    
    //GetProfile
public:
    void getProfile();
    
private:
    void getProfileAsync();
    
public:
    void editProfile(string email = "", string nickName = "", Gender gender = Gender::Other );
    
private:
    void editProfileAsync(string email = "", string nickName = "", Gender gender = Gender::Other);
    
public:
    void getProgress(bool sendData);
    
private:
    void getProgressAsync(bool sendData);
    
    //SaveProgress
public:
    void saveProgress(string data, string meta = "");
    
private:
    void saveProgressAsync( string data, string meta = "");
    
    //Update Game Stats
public:
    void updateGameStats(const GameStats& stats);
    
private:
    void updateGameStatsAsync( const GameStats& stats);
    
    //Get Unfullfilled Exchanges
public:
    void getUnFullFilledExchanges();
    
private:
    void getUnFullFilledExchangesAsync();
    
    //Fullfilled Exchanges
public:
    void fullFillExchange(string transactionId);
    
private:
    void fullFillExchangeAsync(string transactionId);
    
    //Reject Exchanges
public:
    void rejectExchange(string transactionId);
    
private:
    void rejectExchangeAsync(string transactionId);
   
    //Update External Exchange
public:
    void updateExternalExchange(Guid transactionId, string exchangeOptionIdentifier);
    
private:
    void updateExternalExchangeAsync(Guid transactionId, string exchangeOptionIdentifier);

public:
    void unBindOauth(SocialPlatforms platform, string token);
    
private:
    void unBindOauthAsync(SocialPlatforms platform, string token);
    

};

NS_GTOKEN_END

#endif /* defined(__GoPlaySDK__GoPlaySdk__) */
