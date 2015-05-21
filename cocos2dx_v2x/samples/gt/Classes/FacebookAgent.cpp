#include "FacebookAgent.h"
USING_NS_CC;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif

#include "FacebookIOS.h"

#define FB_CLASS_PATH "com/gt/sdk/AppActivity"

FacebookAgent* FacebookAgent::_instance = NULL;

FacebookAgent::FacebookAgent() : _accessToken("")
{
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FacebookAgent::onLoginResponse), NOTIFICATION_LOGIN_FB, NULL);

}

FacebookAgent::~FacebookAgent()
{
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_LOGIN_FB);
}

FacebookAgent* FacebookAgent::getInstance()
{
    if (!_instance)
    {
        _instance = new FacebookAgent();
    }
    return _instance;
}

void FacebookAgent::release()
{
    if (_instance)
    {
        delete _instance;
        _instance = NULL;
    }
}

void FacebookAgent::login(cocos2d::CCObject* target, cocos2d::SEL_CallFunc callback)
{
	_listener = target;
	_callback = callback;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo minfo;
	bool isHave = cocos2d::JniHelper::getStaticMethodInfo(minfo, FB_CLASS_PATH, "loginFacebook", "()V");

	if (isHave)
	{
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

	// post notification
	CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_LOGIN_FB, NULL);

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    FacebookIOS::getInstance()->login();

#endif
}

bool FacebookAgent::isLoggedIn()
{
    return !_accessToken.empty();
}

void FacebookAgent::logout()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo minfo;
	bool isHave = cocos2d::JniHelper::getStaticMethodInfo(minfo, FB_CLASS_PATH, "logoutFacebook", "()V");

	if (isHave)
	{
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
    
#endif
}

std::string FacebookAgent::getAccessToken()
{
    return _accessToken;
}

void FacebookAgent::onLoginResponse(cocos2d::CCObject* result)
{
	if (result)
	{
		CCString* token = dynamic_cast<CCString*>(result);
		_accessToken = token->getCString();
		delete token;
	}
	
	if (_listener && _callback)
	{
		(_listener->*_callback)();
	}	
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
extern "C" 
{
	JNIEXPORT void JNICALL Java_com_gt_sdk_AppActivity_nativeLoginFB(JNIEnv *env, jclass jclass, jstring jtoken) 
	{
		const char *token = env->GetStringUTFChars(jtoken, NULL);

		// post notification
		CCString* str = new CCString(token);
		CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_LOGIN_FB, str);

		env->ReleaseStringUTFChars(jtoken, token);
	}
}
#endif