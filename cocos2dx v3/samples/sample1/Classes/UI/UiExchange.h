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
#include <string>

using namespace std;

class UiExchange : public cocos2d::Node{
private:
    Label * logTest = nullptr;
    Node * nodeLoading = nullptr;
public:
    UiExchange();
    virtual ~UiExchange();
    void initWith();
    void setDebugString( std::string s, std::string sub = "");
private:
    void createBt(const string& t, const float& x, const float& y, std::function<void(AvaButton*)> onClick, const char* s_bt_1 = "common/dialog_bt_01.png", const char* s_bt_2 = "common/dialog_bt_02.png");
    
    void increaseLoading();
    void decreaseLoading();
};

#endif /* defined(__GTokenSDKDemo__UiExchange__) */
