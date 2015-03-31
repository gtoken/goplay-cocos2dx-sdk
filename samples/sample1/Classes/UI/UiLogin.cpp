//
//  UiLogin.cpp
//  TestGame
//
//  Created by DucHQ on 8/20/14.
//
//

#include "UiLogin.h"
#include "../GoPlaySDK/GoPlaySdk.h"
#include "UiExchange.h"
#include "UIConfig.h"

#if IS_PLATFORM_MOBILE
#include "FacebookAgent.h"
using namespace cocos2d::plugin;
#endif

#define LENGTH_INPUT_MIN 3
#define LENGTH_INPUT_MAX 20


bool checkString(std::string content, std::string name_call);

UiLogin::UiLogin(){
    _callBackLoginSucces = nullptr;
}

UiLogin::~UiLogin(){
    log("~UiLogin");
    GoPlaySDK::GoPlaySdk::GetInstance()->onLogin = nullptr;
    GoPlaySDK::GoPlaySdk::GetInstance()->onRegister = nullptr;
}

void UiLogin::initWith(){
    this->setContentSize(Director::getInstance()->getVisibleSize());
    this->showUi();
    
#if IS_PLATFORM_MOBILE
    //Add FB Button
    AvaButtonUI* fb= new AvaButtonUI();
    fb->initWithTwoPath(Sprite::create( "common/dialog_bt_01.png"), Sprite::create( "common/dialog_bt_02.png"));
    this->addChild( fb);
    fb->release();
    fb->addText( "FaceBook", 24, Color3B::WHITE, 0);
    fb->setAnchorPoint( Vec2::ANCHOR_BOTTOM_RIGHT);
    fb->setPosition( Director::getInstance()->getVisibleSize().width - 10, 10);
    
    fb->onClick = [&](AvaButton* bt){
        this->increaseLoading();
        FacebookAgent::getInstance()->login([&](int ret, std::string& msg){
            if (this->isHasTokenFB) {
                return;
            }
            log("type is %d, msg is %s", ret, msg.c_str());
            if (!FacebookAgent::getInstance()->isLoggedIn()) {
                this->decreaseLoading();
                Utilities::shareUtilities()->pushLog( "Can't login Facebook!");
                FacebookAgent::getInstance()->logout();
                return;
            }
            this->isHasTokenFB = true;

            GoPlaySDK::GoPlaySdk::GetInstance()->onLogin = [&](GoPlaySDK::IResult * r){
                CCLOG("IResult message: %s", r->getMessage().c_str());
                if (!r->getSucceeded()) {
                    if ( r->getErrorCode() == GoPlaySDK::Error::OauthUserNotExist) {
                        //Need register
                        this->setVisible(false);
                        AvaFrame_Register * register_f = new AvaFrame_Register();
                        register_f->initWith();
                        register_f->setTile("Register via Facebook");
                        Director::getInstance()->getRunningScene()->addChild( register_f);
                        register_f->release();
                        register_f->setPosition( Director::getInstance()->getVisibleSize() / 2);
                        register_f->createTextBox();
                        register_f->createBt("REGISTER", BT_TYPE::KBT_TYPE_CENTER, BT_COLOR::KBT_COLOR_GREEN, [&, register_f](AvaFrame * frame){
                            std::string name = register_f->_name->getText();
                            std::string pass = register_f->_pass->getText();
                            std::string pass2 = register_f->_pass2->getText();
                            if (!checkString(name, "Login name")) {
                                return;
                            }

                            if (!checkString(pass, "Password")) {
                                return;
                            }

                            if (pass.compare(pass2) != 0) {
                                Utilities::shareUtilities()->pushLog("Password and Confirm Password do not match!");
                                return;
                            }

                            GoPlaySDK::GoPlaySdk::GetInstance()->onRegister = [&, register_f](GoPlaySDK::IResult * r){
                                CCLOG("IResult message: %s", r->getMessage().c_str());
                                if (!r->getSucceeded()) {
                                    Utilities::shareUtilities()->pushLog( r->getMessage().c_str());
                                }else{
                                    //Go to Exchange
                                    register_f->removeFromParent();
                                    this->removeFromParent();
                                    UiExchange * uiExchange = new UiExchange();
                                    uiExchange->initWith();
                                    //uiExchange->setDebugString(r->getHttpResponse(), GoPlaySDK::URLs::ACTION_REGISTER);
                                    Director::getInstance()->getRunningScene()->addChild(uiExchange);
                                    uiExchange->release();
                                }
                            };

                            GoPlaySDK::GoPlaySdk::GetInstance()->registerWith( name, pass);
                        });
                    }else{
                        this->decreaseLoading();
                        Utilities::shareUtilities()->pushLog( r->getMessage().c_str());
                        return ;
                    }
                }else{
                    this->removeFromParent();
                    //Go to Exchange
                    UiExchange * uiExchange = new UiExchange();
                    uiExchange->initWith();
                    //uiExchange->setDebugString(r->getHttpResponse(), GoPlaySDK::URLs::ACTION_LOGIN);
                    Director::getInstance()->getRunningScene()->addChild(uiExchange);
                    uiExchange->release();
                }
            };
            GoPlaySDK::GoPlaySdk::GetInstance()->login(GoPlaySDK::SocialPlatforms::FaceBook, FacebookAgent::getInstance()->getAccessToken());
        });
    };
#endif
}

