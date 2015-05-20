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

class AvaButton : public cocos2d::CCLayer {
protected:
    
    CREATE_SET_GET_FUNC(IsSwallow, bool, isSwallow)
    
    CREATE_SET_GET_FUNC(IsJustCheckBeginOnTouchAllAtOne, bool, mIsJustCheckBeginOnTouchAllAtOne)
    
//    cocos2d::EventListenerTouchOneByOne* mListenerTouch_OneByOne;
    
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
    
//    std::function<void(AvaButton*)> onClick;//When TouchEndInSide
//    std::function<void(AvaButton*)> onBeginTouchIn;//BeginProcessTouch
//    std::function<void(AvaButton*)> onMoveInSight;//When Move
//    std::function<void(AvaButton*)> onMoveOutSight;//When Move
//    std::function<void(AvaButton*)> onEndTouch;//End Process Touch (TouchEndInSide or TouchEndOutSide)
    
protected:
    SEL_CallFuncN _onClick;
    SEL_CallFuncN _onBeginTouchIn;
    SEL_CallFuncN _onMoveInSight;
    SEL_CallFuncN _onMoveOutSight;
    SEL_CallFuncN _onEndTouch;
    
    CCObject* _listener;

public:
    
    void setEventListeners(CCObject* target, SEL_CallFuncN onClick, SEL_CallFuncN onBeginTouchIn,
                           SEL_CallFuncN onMoveInSight, SEL_CallFuncN onMoveOutSight, SEL_CallFuncN onEndTouch);
    
    // touch events
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

protected:
    bool mWaitClick;
    float mCountLongClick;
    bool mWaitDismiss;
    kmMat4 mMat4WtoN_TouchCheck;
    CCSize mSize_TouchCheck;
    kmMat4 mMat4WtoN_TouchCheck_Begin;
    CCSize mSize_TouchCheck_Begin;
    bool mIsVisiting;
    CCTouch* _touchtaget;
    CCTouch* _touch_when_click;
    
    CCTouch* _last_touch_out_of_move;
    
public:
    AvaButton();
    virtual ~AvaButton();
    void initWith();
    
    virtual void registerWithTouchDispatcher();
    
    void setNoteTouchCheckBegin( cocos2d::CCNode * node_check);
    bool isTouchInside_TouchCheckBegin(cocos2d::CCTouch* touch);
    void setNoteTouchCheck( cocos2d::CCNode * node_check);
    bool isTouchInside_TouchCheck(const cocos2d::CCPoint& world);
    
    virtual bool isTouchInside(cocos2d::CCTouch* touch);
    
    //Common process touch move
    void p_onTouchMoved(cocos2d::CCTouch* touch);
    
    virtual void beginTouchIn();
    virtual void moveInSight();
    virtual void moveOutSight();
    virtual void endTouchIn();
    
    void updateTouch( float dt);
    virtual void visit();
    
    virtual void processClick();
    virtual void processLongClick();
    virtual void beforeVisit();
    
    virtual void dismiss();
    
    cocos2d::CCTouch* getTouchTaget();
    void setTouchTaget(cocos2d::CCTouch* touch);
    
    cocos2d::CCTouch* getTouchWhenClick();
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
    cocos2d::CCNode * mBackGround;
    cocos2d::CCNode * mHighlight;
    CREATE_SET_GET_FUNC(ScaleWhenTouch, float, mScaleWhenTouch);
    AvaButtonStatus mAvaButtonStatus;
private:
    int textAndAction_count;
    std::string textAndAction_content;
    cocos2d::CCLabelTTF* textAndAction_label;
    cocos2d::CCLabelTTF* mLabelBackGround;
    cocos2d::CCLabelTTF* mLabelHighlight;
public:
    AvaButtonUI();
    virtual ~AvaButtonUI();
    
    void initWithDotaStyle(const cocos2d::CCSize& size = CCSize(200, 40), const BT_COLOR& bt_colour = KBT_COLOR_BLACK);
    void initWithLiveScoreStyle(const cocos2d::CCSize& size, const std::string& content, const CCPoint& anchorPointLayer, const ccColor4B& colour1, const ccColor4B& colour2);
    
    void initWith9Sprite(const std::string& file, const cocos2d::CCRect& capInsets, const cocos2d::CCSize& size);
    void initWithOneSprite(const std::string& file);
    
    void initWithTwoPath(const std::string& path1, const std::string& path2);
    void initWithTwoPath(cocos2d::CCNode * background, cocos2d::CCNode * hightlight);
    void initWithTwo9Sprite(const std::string& file1, const cocos2d::CCRect& capInsets1, const std::string& file2, const cocos2d::CCRect& capInsets2, const cocos2d::CCSize& size);
    void initWithLightFromSpriteFrame(const std::string& spriteFrameName);
    
    virtual void beforeVisit();
    
    cocos2d::CCLabelTTF* addText(const std::string& content, const int& size_text = 23, const ccColor3B& colour = ccc3(185, 185, 185), const int& outline = 2);
    void addTextNoelStyle(const std::string& content, const char * path_icon);
    
    cocos2d::CCLabelTTF* addTextAndClickAffterTime(const std::string& content, const int& countTime, const int& size_text = 23, const ccColor3B& colour = ccc3(185, 185, 185), const int& outline = 2);
    void textandaction(float dt);
    
    cocos2d::CCNode * getBackGround();
    cocos2d::CCNode * getHighlight();
    cocos2d::CCLabelTTF* getLabelBackground();
    cocos2d::CCLabelTTF* getLabelHighlight();
};
#endif /* defined(__DotA__AvaButton__) */
