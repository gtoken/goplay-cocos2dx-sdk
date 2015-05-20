//
//  UiLogin.h
//  TestGame
//
//  Created by DucHQ on 8/20/14.
//
//

#ifndef __TestGame__UiLogin__
#define __TestGame__UiLogin__

#include "Common/AvaFrame.h"
#include "Common/TextBoxEx.h"
#include "../GoPlaySDK/GoPlaySdk.h"

class AvaFrame_Login : public AvaFrame{
public:
    TextBoxEx * _name;
    TextBoxEx * _pass;
public:
    AvaFrame_Login(){
        _name = NULL;
        _pass = NULL;
    }
    
    virtual ~AvaFrame_Login(){

    }
};


class AvaFrame_Register : public AvaFrame{
public:
    TextBoxEx * _name;
    TextBoxEx * _pass;
    TextBoxEx * _pass2;
public:
    AvaFrame_Register(){
        _name = NULL;
        _pass = NULL;
        _pass2 = NULL;
    }
    
    virtual ~AvaFrame_Register(){

    }
    
    void createTextBox(){
        TextBoxEx * _edit_username = new TextBoxEx();
        _edit_username->initWith();
        _edit_username->setPlaceHolder("Login name:");
        this->addChild(_edit_username);
        _edit_username->release();
        _edit_username->setAnchorPoint( ccp(0.5f, 0.5f));
        _edit_username->setPosition( ccp(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.7f));
        
        TextBoxEx * _edit_pass = new TextBoxEx();
        _edit_pass->initWith();
        _edit_pass->setInputFlag(kEditBoxInputFlagPassword);
        _edit_pass->setPlaceHolder("Password:");
        this->addChild(_edit_pass);
        _edit_pass->release();
        _edit_pass->setAnchorPoint( ccp(0.5f, 0.5f));
        _edit_pass->setPosition( ccp(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.5f));


        TextBoxEx * _edit_pass2 = new TextBoxEx();
        _edit_pass2->initWith();
        _edit_pass2->setInputFlag(kEditBoxInputFlagPassword);
        _edit_pass2->setPlaceHolder("Confirm Password:");
        this->addChild(_edit_pass2);
        _edit_pass2->release();
        _edit_pass2->setAnchorPoint( ccp(0.5f, 0.5f));
        _edit_pass2->setPosition( ccp(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.3f));
        
        this->_name = _edit_username;
        this->_pass = _edit_pass;
        this->_pass2 = _edit_pass2;
    }
};

class UiLogin : public cocos2d::CCLayer {
private:
    AvaFrame_Login * loginFrame;
    AvaFrame_Register * registerFrame;
    
    AvaFrame_Register* register_f;

    CCNode * nodeLoading = NULL;

    bool isHasTokenFB = false; // Thêm biến này vì là FacebookAgent trả về callback nhiều lần
public:
    UiLogin();
    virtual ~UiLogin();
    void initWith();
    void showUi();
    void requestRegister(const std::string& name, const std::string& pass, const std::string& pass2);
    void requestLogin(const std::string& name, const std::string& pass);
    //NotificationListener
//    virtual void performListener(const TNOTIFICATION& pId, cocos2d::Ref *obj);
    
    void saveAcount();
    void loadAcount();
    
//    std::function<void(UiLogin *)> _callBackLoginSucces;
    
    void increaseLoading();
    void decreaseLoading();
    
    virtual void registerWithTouchDispatcher();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void onBtnFB(CCNode* node);
    void onBtnRegisterWithFB(CCNode* node);
    
    void onLoginFB();

    void onLoginToLogin(CCNode* node);
    void onLoginToRegister(CCNode* node);
    
    void onRegisterToLogin(CCNode* node);
    void onRegisterToRegister(CCNode* node);
    
    void onResponseRegister(GoPlaySDK::IResult* result);
    void onResponseLogin(GoPlaySDK::IResult* result);
    
    void onResponseLoginByFB(GoPlaySDK::IResult* result);
    void onResponseRegisterByFB(GoPlaySDK::IResult* result);
};

#endif /* defined(__TestGame__UiLogin__) */