void UiLogin::showUi(){
    loginFrame = new AvaFrame_Login();
    loginFrame->initWith();
    loginFrame->setTile("Login");
    loginFrame->setPosition((this->getContentSize() - loginFrame->getContentSize()) / 2);
    
    addChild( loginFrame);
    loginFrame->release();
    
    registerFrame = new AvaFrame_Register();
    registerFrame->initWith();
    registerFrame->setTile("Register");
    addChild( registerFrame);
    registerFrame->release();
    registerFrame->setPosition((this->getContentSize() - registerFrame->getContentSize()) / 2);
    
    this->setContentSize(loginFrame->getContentSize());
    
    std::function<void(AvaFrame*)> onLoginToLogin = [&](AvaFrame* frame)
    {
        this->requestLogin(loginFrame->_name->getText(), loginFrame->_pass->getText());
    };
    
    std::function<void(AvaFrame*)> onLoginToRegister = [&](AvaFrame* frame)
    {
        loginFrame->setVisible(false);
        registerFrame->setVisible(true);
    };
    
    
    std::function<void(AvaFrame*)> onRegisterToLogin = [&](AvaFrame* frame)
    {
        loginFrame->setVisible(true);
        registerFrame->setVisible(false);
    };
    
    std::function<void(AvaFrame*)> onRegisterToRegister = [&](AvaFrame* frame)
    {
        this->requestRegister(registerFrame->_name->getText(), registerFrame->_pass->getText(), registerFrame->_pass2->getText());
    };
    
    float percenY[] = {
        0.7f,
        0.5f,
        0.3f
    };
    do {
        //CreateLoginFrame
        loginFrame
        ->setText("REGISTER if you do not have an account.", 0.28f)
        ->createBt("LOGIN", BT_TYPE::KBT_TYPE_LEFT, BT_COLOR::KBT_COLOR_GREEN, onLoginToLogin)
        ->createBt("REGISTER", BT_TYPE::KBT_TYPE_RIGHT, BT_COLOR::KBT_COLOR_BLACK, onLoginToRegister);
        loginFrame->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT);
        //
        TextBoxEx * _edit_username = new TextBoxEx();
        _edit_username->initWith();
        _edit_username->setPlaceHolder("Login name:");
        loginFrame->addChild(_edit_username);
        _edit_username->release();
        _edit_username->setAnchorPoint( Vec2::ANCHOR_MIDDLE);
        _edit_username->setPosition( Vec2(loginFrame->getContentSize().width * 0.5f, loginFrame->getContentSize().height * percenY[0]));
        
        TextBoxEx * _edit_pass = new TextBoxEx();
        _edit_pass->initWith();
        _edit_pass->setInputFlag(EditBox::InputFlag::PASSWORD);
        _edit_pass->setPlaceHolder("Password:");
        loginFrame->addChild(_edit_pass);
        _edit_pass->release();
        _edit_pass->setAnchorPoint( Vec2::ANCHOR_MIDDLE);
        _edit_pass->setPosition( Vec2(loginFrame->getContentSize().width * 0.5f, loginFrame->getContentSize().height * percenY[1]));
        
        loginFrame->_name = _edit_username;
        loginFrame->_pass = _edit_pass;
    } while (0);
    
    do {
        //CreateRegisterFrame
        registerFrame
        ->createBt("LOGIN", BT_TYPE::KBT_TYPE_LEFT, BT_COLOR::KBT_COLOR_BLACK, onRegisterToLogin)
        ->createBt("REGISTER", BT_TYPE::KBT_TYPE_RIGHT, BT_COLOR::KBT_COLOR_GREEN, onRegisterToRegister);
        registerFrame->setAnchorPoint( loginFrame->getAnchorPoint());
        registerFrame->setPosition( loginFrame->getPosition());
        registerFrame->setVisible(false);
        registerFrame->createTextBox();
    } while (0);
    //
    this->loadAcount();
}

void UiLogin::requestRegister(const std::string& name, const std::string& pass, const std::string& pass2){
    std::string username = name;
    if (!checkString(name, "Login name")) {
        return;
    }
    
    if (!checkString(pass, "Password")) {
        return;
    }
    
    if (pass.compare(pass2) != 0) {
        Utilities::shareUtilities()->pushLog("Password and Confirm Password do not match!");
        return;
    }
    
    //Save data
    if (loginFrame) {
        loginFrame->_name->setText(name.c_str());
        loginFrame->_pass->setText(pass.c_str());
    }
    this->saveAcount();
    
    GoPlaySDK::GoPlaySdk::GetInstance()->onRegister = [&](GoPlaySDK::IResult * r){
        CCLOG("IResult message: %s", r->getMessage().c_str());
        if (!r->getSucceeded()) {
            this->decreaseLoading();
            Utilities::shareUtilities()->pushLog( r->getMessage().c_str());
        }else{
            //Go to Exchange
            this->removeFromParent();
            UiExchange * uiExchange = new UiExchange();
            uiExchange->initWith();
            //uiExchange->setDebugString(r->getHttpResponse(), GoPlaySDK::URLs::ACTION_REGISTER);
            Director::getInstance()->getRunningScene()->addChild(uiExchange);
            uiExchange->release();
        }
    };
    this->saveAcount();
    this->increaseLoading();
    
    if (GoPlaySDK::GoPlaySdk::GetInstance()->getRequireOAuthData()) {
        delete GoPlaySDK::GoPlaySdk::GetInstance()->getRequireOAuthData();
        GoPlaySDK::GoPlaySdk::GetInstance()->setRequireOAuthData(nullptr);
    }
    
    GoPlaySDK::GoPlaySdk::GetInstance()->registerWith( name, pass);
}

