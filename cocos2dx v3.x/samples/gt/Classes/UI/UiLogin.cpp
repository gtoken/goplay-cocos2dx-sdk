//
//  UiLogin.cpp
//  TestGame
//
//  Created by DucHQ on 8/20/14.
//
//

#include "UiLogin.h"
#include "UiExchange.h"
#include "UIConfig.h"

#include "FacebookAgent.h"

#define LENGTH_INPUT_MIN 3
#define LENGTH_INPUT_MAX 20


bool checkString(std::string content, std::string name_call);

UiLogin::UiLogin(){
//    _callBackLoginSucces = nullptr;
}

UiLogin::~UiLogin(){
    CCLOG("~UiLogin");
    GoPlaySDK::GoPlaySdk::GetInstance()->setOnLoginHandler(NULL);
    GoPlaySDK::GoPlaySdk::GetInstance()->setOnRegisterHandler(NULL);
}

void UiLogin::onResponseRegisterByFB(GoPlaySDK::IResult *result)
{
    CCLOG("IResult message: %s", result->getMessage().c_str());
    if (!result->getSucceeded()) {
        Utilities::shareUtilities()->pushLog( result->getMessage().c_str());
    }else{
        //Go to Exchange
        register_f->removeFromParent();
        this->removeFromParent();
        UiExchange * uiExchange = new UiExchange();
        uiExchange->initWith();
        //uiExchange->setDebugString(r->getHttpResponse(), GoPlaySDK::URLs::ACTION_REGISTER);
        CCDirector::sharedDirector()->getRunningScene()->addChild(uiExchange);
        uiExchange->release();
    }
}

void UiLogin::onBtnRegisterWithFB(CCNode* node)
{
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
    
    GoPlaySDK::ResponseHandler* handler = new GoPlaySDK::ResponseHandler();
    handler->target = this;
    handler->callback = callfuncResult_selector(UiLogin::onResponseRegisterByFB);
    GoPlaySDK::GoPlaySdk::GetInstance()->setOnRegisterHandler(handler);
    
    GoPlaySDK::GoPlaySdk::GetInstance()->registerWith( name, pass);
}

void UiLogin::onResponseLoginByFB(GoPlaySDK::IResult* result)
{
    CCLOG("IResult message: %s", result->getMessage().c_str());
    if (!result->getSucceeded()) {
        if ( result->getErrorCode() == GoPlaySDK::ErrorOauthUserNotExist) {

            // disable touch
            setTouchEnabled(false);

            //Need register
            this->setVisible(false);
            register_f = new AvaFrame_Register();
            register_f->initWith();
            register_f->setTile("Register via Facebook");
            CCDirector::sharedDirector()->getRunningScene()->addChild( register_f);
            register_f->release();
            
            CCSize size = CCDirector::sharedDirector()->getVisibleSize();
            
            register_f->setPosition(ccp(size.width/2.0f, size.height/2.0f));
            register_f->createTextBox();
            register_f->createBt("REGISTER", this, callfuncN_selector(UiLogin::onBtnRegisterWithFB), KBT_TYPE_CENTER,KBT_COLOR_GREEN);
        } else {
            this->decreaseLoading();
            Utilities::shareUtilities()->pushLog(result->getMessage().c_str());
            return ;
        }
    } else {
        this->removeFromParent();
        //Go to Exchange
        UiExchange * uiExchange = new UiExchange();
        uiExchange->initWith();
        //uiExchange->setDebugString(r->getHttpResponse(), GoPlaySDK::URLs::ACTION_LOGIN);
        CCDirector::sharedDirector()->getRunningScene()->addChild(uiExchange);
        uiExchange->release();
    }
}

void UiLogin::onLoginFB()
{
    if (this->isHasTokenFB) {
        return;
    }
    if (!FacebookAgent::getInstance()->isLoggedIn()) {
        this->decreaseLoading();
        Utilities::shareUtilities()->pushLog( "Can't login Facebook!");
        FacebookAgent::getInstance()->logout();
        return;
    }
    this->isHasTokenFB = true;
    
    GoPlaySDK::ResponseHandler* handler = new GoPlaySDK::ResponseHandler();
    handler->target = this;
    handler->callback = callfuncResult_selector(UiLogin::onResponseLoginByFB);
    GoPlaySDK::GoPlaySdk::GetInstance()->setOnLoginHandler(handler);
    
    GoPlaySDK::GoPlaySdk::GetInstance()->login(GoPlaySDK::SocialPlatformFaceBook, FacebookAgent::getInstance()->getAccessToken());
}

