#ifndef __gt__FacebookAgent__
#define __gt__FacebookAgent__

#include "cocos2d.h"

static const char* NOTIFICATION_LOGIN_FB = "NOTIFICATION_LOGIN_FB";

class FacebookAgent : public cocos2d::CCObject
{
public:
    static FacebookAgent* getInstance();
    static void release();
    
    void login(cocos2d::CCObject* target, cocos2d::SEL_CallFunc callback);
    
    bool isLoggedIn();
    
    void logout();
    
    std::string getAccessToken();
    
private:
    FacebookAgent();
    ~FacebookAgent();

	void onLoginResponse(cocos2d::CCObject* result);
    
    cocos2d::CCObject* _listener;
    cocos2d::SEL_CallFunc _callback;

	std::string _accessToken;
    
    static FacebookAgent* _instance;
};

#endif /* defined(__gt__FacebookAgent__) */
