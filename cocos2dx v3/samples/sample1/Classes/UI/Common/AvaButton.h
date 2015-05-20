//
//  AvaButton.h
//  DotA
//
//  Created by DucHQ on 5/26/14.
//
//

#ifndef __DotA__AvaButton__
#define __DotA__AvaButton__

#include "Scale9SpriteEx.h"

class AvaButton;
class AvaButton_Delegate {
public:
    virtual void avaClick(AvaButton * pAvaButton){};
    virtual void avaLongClick(AvaButton * pAvaButton){};
};

class AvaButton : public cocos2d::Node{
protected:
    
    CREATE_SET_GET_FUNC(IsSwallow, bool, isSwallow)
    
    CREATE_SET_GET_FUNC(IsJustCheckBeginOnTouchAllAtOne, bool, mIsJustCheckBeginOnTouchAllAtOne)
    
    cocos2d::EventListenerTouchOneByOne* mListenerTouch_OneByOne;
    
    CREATE_SET_GET_FUNC(OffsetTouchTop, float, mOffsetTouchTop)
    CREATE_SET_GET_FUNC(OffsetTouchRight, float, mOffsetTouchRight)
    CREATE_SET_GET_FUNC(OffsetTouchLeft, float, mOffsetTouchLeft)
    CREATE_SET_GET_FUNC(OffsetTouchBot, float, mOffsetTouchBot)
    //
    CREATE_SET_GET_FUNC(IsPushed, bool, mIsPushed)
    CREATE_SET_GET_FUNC(IsPushInSight, bool, mIsPushInSight)
    //
    CREATE_SET_GET_FUNC(CheckOutOffMove, bool, mCheckOutOffMove)
    CREATE_SET_GET_FUNC(Offset_CheckOutOffMove, float, mOffset_CheckOutOffMove)
    
    CREATE_SET_GET_FUNC(AvaButton_Delegate, AvaButton_Delegate *, mAvaButton_Delegate)
public:
    std::function<void(AvaButton*)> onClick;//When TouchEndInSide
    std::function<void(AvaButton*)> onBeginTouchIn;//BeginProcessTouch
    std::function<void(AvaButton*)> onMoveInSight;//When Move
    std::function<void(AvaButton*)> onMoveOutSight;//When Move
    std::function<void(AvaButton*)> onEndTouch;//End Process Touch (TouchEndInSide or TouchEndOutSide)
protected:
    bool mWaitClick;
    float mCountLongClick;
    bool mWaitDismiss;
    Mat4 * mMat4WtoN_TouchCheck;
    Size * mSize_TouchCheck;
    Mat4 * mMat4WtoN_TouchCheck_Begin;
    Size * mSize_TouchCheck_Begin;
    bool mIsVisiting;
    Touch* _touchtaget;
    Touch* _touch_when_click;
    
    Touch* _last_touch_out_of_move;
public:
    AvaButton();
    virtual ~AvaButton();
    void initWith();
    
    void setTouchEnable( bool b);
    void setNoteTouchCheckBegin( cocos2d::Node * node_check);
    bool isTouchInside_TouchCheckBegin(cocos2d::Touch* touch);
    void setNoteTouchCheck( cocos2d::Node * node_check);
    bool isTouchInside_TouchCheck(const cocos2d::Point& world);
    
    virtual bool isTouchInside(cocos2d::Touch* touch);
    
    //OneByOne
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
    //Common process touch move
    void p_onTouchMoved(cocos2d::Touch* touch);
    
    virtual void beginTouchIn();
    virtual void moveInSight();
    virtual void moveOutSight();
    virtual void endTouchIn();
    
    void updateTouch( float dt);
    virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, FLAG_DRAW parentFlags);
    
    virtual void processClick();
    virtual void processLongClick();
    virtual void beforeVisit();
    
    virtual void dismiss();
    
    cocos2d::Touch* getTouchTaget();
    void setTouchTaget(cocos2d::Touch* touch);
    
    cocos2d::Touch* getTouchWhenClick();
};

typedef enum{
    AvaButtonStatus_Scale9Sprite,
    AvaButtonStatus_TwoPath
}AvaButtonStatus;


typedef enum{
    KBT_COLOR_RED = 1,
    KBT_COLOR_BLACK,
    KBT_COLOR_GREEN
}BT_COLOR;

class AvaButtonUI: public AvaButton{
protected:
    cocos2d::Node * mBackGround;
    cocos2d::Node * mHighlight;
    CREATE_SET_GET_FUNC(ScaleWhenTouch, float, mScaleWhenTouch);
    AvaButtonStatus mAvaButtonStatus;
private:
    int textAndAction_count;
    std::string textAndAction_content;
    Label * textAndAction_label;
    Label * mLabelBackGround;
    Label * mLabelHighlight;
public:
    AvaButtonUI();
    virtual ~AvaButtonUI();
    
    void initWithDotaStyle(const cocos2d::Size& size = Size(200, 40), const BT_COLOR& bt_colour = BT_COLOR::KBT_COLOR_BLACK);
    void initWithLiveScoreStyle(const cocos2d::Size& size, const std::string& content, const Vec2& anchorPointLayer, const Color4B& colour1, const Color4B& colour2);
    
    void initWith9Sprite(const std::string& file, const cocos2d::Rect& capInsets, const cocos2d::Size& size);
    void initWithOneSprite(const std::string& file);
    
    void initWithTwoPath(const std::string& path1, const std::string& path2);
    void initWithTwoPath(cocos2d::Node * background, cocos2d::Node * hightlight);
    void initWithTwo9Sprite(const std::string& file1, const cocos2d::Rect& capInsets1, const std::string& file2, const cocos2d::Rect& capInsets2, const cocos2d::Size& size);
    void initWithLightFromSpriteFrame(const std::string& spriteFrameName);
    
    virtual void beforeVisit();
    
    Label* addText(const std::string& content, const int& size_text = 23, const Color3B& colour = Color3B(185, 185, 185), const int& outline = 2);
    void addTextNoelStyle(const std::string& content, const char * path_icon);
    
    Label* addTextAndClickAffterTime(const std::string& content, const int& countTime, const int& size_text = 23, const Color3B& colour = Color3B(185, 185, 185), const int& outline = 2);
    void textandaction(float dt);
    
    cocos2d::Node * getBackGround();
    cocos2d::Node * getHighlight();
    Label * getLabelBackground();
    Label * getLabelHighlight();
};
#endif /* defined(__DotA__AvaButton__) */