void UiLogin::onBtnFB(cocos2d::CCNode *node)
{
    this->increaseLoading();
    FacebookAgent::getInstance()->login(this, callfunc_selector(UiLogin::onLoginFB));
}

void UiLogin::initWith(){
    this->setContentSize(CCDirector::sharedDirector()->getVisibleSize());
    this->showUi();
    
    //Add FB Button
    AvaButtonUI* fb= new AvaButtonUI();
    fb->initWithTwoPath(CCSprite::create( "common/dialog_bt_01.png"), CCSprite::create( "common/dialog_bt_02.png"));
    this->addChild( fb);
    fb->release();
    fb->addText( "FaceBook", 24, ccWHITE, 0);
    fb->setAnchorPoint( ANCHOR_BOTTOM_RIGHT);
    fb->setPosition( CCDirector::sharedDirector()->getVisibleSize().width - 100, 50);
    fb->setEventListeners(this, callfuncN_selector(UiLogin::onBtnFB), NULL, NULL, NULL, NULL);
}

void UiLogin::onLoginToLogin(cocos2d::CCNode *node)
{
    this->requestLogin(loginFrame->_name->getText(), loginFrame->_pass->getText());
}

void UiLogin::onLoginToRegister(cocos2d::CCNode *node)
{
    loginFrame->setVisible(false);
    registerFrame->setVisible(true);
}

void UiLogin::onRegisterToLogin(cocos2d::CCNode *node)
{
    loginFrame->setVisible(true);
    registerFrame->setVisible(false);
}

void UiLogin::onRegisterToRegister(cocos2d::CCNode *node)
{
    this->requestRegister(registerFrame->_name->getText(), registerFrame->_pass->getText(), registerFrame->_pass2->getText());
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
    
    float percenY[] = {
        0.7f,
        0.5f,
        0.3f
    };
    do {
        //CreateLoginFrame
        loginFrame
        ->setText("REGISTER if you do not have an account.", 0.28f)
        ->createBt("LOGIN", this, callfuncN_selector(UiLogin::onLoginToLogin), KBT_TYPE_LEFT, KBT_COLOR_GREEN)
        ->createBt("REGISTER", this, callfuncN_selector(UiLogin::onLoginToRegister), KBT_TYPE_RIGHT,KBT_COLOR_BLACK);
        loginFrame->setAnchorPoint(ANCHOR_BOTTOM_LEFT);
        //
        TextBoxEx * _edit_username = new TextBoxEx();
        _edit_username->initWith();
        _edit_username->setPlaceHolder("Login name:");
        loginFrame->addChild(_edit_username);
        _edit_username->release();
        _edit_username->setAnchorPoint( ANCHOR_MIDDLE);
        _edit_username->setPosition( ccp(loginFrame->getContentSize().width * 0.5f, loginFrame->getContentSize().height * percenY[0]));
        
        TextBoxEx * _edit_pass = new TextBoxEx();
        _edit_pass->initWith();
        _edit_pass->setInputFlag(kEditBoxInputFlagPassword);
        _edit_pass->setPlaceHolder("Password:");
        loginFrame->addChild(_edit_pass);
        _edit_pass->release();
        _edit_pass->setAnchorPoint( ANCHOR_MIDDLE);
        _edit_pass->setPosition( ccp(loginFrame->getContentSize().width * 0.5f, loginFrame->getContentSize().height * percenY[1]));
        
        loginFrame->_name = _edit_username;
        loginFrame->_pass = _edit_pass;
    } while (0);
    
    do {
        //CreateRegisterFrame
        registerFrame
        ->createBt("LOGIN", this, callfuncN_selector(UiLogin::onRegisterToLogin), KBT_TYPE_LEFT, KBT_COLOR_BLACK)
        ->createBt("REGISTER", this, callfuncN_selector(UiLogin::onRegisterToRegister), KBT_TYPE_RIGHT, KBT_COLOR_GREEN);
        registerFrame->setAnchorPoint( loginFrame->getAnchorPoint());
        registerFrame->setPosition( loginFrame->getPosition());
        registerFrame->setVisible(false);
        registerFrame->createTextBox();
    } while (0);
    //
    this->loadAcount();
}

