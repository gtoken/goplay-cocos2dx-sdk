//
//  UiExchange.cpp
//  GTokenSDKDemo
//
//  Created by DucHQ on 3/10/15.
//
//

#include "UiExchange.h"
#include "UIConfig.h"
#include "UiLogin.h"

#include "../GoPlaySDK/GoPlaySdk.h"
#include "../GoPlaySDK/Constants.h"

std::string remakeJsonStyle(std::string subject){
    std::string tab = "          ";
    int c_s = 0;
    for (size_t indext = 0; indext < subject.size(); indext++) {
        if (subject.at(indext) == '{') {
            c_s++;
            std::string a = "\n";
            for (int i = 0; i < c_s; i++) {
                a += tab;
            }
            subject.insert(indext + 1, a);
            indext += a.size();
        }else if (subject.at(indext) == ','){
            std::string a = "\n";
            for (int i = 0; i < c_s; i++) {
                a += tab;
            }
            subject.insert(indext + 1, a);
            indext += a.size();
        }else if (subject.at(indext) == '}'){
            c_s--;
            std::string a = "\n";
            for (int i = 0; i < c_s; i++) {
                a += tab;
            }
            subject.insert(indext, a);
            indext += a.size();
        }
    }
    return subject;
}

UiExchange::UiExchange(){
    CCLOG("new UiExchange");
}

UiExchange::~UiExchange(){
    CCLOG("~UiExchange");
}

void UiExchange::onBtnBack(cocos2d::CCNode *node)
{
    GoPlaySDK::GoPlaySdk::GetInstance()->logout();
    this->removeFromParent();
    UiLogin * mUiLogin = new UiLogin();
    mUiLogin->initWith();

    CCDirector::sharedDirector()->getRunningScene()->addChild(mUiLogin);
    mUiLogin->release();
}

void UiExchange::onBtnProfile(cocos2d::CCNode *sender)
{
    this->setDebugString("");
    GoPlaySDK::ResponseHandler* handler = new GoPlaySDK::ResponseHandler();
    handler->target = this;
    handler->callback = callfuncResult_selector(UiExchange::onResponseProfile);
    GoPlaySDK::GoPlaySdk::GetInstance()->setOnGetProfileHandler(handler);
    this->increaseLoading();
    GoPlaySDK::GoPlaySdk::GetInstance()->getProfile();
}

void UiExchange::onResponseProfile(GoPlaySDK::IResult *result)
{
    this->decreaseLoading();
    //this->setDebugString(r->getHttpResponse(), GoPlaySDK::URLs::ACTION_GET_PROFILE);
    
    GoPlaySDK::ProfileResult * profileResult = (GoPlaySDK::ProfileResult *)result;
    GoPlaySDK::UserProfile userProfile =  profileResult->getProfile();
    string parse = "Nick Name = " + userProfile.getNickName() +
    "\nContry = " + userProfile.getCountryCode() +
    "\nuserProfileEmail = " + userProfile.getNickName() ;
    this->setDebugString(parse);
}

void UiExchange::onBtnGetUnFill(cocos2d::CCNode *node)
{
    this->setDebugString("");
    
    GoPlaySDK::ResponseHandler* handler = new GoPlaySDK::ResponseHandler();
    handler->target = this;
    handler->callback = callfuncResult_selector(UiExchange::onResponseGetUnFill);
    GoPlaySDK::GoPlaySdk::GetInstance()->setOnGetUnFullFilledExchanges(handler);
    
    this->increaseLoading();
    GoPlaySDK::GoPlaySdk::GetInstance()->getUnFullFilledExchanges();
}

void UiExchange::onResponseGetUnFill(GoPlaySDK::IResult *result)
{
    this->decreaseLoading();
    
    if(result->getSucceeded() == true){
        
        GoPlaySDK::GetUnFullFilledExchangesResult * unfillResult = (GoPlaySDK::GetUnFullFilledExchangesResult *)result;
        GoPlaySDK::Exchanges exchangeArray =  unfillResult->getExchanges();
        vector<GoPlaySDK::Exchange>::iterator exchange; // declare an iterator to a vector of strings
        
        string parseArray = "";
        
        for(exchange = exchangeArray.begin() ; exchange < exchangeArray.end(); exchange++ ) {
            string token = CCString::createWithFormat("%lf", (*exchange).getGoPlayTokenValue())->getCString();
            string parse = "TransactionID = " + (*exchange).getTransactionId().ToString() +
            "\n ExchangeOptionID = " + (*exchange).getExchangeOptionId() +
            "\n ExchangeOptionType = " + GoPlaySDK::ExchangeOptionToString[(*exchange).getExchangeType()] +
            "\n Gtoken Value = " + token;
            
            parseArray += parse + "\n ========== \n";
        }
        
        this->setDebugString(parseArray);
    } else {
        this->setDebugString(result->getMessage());
    }
}

