//
//  AvaFrame.h
//  DotA
//
//  Created by DucHQ on 5/26/14.
//
//

#ifndef __DotA__AvaFrame__
#define __DotA__AvaFrame__

#include "AvaButton.h"

//class CaptureScreen : public cocos2d::CCObject {
//public:
//    static void create(std::function<void(Texture2D * texture)> afterCaptured);
//protected:
//    CaptureScreen();
//    virtual ~CaptureScreen();
//    void initWith(std::function<void(Texture2D * texture)> onCaptureCallback);
//private:
//    void onCaptureScreen(std::function<void(Texture2D * texture)>& afterCaptured);
//private:
//    std::function<void(Texture2D * texture)> _onCaptureCallback;
//    Texture2D * _texture_cache;
//    void onCallBack(float dt);
//};
//
//class CaptureScreenImage : public cocos2d::Ref{
//public:
//    static void create(std::function<void(Image * image)> afterCaptured);
//protected:
//    CaptureScreenImage();
//    virtual ~CaptureScreenImage();
//    void initWith(std::function<void(Image * image)> onCaptureCallback);
//private:
//    void onCaptureScreen(std::function<void(Image * image)>& afterCaptured);
//private:
//    std::function<void(Image * image)> _onCaptureCallback;
//    Image * _image_cache;
//    void onCallBack(float dt);
//};


typedef enum{
    KBT_TYPE_LEFT = 1,
    KBT_TYPE_RIGHT,
    KBT_TYPE_CENTER
}BT_TYPE;

class AvaFrame : public cocos2d::CCLayer {
private:
    
    bool _bTouchOutToRemove;
    
public:
    CCLabelTTF* title;
    CCLabelTTF* text;
    AvaFrame();
    virtual ~AvaFrame();
    void initWith();
    AvaFrame* setCenter();
    AvaFrame* setCenterDirectorAddBack(CCTexture2D * textureCapture, bool create_touch = true);
    AvaFrame* createTouch();
    AvaFrame* createTouchTouchOutToRemove();
    AvaFrame* setTile(const std::string& content);
    AvaFrame* setText(const std::string& content, float muY = 0.38f);
    AvaFrame* createBt(const std::string& text,
                       CCObject* target,
                       SEL_CallFuncN onClick,
                       const BT_TYPE& type = KBT_TYPE_CENTER,
                       const BT_COLOR& bt_colour = KBT_COLOR_BLACK,
                       const int& countTime = 0);
    //                       std::function<void(AvaFrame*)> onClick = [](AvaFrame* frame)
    //                       {
    //                           frame->removeFromParent();
    //                       },
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void registerWithTouchDispatcher();
    
protected:
    void onClickDefault(CCNode* node);
};


#endif /* defined(__DotA__AvaFrame__) */
