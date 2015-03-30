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

class CaptureScreen : public cocos2d::Ref{
public:
    static void create(std::function<void(Texture2D * texture)> afterCaptured);
protected:
    CaptureScreen();
    virtual ~CaptureScreen();
    void initWith(std::function<void(Texture2D * texture)> onCaptureCallback);
private:
    void onCaptureScreen(std::function<void(Texture2D * texture)>& afterCaptured);
private:
    std::function<void(Texture2D * texture)> _onCaptureCallback;
    Texture2D * _texture_cache;
    void onCallBack(float dt);
};

class CaptureScreenImage : public cocos2d::Ref{
public:
    static void create(std::function<void(Image * image)> afterCaptured);
protected:
    CaptureScreenImage();
    virtual ~CaptureScreenImage();
    void initWith(std::function<void(Image * image)> onCaptureCallback);
private:
    void onCaptureScreen(std::function<void(Image * image)>& afterCaptured);
private:
    std::function<void(Image * image)> _onCaptureCallback;
    Image * _image_cache;
    void onCallBack(float dt);
};


typedef enum{
    KBT_TYPE_LEFT = 1,
    KBT_TYPE_RIGHT,
    KBT_TYPE_CENTER
}BT_TYPE;

class AvaFrame : public cocos2d::Node{
private:
    EventListenerTouchOneByOne * listener;
public:
    Label* title;
    Label* text;
    AvaFrame();
    virtual ~AvaFrame();
    void initWith();
    AvaFrame* setCenter();
    AvaFrame* setCenterDirectorAddBack(Texture2D * textureCapture, bool create_touch = true);
    AvaFrame* createTouch();
    AvaFrame* createTouchTouchOutToRemove();
    AvaFrame* setTile(const std::string& content);
    AvaFrame* setText(const std::string& content, float muY = 0.38f);
    AvaFrame* createBt(const std::string& text = "CLOSE",
                       const BT_TYPE& type = BT_TYPE::KBT_TYPE_CENTER,
                       const BT_COLOR& bt_colour = BT_COLOR::KBT_COLOR_BLACK,
                       std::function<void(AvaFrame*)> onClick = [](AvaFrame* frame)
                       {
                           frame->removeFromParent();
                       }, const int& countTime = 0);
    
};


#endif /* defined(__DotA__AvaFrame__) */