void UiExchange::onBtnRequest(cocos2d::CCNode *node)
{
    std::string content = _textBox->getText();
    for ( int i = 0; i < content.length(); i++) {
        bool isOke = true;
        
        if ( !isOke) {
            Utilities::shareUtilities()->pushLog("transaction_id cannot contains special characters.");
            return;
        }
    }
    if( content.size() == 0){
        Utilities::shareUtilities()->pushLog("Fill in transaction_id");
        return;
    }
    node->removeFromParent();
    
    GoPlaySDK::ResponseHandler* handler = new GoPlaySDK::ResponseHandler();
    handler->target = this;
    handler->callback = callfuncResult_selector(UiExchange::onResponseFullFillExchange);
    GoPlaySDK::GoPlaySdk::GetInstance()->setOnFullFillExchange(handler);
    this->increaseLoading();
    
    GoPlaySDK::GoPlaySdk::GetInstance()->fullFillExchange(content);
}

void UiExchange::onResponseFullFillExchange(GoPlaySDK::IResult *result)
{
    this->decreaseLoading();
    //this->setDebugString(r->getHttpResponse(), GoPlaySDK::URLs::ACTION_FULLFILL_EXCHANGE);
    
    if(result->getSucceeded() == true){
        GoPlaySDK::FullFillExchangeResult * fullfillResult = (GoPlaySDK::FullFillExchangeResult *)result;
        GoPlaySDK::Exchange exchange =  fullfillResult->getExchange();
        
        string token = CCString::createWithFormat("%lf", exchange.getGoPlayTokenValue())->getCString();
        string parse = "TransactionID = " + exchange.getTransactionId().ToString() +
        "\n ExchangeOptionID = " + exchange.getExchangeOptionId() +
        "\n ExchangeOptionType = " + GoPlaySDK::ExchangeOptionToString[exchange.getExchangeType()] +
        "\n Gtoken Value = " + token;
        this->setDebugString(parse);
    }else{
        this->setDebugString(result->getMessage());
    }

}

void UiExchange::onBtnFullFill(cocos2d::CCNode *node)
{
    this->setDebugString("");
    
    CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
    
    AvaFrame * input = new AvaFrame();
    input->initWith();
    input->createTouchTouchOutToRemove();
    input->setTile("/transaction/fulfill-exchange");
    input->setPosition(ccp(winSize.width/2.0f, winSize.height/2.0f));
    this->addChild(input);
    input->release();
    
    TextBoxEx * tb = new TextBoxEx();
    tb->initWith();
    tb->setPlaceHolder("transaction_id");
    input->addChild(tb);
    tb->release();
    tb->setAnchorPoint(ANCHOR_MIDDLE);
    tb->setPosition(ccp(input->getContentSize().width * 0.5f, input->getContentSize().height * 0.5f));
    _textBox = tb;
    
    input->createBt("Request",
                    this,
                    callfuncN_selector(UiExchange::onBtnRequest),
                    KBT_TYPE_CENTER,
                    KBT_COLOR_BLACK);
}

void UiExchange::onBtnUpdateExternal(cocos2d::CCNode *node)
{
    this->setDebugString("");
    
    CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
    
    AvaFrame * input = new AvaFrame();
    input->initWith();
    input->createTouchTouchOutToRemove();
    input->setTile("/transaction/update-external-exchange");
    input->setPosition(ccp(winSize.width/2.0f, winSize.height/2.0f));
    this->addChild(input);
    input->release();
    
    TextBoxEx * tb = new TextBoxEx();
    tb->initWith();
    tb->setPlaceHolder("transaction_id");
    input->addChild(tb);
    tb->release();
    tb->setAnchorPoint(ANCHOR_MIDDLE);
    tb->setPosition(ccp(input->getContentSize().width * 0.5f, input->getContentSize().height * 0.6f));
    _updateTex = tb;
    
    TextBoxEx * tb2 = new TextBoxEx();
    tb2->initWith();
    tb2->setPlaceHolder("exchange_option_identifier");
    input->addChild(tb2);
    tb2->release();
    tb2->setAnchorPoint(ANCHOR_MIDDLE);
    tb2->setPosition(ccp(input->getContentSize().width * 0.5f, input->getContentSize().height * 0.34f));
    _updateTex2 = tb2;
    
    input->createBt("Request",
                    this,
                    callfuncN_selector(UiExchange::onBtnRequestUpdateExternal),
                    KBT_TYPE_CENTER,
                    KBT_COLOR_BLACK);
}

