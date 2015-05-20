//
//  UiExchange.h
//  GTokenSDKDemo
//
//  Created by DucHQ on 3/10/15.
//
//

#ifndef __GTokenSDKDemo__UiExchange__
#define __GTokenSDKDemo__UiExchange__

#include "Common/AvaButton.h"
#include "../GoPlaySDK/Results.h"
#include <string>

class TextBoxEx;

using namespace std;

class UiExchange : public cocos2d::CCLayer {
private:
    cocos2d::CCLabelTTF * logTest = NULL;
    cocos2d::CCNode * nodeLoading = NULL;
public:
    UiExchange();
    virtual ~UiExchange();
    void initWith();
    void setDebugString( std::string s, std::string sub = "");
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void registerWithTouchDispatcher();
    
private:
    void createBt(const string& t, const float& x, const float& y, CCObject* target, SEL_CallFuncN onClick, const char* s_bt_1 = "common/dialog_bt_01.png", const char* s_bt_2 = "common/dialog_bt_02.png");
    
    void increaseLoading();
    void decreaseLoading();
    
    TextBoxEx* _textBox;
    
    TextBoxEx* _updateTex;
    TextBoxEx* _updateTex2;
    
protected:
    void onBtnBack(CCNode* node);
    
    void onBtnProfile(CCNode* sender);
    void onResponseProfile(GoPlaySDK::IResult* result);
    
    void onBtnGetUnFill(CCNode* node);
    void onResponseGetUnFill(GoPlaySDK::IResult* result);
    
    void onBtnFullFill(CCNode* node);
    void onResponseFullFill(GoPlaySDK::IResult* result);
    
    void onBtnRequest(CCNode* node);
    void onResponseFullFillExchange(GoPlaySDK::IResult* result);
    
    void onBtnUpdateExternal(CCNode* node);
    void onBtnRequestUpdateExternal(CCNode* node);
    void onResponseUpdateExternal(GoPlaySDK::IResult* result);
    
    void onBtnDisOauth(CCNode* node);
};

#endif /* defined(__GTokenSDKDemo__UiExchange__) */