void UiLogin::onResponseRegister(GoPlaySDK::IResult *result)
{
    CCLOG("IResult message: %s", result->getMessage().c_str());
    if (!result->getSucceeded()) {
        this->decreaseLoading();
        Utilities::shareUtilities()->pushLog( result->getMessage().c_str());
    }else{
        //Go to Exchange
        this->removeFromParent();
        UiExchange * uiExchange = new UiExchange();
        uiExchange->initWith();
        //uiExchange->setDebugString(r->getHttpResponse(), GoPlaySDK::URLs::ACTION_REGISTER);
        CCDirector::sharedDirector()->getRunningScene()->addChild(uiExchange);
        uiExchange->release();
    }
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
    
    GoPlaySDK::ResponseHandler* handler = new GoPlaySDK::ResponseHandler();
    handler->target = this;
    handler->callback = callfuncResult_selector(UiLogin::onResponseRegister);
    
    GoPlaySDK::GoPlaySdk::GetInstance()->setOnRegisterHandler(handler);
    this->saveAcount();
    this->increaseLoading();
    
    if (GoPlaySDK::GoPlaySdk::GetInstance()->getRequireOAuthData()) {
        delete GoPlaySDK::GoPlaySdk::GetInstance()->getRequireOAuthData();
        GoPlaySDK::GoPlaySdk::GetInstance()->setRequireOAuthData(NULL);
    }
    
    GoPlaySDK::GoPlaySdk::GetInstance()->registerWith( name, pass);
}

void UiLogin::onResponseLogin(GoPlaySDK::IResult *result)
{
    CCLOG("IResult message: %s", result->getMessage().c_str());
    if (!result->getSucceeded()) {
        this->decreaseLoading();
        Utilities::shareUtilities()->pushLog( result->getMessage().c_str());
    }else{
        //Go to Exchange
        this->removeFromParent();
        UiExchange * uiExchange = new UiExchange();
        uiExchange->initWith();
        //uiExchange->setDebugString(r->getHttpResponse(), GoPlaySDK::URLs::ACTION_LOGIN);
        CCDirector::sharedDirector()->getRunningScene()->addChild(uiExchange);
        uiExchange->release();
    }
}

void UiLogin::requestLogin(const std::string& name, const std::string& pass){
    if (!checkString(name, "Login name")) {
        return;
    }
    if (!checkString(pass, "Password")) {
        return;
    }
    
    GoPlaySDK::ResponseHandler* handler = new GoPlaySDK::ResponseHandler();
    handler->target = this;
    handler->callback = callfuncResult_selector(UiLogin::onResponseLogin);
    
    GoPlaySDK::GoPlaySdk::GetInstance()->setOnLoginHandler(handler);
    this->saveAcount();
    this->increaseLoading();
    
    if (GoPlaySDK::GoPlaySdk::GetInstance()->getRequireOAuthData()) {
        delete GoPlaySDK::GoPlaySdk::GetInstance()->getRequireOAuthData();
        GoPlaySDK::GoPlaySdk::GetInstance()->setRequireOAuthData(NULL);
    }
    
    GoPlaySDK::GoPlaySdk::GetInstance()->login( name, pass);
}

void UiLogin::saveAcount(){
    if (loginFrame) {
        CCUserDefault::sharedUserDefault()->setStringForKey("name", loginFrame->_name->getText());
        CCUserDefault::sharedUserDefault()->setStringForKey("pass", loginFrame->_pass->getText());
        CCUserDefault::sharedUserDefault()->flush();
    }
}

void UiLogin::loadAcount(){
    if (loginFrame) {
        loginFrame->_name->setText(CCUserDefault::sharedUserDefault()->getStringForKey("name", "").c_str());
        loginFrame->_pass->setText(CCUserDefault::sharedUserDefault()->getStringForKey("pass", "").c_str());
    }
}


void UiLogin::increaseLoading(){
    if (!nodeLoading) {
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        nodeLoading = CCLayerColor::create(ccc4(0, 0, 0, 100), visibleSize.width, visibleSize.height);
        this->addChild( nodeLoading);
    }else{
        nodeLoading->setVisible(true);
    }

    setTouchEnabled(true);    
}

bool UiLogin::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (nodeLoading->isVisible()) {
        return true;
    }
    return false;
}

void UiLogin::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), true);
}

void UiLogin::decreaseLoading(){
    if (nodeLoading) {
        nodeLoading->setVisible(false);
        setTouchEnabled(false);
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
