//
//  UiExchange.cpp
//  GTokenSDKDemo
//
//  Created by DucHQ on 3/10/15.
//
//

#include "UiExchange.h"
#include "UiLogin.h"
#include "../GoPlaySDK/GoPlaySdk.h"
#include "UIConfig.h"
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
    log("new UiExchange");
}

UiExchange::~UiExchange(){
    log("~UiExchange");
}

void UiExchange::initWith(){
    this->setContentSize(Director::getInstance()->getVisibleSize());
    this->createBt("", 3.0f, this->getContentSize().height - 3,
                   [&](AvaButton* bt){
                       GoPlaySDK::GoPlaySdk::GetInstance()->logout();
                       this->removeFromParent();
                       UiLogin * mUiLogin = new UiLogin();
                       mUiLogin->initWith();
                       Director::getInstance()->getRunningScene()->addChild(mUiLogin);
                       mUiLogin->release();
                   },
                   "common/buttons_back.png",
                   "common/buttons_back_selected.png");
    
    this->createBt("profile", 100.0f, this->getContentSize().height - 10,
                   [&](AvaButton* bt){
                       this->setDebugString("");
                       GoPlaySDK::GoPlaySdk::GetInstance()->onGetProfile = [&](GoPlaySDK::IResult * r){
                           this->decreaseLoading();
                           //this->setDebugString(r->getHttpResponse(), GoPlaySDK::URLs::ACTION_GET_PROFILE);
                           
                           GoPlaySDK::ProfileResult * profileResult = (GoPlaySDK::ProfileResult *)r;
                           GoPlaySDK::UserProfile userProfile =  profileResult->getProfile();
                           string parse = "Nick Name = " + userProfile.getNickName() +
                           "\nContry = " + userProfile.getCountryCode() +
                           "\nuserProfileEmail = " + userProfile.getNickName() ;
                           this->setDebugString(parse);
                           
                       };
                       this->increaseLoading();
                       GoPlaySDK::GoPlaySdk::GetInstance()->getProfile();
                   });
    
    this->createBt("get-unfulfilled", 100.0f + 170, this->getContentSize().height - 10,
                   [&](AvaButton* bt){
                       this->setDebugString("");
                       GoPlaySDK::GoPlaySdk::GetInstance()->onGetUnFullFilledExchanges = [&](GoPlaySDK::IResult * r){
                           this->decreaseLoading();
                           
                           if(r->getSucceeded() == true){
                               
                               GoPlaySDK::GetUnFullFilledExchangesResult * unfillResult = (GoPlaySDK::GetUnFullFilledExchangesResult *)r;
                               GoPlaySDK::Exchanges exchangeArray =  unfillResult->getExchanges();
                               vector<GoPlaySDK::Exchange>::iterator exchange; // declare an iterator to a vector of strings
                               
                               string parseArray = "";
                               
                               for(exchange = exchangeArray.begin() ; exchange < exchangeArray.end(); exchange++ ) {
                                   string parse = "TransactionID = " + (*exchange).getTransactionId().ToString() +
                                   "\n ExchangeOptionID = " + (*exchange).getExchangeOptionId() +
                                   "\n ExchangeOptionType = " + GoPlaySDK::ExchangeOptionToString[(*exchange).getExchangeType()] +
                                   "\n Gtoken Value = " + StringUtils::format("%lf", (*exchange).getGoPlayTokenValue()).c_str() ;
                                   
                                   parseArray += parse + "\n ========== \n";
                               }
                               
                               this->setDebugString(parseArray);
                           }else{
                               this->setDebugString(r->getMessage());
                           }
                           
                       };
                       this->increaseLoading();
                       GoPlaySDK::GoPlaySdk::GetInstance()->getUnFullFilledExchanges();
                   });
    
    this->createBt("fulfill", 100.0f + 170 * 2, this->getContentSize().height - 10,
                   [&](AvaButton* bt){
                       this->setDebugString("");
                       
                       AvaFrame * input = new AvaFrame();
                       input->initWith();
                       input->createTouchTouchOutToRemove();
                       input->setTile("/transaction/fulfill-exchange");
                       input->setPosition( Director::getInstance()->getVisibleSize() / 2);
                       this->addChild(input);
                       input->release();
                       
                       TextBoxEx * tb = new TextBoxEx();
                       tb->initWith();
                       tb->setPlaceHolder("transaction_id");
                       input->addChild(tb);
                       tb->release();
                       tb->setAnchorPoint( Vec2::ANCHOR_MIDDLE);
                       tb->setPosition( Vec2(input->getContentSize().width * 0.5f, input->getContentSize().height * 0.5f));
                       //tb->setText("553c75bd-c32d-41a2-8b38-c8623a668098");
                       
                       input->createBt("Request",
                                       BT_TYPE::KBT_TYPE_CENTER,
                                       BT_COLOR::KBT_COLOR_BLACK,
                                       [&, tb](AvaFrame* frame){
                                           std::string content = tb->getText();
                                           for ( int i = 0; i < content.length(); i++) {
                                               char c_value = content[i];
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
                                           frame->removeFromParent();
                                           
                                           GoPlaySDK::GoPlaySdk::GetInstance()->onFullFillExchange = [&](GoPlaySDK::IResult * r){
                                               this->decreaseLoading();
                                               //this->setDebugString(r->getHttpResponse(), GoPlaySDK::URLs::ACTION_FULLFILL_EXCHANGE);
                                               
                                               if(r->getSucceeded() == true){
                                                   
                                                   GoPlaySDK::FullFillExchangeResult * fullfillResult = (GoPlaySDK::FullFillExchangeResult *)r;
                                                   GoPlaySDK::Exchange exchange =  fullfillResult->getExchange();
                                                   string parse = "TransactionID = " + exchange.getTransactionId().ToString() +
                                                   "\n ExchangeOptionID = " + exchange.getExchangeOptionId() +
                                                   "\n ExchangeOptionType = " + GoPlaySDK::ExchangeOptionToString[exchange.getExchangeType()] +
                                                   "\n Gtoken Value = " + StringUtils::format("%lf", exchange.getGoPlayTokenValue()).c_str() ;
                                                   this->setDebugString(parse);
                                               }else{
                                                   this->setDebugString(r->getMessage());
                                               }
                                               
                                           };
                                           this->increaseLoading();

                                           GoPlaySDK::GoPlaySdk::GetInstance()->fullFillExchange(content);
                                       });
                   });
    
    
    this->createBt("update-external", 100.0f + 170 * 3, this->getContentSize().height - 10,
                   [&](AvaButton* bt){
                       this->setDebugString("");
                       
                       AvaFrame * input = new AvaFrame();
                       input->initWith();
                       input->createTouchTouchOutToRemove();
                       input->setTile("/transaction/update-external-exchange");
                       input->setPosition( Director::getInstance()->getVisibleSize() / 2);
                       this->addChild(input);
                       input->release();
                       
                       TextBoxEx * tb = new TextBoxEx();
                       tb->initWith();
                       tb->setPlaceHolder("transaction_id");
                       input->addChild(tb);
                       tb->release();
                       tb->setAnchorPoint( Vec2::ANCHOR_MIDDLE);
                       tb->setPosition( Vec2(input->getContentSize().width * 0.5f, input->getContentSize().height * 0.6f));
                       
                       TextBoxEx * tb2 = new TextBoxEx();
                       tb2->initWith();
                       tb2->setPlaceHolder("exchange_option_identifier");
                       input->addChild(tb2);
                       tb2->release();
                       tb2->setAnchorPoint( Vec2::ANCHOR_MIDDLE);
                       tb2->setPosition( Vec2(input->getContentSize().width * 0.5f, input->getContentSize().height * 0.34f));
                       
                       input->createBt("Request",
                                       BT_TYPE::KBT_TYPE_CENTER,
                                       BT_COLOR::KBT_COLOR_BLACK,
                                       [&, tb, tb2](AvaFrame* frame){
                                           std::string content = tb->getText();
                                           for ( int i = 0; i < content.length(); i++) {
                                               char c_value = content[i];
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
                                           std::string content2 = tb2->getText();
                                           for ( int i = 0; i < content2.length(); i++) {
                                               char c_value = content2[i];
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
                                           frame->removeFromParent();
                                           GoPlaySDK::GoPlaySdk::GetInstance()->onUpdateExternalExchange = [&](GoPlaySDK::IResult * r){
                                               this->decreaseLoading();
                                               
                                               if(r->getSucceeded() == false){
                                                   this->setDebugString(r->getMessage().c_str());
                                               }else {
                                                   this->setDebugString("Successful!");
                                               }
                                           };
                                           this->increaseLoading();
                                           GoPlaySDK::GoPlaySdk::GetInstance()->updateExternalExchange(content.c_str(), content2.c_str());
                                       });
                   });
    
    this->createBt("dis-oauth", 100.0f + 170 * 4, this->getContentSize().height - 10,
                   [&](AvaButton* bt){
                       if (!GoPlaySDK::GoPlaySdk::GetInstance()->getRequireOAuthData()) {
                           Utilities::shareUtilities()->pushLog("Your acount not has connectOAuth");
                           return;
                       }
                       this->setDebugString("");
                   
                       GoPlaySDK::GoPlaySdk::GetInstance()->unBindOauth(GoPlaySDK::GoPlaySdk::GetInstance()->getRequireOAuthData()->getPlatform(),
                                                                     GoPlaySDK::GoPlaySdk::GetInstance()->getRequireOAuthData()->getToken()
                                                                     );
                       
                   });
}

void UiExchange::setDebugString( std::string s, std::string sub){
    std::string s_debug = remakeJsonStyle(s);
    if (!sub.empty()) {
        s_debug.insert(0, "\n");
        s_debug.insert(0, sub);
    }
    if (!logTest) {
        logTest = Label::createWithTTF( s_debug, s_pPathFontCommonBold, 20, Size( this->getContentSize().width, 0), TextHAlignment::LEFT, TextVAlignment::TOP);
        logTest->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT);
        this->addChild( logTest);
        logTest->setPositionX( 100);
        logTest->setPositionY( this->getContentSize().height - 80);
    }else{
        logTest->setString(s_debug);
    }
}

void UiExchange::createBt(const string& t, const float& x, const float& y, std::function<void(AvaButton*)> onClick, const char* s_bt_1, const char* s_bt_2){
    AvaButtonUI* bt= new AvaButtonUI();
    bt->initWithTwoPath(Sprite::create( s_bt_1), Sprite::create( s_bt_2));
    this->addChild( bt);
    bt->release();
    if (t.size() > 0) {
        bt->addText( t, 21, Color3B::WHITE, 0);
    }
    bt->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT);
    bt->setPosition( x, y);
    bt->onClick = onClick;
}

void UiExchange::increaseLoading(){
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

void UiExchange::decreaseLoading(){
    if (nodeLoading) {
        nodeLoading->setVisible(false);
    }
}