void UiLogin::requestLogin(const std::string& name, const std::string& pass){
    if (!checkString(name, "Login name")) {
        return;
    }
    if (!checkString(pass, "Password")) {
        return;
    }
    GoPlaySDK::GoPlaySdk::GetInstance()->onLogin = [&](GoPlaySDK::IResult * r){
        CCLOG("IResult message: %s", r->getMessage().c_str());
        if (!r->getSucceeded()) {
            this->decreaseLoading();
            Utilities::shareUtilities()->pushLog( r->getMessage().c_str());
        }else{
            //Go to Exchange
            this->removeFromParent();
            UiExchange * uiExchange = new UiExchange();
            uiExchange->initWith();
            //uiExchange->setDebugString(r->getHttpResponse(), GoPlaySDK::URLs::ACTION_LOGIN);
            Director::getInstance()->getRunningScene()->addChild(uiExchange);
            uiExchange->release();
        }
    };
    this->saveAcount();
    this->increaseLoading();
    
    if (GoPlaySDK::GoPlaySdk::GetInstance()->getRequireOAuthData()) {
        delete GoPlaySDK::GoPlaySdk::GetInstance()->getRequireOAuthData();
        GoPlaySDK::GoPlaySdk::GetInstance()->setRequireOAuthData(nullptr);
    }
    
    GoPlaySDK::GoPlaySdk::GetInstance()->login( name, pass);
}

void UiLogin::saveAcount(){
    if (loginFrame) {
        CCUserDefault::getInstance()->setStringForKey("name", loginFrame->_name->getText());
        CCUserDefault::getInstance()->setStringForKey("pass", loginFrame->_pass->getText());
        CCUserDefault::getInstance()->flush();
    }
}

void UiLogin::loadAcount(){
    if (loginFrame) {
        loginFrame->_name->setText(CCUserDefault::getInstance()->getStringForKey("name", "").c_str());
        loginFrame->_pass->setText(CCUserDefault::getInstance()->getStringForKey("pass", "").c_str());
    }
}


void UiLogin::increaseLoading(){
    if (!nodeLoading) {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        nodeLoading = LayerColor::create(Color4B(0, 0, 0, 100), visibleSize.width, visibleSize.height);
        this->addChild( nodeLoading);
        
        EventListenerTouchOneByOne * listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [&](Touch* touch, Event* event) -> bool{
            if (nodeLoading->isVisible()) {
                return true;
            }
            return false;
        };
        listener->onTouchMoved = [](Touch* touch, Event* event){
        };
        listener->onTouchEnded = [](Touch* touch, Event* event){
        };
        listener->onTouchCancelled = [](Touch* touch, Event* event){
        };
        getEventDispatcher()->addEventListenerWithSceneGraphPriority( listener, nodeLoading);
    }else{
        nodeLoading->setVisible(true);
    }
    
}

void UiLogin::decreaseLoading(){
    if (nodeLoading) {
        nodeLoading->setVisible(false);
    }
}

bool checkString(std::string content, std::string name_call){
    for ( int i = 0; i < content.length(); i++) {
        char c_value = content[i];
        bool isOke = false;
        if ( c_value >= 'a'
            && c_value <= 'z') {
            isOke = true;
        }else if ( c_value >= 'A'
                  && c_value <= 'Z') {
            isOke = true;
        }else if ( c_value >= '0'
                  && c_value <= '9') {
            isOke = true;
        }
        if ( !isOke) {
            Utilities::shareUtilities()->initWithFormat("%s cannot contains special characters.", name_call.c_str());
            Utilities::shareUtilities()->pushLog(Utilities::shareUtilities()->getCString());
            return false;
        }
    }
    //Kiem tra do dai ky tu
    if(content.size() == 0){
        Utilities::shareUtilities()->initWithFormat("Fill in %s", name_call.c_str());
        Utilities::shareUtilities()->pushLog(Utilities::shareUtilities()->getCString());
        return false;
    }
    
    if(content.size() < LENGTH_INPUT_MIN || content.size() > LENGTH_INPUT_MAX){
        Utilities::shareUtilities()->initWithFormat("%s length from %d to %d characters.", name_call.c_str(), LENGTH_INPUT_MIN, LENGTH_INPUT_MAX);
        Utilities::shareUtilities()->pushLog(Utilities::shareUtilities()->getCString());
        return false;
    }
    return true;
}
