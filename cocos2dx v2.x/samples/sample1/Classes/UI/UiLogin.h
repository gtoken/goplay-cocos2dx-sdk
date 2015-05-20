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

class AvaFrame_Login : public AvaFrame{
public:
    TextBoxEx * _name;
    TextBoxEx * _pass;
public:
    AvaFrame_Login(){
        _name = nullptr;
        _pass = nullptr;
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
        _name = nullptr;
        _pass = nullptr;
        _pass2 = nullptr;
    }
    
    virtual ~AvaFrame_Register(){

    }
    
    void createTextBox(){
        TextBoxEx * _edit_username = new TextBoxEx();
        _edit_username->initWith();
        _edit_username->setPlaceHolder("Login name:");
        this->addChild(_edit_username);
        _edit_username->release();
        _edit_username->setAnchorPoint( Vec2::ANCHOR_MIDDLE);
        _edit_username->setPosition( Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.7f));
        
        TextBoxEx * _edit_pass = new TextBoxEx();
        _edit_pass->initWith();
        _edit_pass->setInputFlag(EditBox::InputFlag::PASSWORD);
        _edit_pass->setPlaceHolder("Password:");
        this->addChild(_edit_pass);
        _edit_pass->release();
        _edit_pass->setAnchorPoint( Vec2::ANCHOR_MIDDLE);
        _edit_pass->setPosition( Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.5f));


        TextBoxEx * _edit_pass2 = new TextBoxEx();
        _edit_pass2->initWith();
        _edit_pass2->setInputFlag(EditBox::InputFlag::PASSWORD);
        _edit_pass2->setPlaceHolder("Confirm Password:");
        this->addChild(_edit_pass2);
        _edit_pass2->release();
        _edit_pass2->setAnchorPoint( Vec2::ANCHOR_MIDDLE);
        _edit_pass2->setPosition( Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.3f));
        
        this->_name = _edit_username;
        this->_pass = _edit_pass;
        this->_pass2 = _edit_pass2;
    }
};

class UiLogin : public cocos2d::Node{
private:
    AvaFrame_Login * loginFrame;
    AvaFrame_Register * registerFrame;

    Node * nodeLoading = nullptr;

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
    
    std::function<void(UiLogin *)> _callBackLoginSucces;
    
    void increaseLoading();
    void decreaseLoading();
};

#endif /* defined(__TestGame__UiLogin__) */