void UiExchange::onBtnRequestUpdateExternal(cocos2d::CCNode *node)
{

    std::string content = _updateTex->getText();
    for ( int i = 0; i < content.length(); i++) {
        bool isOke = true;
        
        if ( !isOke) {
            Utilities::shareUtilities()->initWithFormat("transaction_id cannot contains special characters.");
            Utilities::shareUtilities()->pushLog(Utilities::shareUtilities()->getCString());
            return;
        }
    }
    if( content.size() == 0){
        Utilities::shareUtilities()->initWithFormat("Fill in transaction_id");
        Utilities::shareUtilities()->pushLog(Utilities::shareUtilities()->getCString());
        return;
    }
    std::string content2 = _updateTex2->getText();
    for ( int i = 0; i < content2.length(); i++) {
        bool isOke = true;
        
        if ( !isOke) {
            Utilities::shareUtilities()->initWithFormat("exchange_option_identifier cannot contains special characters.");
            Utilities::shareUtilities()->pushLog(Utilities::shareUtilities()->getCString());
            return;
        }
    }
    if( content2.size() == 0){
        Utilities::shareUtilities()->initWithFormat("Fill in exchange_option_identifier");
        Utilities::shareUtilities()->pushLog(Utilities::shareUtilities()->getCString());
        return;
    }
    node->removeFromParent();
    
    GoPlaySDK::ResponseHandler* handler = new GoPlaySDK::ResponseHandler();
    handler->target = this;
    handler->callback = callfuncResult_selector(UiExchange::onResponseUpdateExternal);
    GoPlaySDK::GoPlaySdk::GetInstance()->setOnUpdateExternalExchange(handler);
    
    this->increaseLoading();
    GoPlaySDK::GoPlaySdk::GetInstance()->updateExternalExchange(content.c_str(), content2.c_str());

}

void UiExchange::onResponseUpdateExternal(GoPlaySDK::IResult *result)
{
    this->decreaseLoading();
    
    if(result->getSucceeded() == false){
        this->setDebugString(result->getMessage().c_str());
    }else {
        this->setDebugString("Successful!");
    }
}

void UiExchange::onBtnDisOauth(cocos2d::CCNode *node)
    {
    if (!GoPlaySDK::GoPlaySdk::GetInstance()->getRequireOAuthData()) {
        Utilities::shareUtilities()->pushLog("Your acount not has connectOAuth");
        return;
    }
    this->setDebugString("");

    GoPlaySDK::GoPlaySdk::GetInstance()->unBindOauth(GoPlaySDK::GoPlaySdk::GetInstance()->getRequireOAuthData()->getPlatform(),
                                                     GoPlaySDK::GoPlaySdk::GetInstance()->getRequireOAuthData()->getToken()
                                                     );
}

void UiExchange::initWith(){
    this->setContentSize(CCDirector::sharedDirector()->getVisibleSize());
    
    float x = 200.0f;
    float y = 50.0f;
    
    this->createBt("", 50.0f, this->getContentSize().height - y,
                   this,
                   callfuncN_selector(UiExchange::onBtnBack),
                   "common/buttons_back.png",
                   "common/buttons_back_selected.png");
    
    this->createBt("profile", x, this->getContentSize().height - y, this, callfuncN_selector(UiExchange::onBtnProfile));
    
    this->createBt("get-unfulfilled", x + 170, this->getContentSize().height - y, this, callfuncN_selector(UiExchange::onBtnGetUnFill));
    
    this->createBt("fulfill", x + 170 * 2, this->getContentSize().height - y, this, callfuncN_selector(UiExchange::onBtnFullFill));
    
    
    this->createBt("update-external", x + 170 * 3, this->getContentSize().height - y, this, callfuncN_selector(UiExchange::onBtnUpdateExternal));
    
    this->createBt("dis-oauth", x + 170 * 4, this->getContentSize().height - y, this, callfuncN_selector(UiExchange::onBtnDisOauth));
}

void UiExchange::setDebugString( std::string s, std::string sub){
    std::string s_debug = remakeJsonStyle(s);
    if (!sub.empty()) {
        s_debug.insert(0, "\n");
        s_debug.insert(0, sub);
    }
    if (!logTest) {
        logTest = CCLabelTTF::create(s_debug.c_str(), s_pPathFontCommonBold, 20, CCSize(this->getContentSize().width, 0), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
        logTest->setAnchorPoint(ANCHOR_TOP_LEFT);
        this->addChild( logTest);
        logTest->setPositionX( 100);
        logTest->setPositionY( this->getContentSize().height - 80);
    }else{
        logTest->setString(s_debug.c_str());
    }
}

void UiExchange::createBt(const string& t, const float& x, const float& y, CCObject* target, SEL_CallFuncN onClick, const char* s_bt_1, const char* s_bt_2){
    AvaButtonUI* bt= new AvaButtonUI();
    bt->initWithTwoPath(CCSprite::create( s_bt_1), CCSprite::create( s_bt_2));
    this->addChild( bt);
    bt->release();
    if (t.size() > 0) {
        bt->addText( t, 21, ccWHITE, 0);
    }
    bt->setAnchorPoint(ANCHOR_TOP_LEFT);
    bt->setPosition( x, y);
    bt->setEventListeners(target, onClick, NULL, NULL, NULL, NULL);
}

void UiExchange::increaseLoading() {
    if (!nodeLoading) {
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        nodeLoading = CCLayerColor::create(ccc4(0, 0, 0, 100), visibleSize.width, visibleSize.height);
        this->addChild( nodeLoading);
        
        // enable touch
        setTouchEnabled(true);
        
    }else{
        nodeLoading->setVisible(true);
    }
    
}

void UiExchange::decreaseLoading(){
    if (nodeLoading) {
        nodeLoading->setVisible(false);
    }
}

void UiExchange::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), true);
}

bool UiExchange::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (nodeLoading->isVisible()) {
        return true;
    }
    return false;
}

void UiExchange::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
}

void UiExchange::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
}