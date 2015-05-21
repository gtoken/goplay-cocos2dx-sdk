//
//  AvaButton.cpp
//  DotA
//
//  Created by DucHQ on 5/26/14.
//
//

#include "AvaButton.h"
#include "support/TransformUtils.h"

#define OFFSET_TOUCH_DEFAUSE 20.0f

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#define DISTANCE_OUT_OF_MOVE 150
#else
#define DISTANCE_OUT_OF_MOVE 50
#endif

#define SizeBTMin 30

#define LONG_CLICK 0.5f

AvaButton::AvaButton(){
    isSwallow = true;
    mOffsetTouchTop = OFFSET_TOUCH_DEFAUSE;
    mOffsetTouchLeft = OFFSET_TOUCH_DEFAUSE;
    mOffsetTouchRight = OFFSET_TOUCH_DEFAUSE;
    mOffsetTouchBot = OFFSET_TOUCH_DEFAUSE;
    mIsPushed = false;
    mIsPushInSight = false;
    mCheckOutOffMove = false;
    mWaitClick = false;
    mCountLongClick = 0;
    mOffset_CheckOutOffMove = DISTANCE_OUT_OF_MOVE;
    mWaitDismiss = false;
    mIsVisiting = false;
    mAvaButton_Delegate = NULL;
    _touchtaget = NULL;
    _touch_when_click = NULL;
    _last_touch_out_of_move = NULL;
    mIsJustCheckBeginOnTouchAllAtOne = false;
    _listener = NULL;
    
    setTouchEnabled(true);
}

AvaButton::~AvaButton(){
    this->setTouchTaget(NULL);
    if (_touch_when_click) {
        _touch_when_click->release();
        _touch_when_click = NULL;
    }
}

void AvaButton::initWith(){
    setTouchEnabled(true);
}

void AvaButton::registerWithTouchDispatcher()
{
    //TODO: touch swallow
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true /*isSwallow*/);
}

void AvaButton::setNoteTouchCheckBegin( cocos2d::CCNode * node_check){
    if (node_check)
    {
//         CCAffineTransform affine = node_check->worldToNodeTransform();
//         CGAffineToGL(&affine, mMat4WtoN_TouchCheck_Begin.mat);
//         mSize_TouchCheck_Begin = node_check->getContentSize();
    }
    else
    {
//        if (mMat4WtoN_TouchCheck_Begin)
//        {
//            delete mMat4WtoN_TouchCheck_Begin;
//            mMat4WtoN_TouchCheck_Begin = NULL;
//            delete mSize_TouchCheck_Begin;
//            mSize_TouchCheck_Begin = NULL;
//        }
    }
}

bool AvaButton::isTouchInside_TouchCheckBegin(cocos2d::CCTouch* touch) {
    
//    if (!mMat4WtoN_TouchCheck_Begin) {
//        return true;
//    }
    
    CCPoint world = touch->getStartLocation();
    kmVec3 vec3 = { world.x, world.y, 0 };
	kmVec3 ret = { 0, 0, 0 };
    
    //TODO: implement matrix math
//    mMat4WtoN_TouchCheck_Begin->transformPoint(vec3,&ret);
    if (ret.x < 0) {
        return false;
    }
    if (ret.y < 0) {
        return false;
    }
    if (ret.x > mSize_TouchCheck_Begin.width) {
        return false;
    }
    if (ret.y > mSize_TouchCheck_Begin.height) {
        return false;
    }
    return true;
}

void AvaButton::setNoteTouchCheck( cocos2d::CCNode * node_check){
    if ( node_check) {
//         CCAffineTransform affine = node_check->worldToNodeTransform();
//         CGAffineToGL(&affine, mMat4WtoN_TouchCheck.mat);
//         mSize_TouchCheck = CCSize(node_check->getContentSize());
    } else {
//        if (mMat4WtoN_TouchCheck) {
//            delete mMat4WtoN_TouchCheck;
//            mMat4WtoN_TouchCheck = NULL;
//            delete mSize_TouchCheck;
//            mSize_TouchCheck = NULL;
//        }
    }
}

bool AvaButton::isTouchInside_TouchCheck(const cocos2d::CCPoint& world){
//    if (!mMat4WtoN_TouchCheck) {
//        return true;
//    }
    
    kmVec3 vec3 = { world.x, world.y, 0 };    
	kmVec3 ret = { 0, 0, 0 };
    
    //TODO: implent matrix math
//    mMat4WtoN_TouchCheck->transformPoint(vec3,&ret);
    if (ret.x < 0) {
        return false;
    }
    if (ret.y < 0) {
        return false;
    }
    if (ret.x > mSize_TouchCheck.width) {
        return false;
    }
    if (ret.y > mSize_TouchCheck.height) {
        return false;
    }
    return true;
}


bool AvaButton::isTouchInside(CCTouch* touch){
    CCPoint pTouch = touch->getLocation();
    if ( !this->isTouchInside_TouchCheckBegin(touch)) {
        return false;
    }
    if ( !this->isTouchInside_TouchCheck(pTouch)) {
        return false;
    }
    CCPoint locationInNode = convertToNodeSpace(pTouch);
    CCSize s = getContentSize();
    float sizeW = mOffsetTouchLeft + s.width + mOffsetTouchRight;
    float widthAdd = 0;
    if ( sizeW < SizeBTMin) {
        widthAdd = (SizeBTMin - sizeW);
    }
    float sizeH = mOffsetTouchBot + s.height + mOffsetTouchTop;
    float heightAdd = 0;
    if ( sizeH < SizeBTMin) {
        heightAdd = (SizeBTMin - sizeH);
    }
    if (mIsPushInSight) {
        widthAdd += 20;
        heightAdd += 20;
    }
    CCRect rect = CCRect( -mOffsetTouchLeft - widthAdd * 0.5f,
                     -mOffsetTouchBot - heightAdd * 0.5f,
                     sizeW + widthAdd,
                     sizeH + heightAdd);
    
    rect.origin.x -= rect.size.width * 0.3f;
    rect.origin.y -= rect.size.height * 0.2f;
    
    if (!rect.containsPoint(locationInNode)){
        return false;
    }
    return true;
}

//OneByOne
bool AvaButton::ccTouchBegan(CCTouch* touch, CCEvent* event){
    //Check Visible
    if (!isVisible()) {
        return false;
    }
    if (this->getTouchTaget()) {
        return false;
    }
    if ( touch->getID() != 0) {
        return false;
    }
    CCNode * nodeParent = getParent();
    while (nodeParent) {
        if( !nodeParent->isVisible()){
            return false;
        }
        nodeParent = nodeParent->getParent();
    }
    //Check In Size
    if ( !isTouchInside(touch)) {
        return false;
    }
    this->setTouchTaget(touch);
    beginTouchIn();
    return true;
}

void AvaButton::ccTouchMoved(CCTouch* touch, CCEvent* event){
    if (this->getTouchTaget() != touch) {
        CCAssert(!this->getTouchTaget(), "_touch_taget has not release");
        return;
    }
    this->p_onTouchMoved(touch);
    
    if (mIsPushed) {
        // This feature not implement in v2
//        event->stopPropagation();
    }
}

void AvaButton::ccTouchEnded(CCTouch* touch, CCEvent* event){
    if (!mIsPushed) {
        return;
    }
    if (touch != this->getTouchTaget()) {
        CCAssert(!this->getTouchTaget(), "_touch_taget has not release");
        return;
    }
    endTouchIn();
    this->setTouchTaget(NULL);
    //Check CLick
    if (!isVisible()) {
        return;
    }
    CCNode * nodeParent = getParent();
    while (nodeParent) {
        if( !nodeParent->isVisible()){
            return;
        }
        nodeParent = nodeParent->getParent();
    }
    if ( isTouchInside(touch)) {
        if (_touch_when_click) {
            _touch_when_click->release();
            _touch_when_click = NULL;
        }
        _touch_when_click = touch;
        _touch_when_click->retain();
        mWaitClick = true;
    }

}

void AvaButton::ccTouchCancelled(CCTouch* touch, CCEvent* event){
    if (!mIsPushed) {
        return;
    }
    if (touch != this->getTouchTaget()) {
        CCAssert(!this->getTouchTaget(), "_touch_taget has not release");
        return;
    }
    endTouchIn();
    this->setTouchTaget(NULL);
}

void AvaButton::p_onTouchMoved(cocos2d::CCTouch* touch){
    if ( !mIsPushed) {
        return;
    }
    if ( mCheckOutOffMove) {
        CCPoint touch_last = touch->getStartLocation();
        CCPoint touchLocation = touch->getLocation();
        float disTanceTouch = (touchLocation.x - touch_last.x) * (touchLocation.x - touch_last.x) + (touchLocation.y - touch_last.y) * (touchLocation.y - touch_last.y);
        if( disTanceTouch > mOffset_CheckOutOffMove)
        {
            _last_touch_out_of_move = touch;
            endTouchIn();
            this->setTouchTaget(NULL);
            return;
        }
    }
    bool lastPushInSight = mIsPushInSight;
    mIsPushInSight = isTouchInside(touch);
    if ( lastPushInSight != mIsPushInSight) {
        if ( mIsPushInSight){
            moveInSight();
        }else{
            moveOutSight();
        }
    }
}

void AvaButton::setEventListeners(cocos2d::CCObject *target, SEL_CallFuncN onClick, SEL_CallFuncN onBeginTouchIn, SEL_CallFuncN onMoveInSight, SEL_CallFuncN onMoveOutSight, SEL_CallFuncN onEndTouch)
{
    _listener = target;
    _onClick = onClick;
    _onBeginTouchIn = onBeginTouchIn;
    _onMoveInSight = onMoveInSight;
    _onMoveOutSight = onMoveOutSight;
    _onEndTouch = onEndTouch;
}

void AvaButton::beginTouchIn(){
    mIsPushed = true;
    mIsPushInSight = true;
    mCountLongClick = 0;
    schedule(schedule_selector(AvaButton::updateTouch));
    
//    if (onBeginTouchIn) {
//        onBeginTouchIn(this);
//    }
    if (_listener && _onBeginTouchIn)
    {
        (_listener->*_onBeginTouchIn)(this);
    }
}

void AvaButton::moveInSight(){
    mCountLongClick = 0;
    
//    if (onMoveInSight) {
//        onMoveInSight(this);
//    }
    if (_listener && _onMoveInSight)
    {
        (_listener->*_onMoveInSight)(this);
    }
}

void AvaButton::moveOutSight(){
//    if (onMoveOutSight) {
//        onMoveOutSight(this);
//    }
    if (_listener && _onMoveOutSight)
    {
        (_listener->*_onMoveOutSight)(this);
    }
}

void AvaButton::endTouchIn(){
    mIsPushed = false;
    mIsPushInSight = false;
    
//    if (onEndTouch) {
//        onEndTouch(this);
//    }
    if (_listener && _onEndTouch)
    {
        (_listener->*_onEndTouch)(this);
    }
}

void AvaButton::updateTouch( float dt){
    if (!mIsPushed) {
        unschedule(schedule_selector(AvaButton::updateTouch));
    }
    if ( mWaitClick) {
        mWaitClick = false;
        this->retain();
        //Process click Here
        processClick();
        if (_touch_when_click) {
            _touch_when_click->release();
            _touch_when_click = NULL;
        }
        if (this->retainCount() == 1) {
            this->release();
            return;
        }else{
            this->release();
        }
    }
    if (mIsPushed && mIsPushInSight && mCountLongClick < LONG_CLICK) {
        mCountLongClick += dt;
        if ( mCountLongClick >= LONG_CLICK) {
            this->retain();
            //Process long click Here
            processLongClick();
            if (this->retainCount() == 1) {
                this->release();
                return;
            }else{
                this->release();
            }
        }
    }
}

void AvaButton::visit() {
    if ( mWaitDismiss) {
        return;
    }
    mIsVisiting = true;
    beforeVisit();
    CCNode::visit();
    mIsVisiting = false;
}

void AvaButton::processClick(){
//    dismiss();
    if ( mAvaButton_Delegate) {
        mAvaButton_Delegate->avaClick( this);
    }
    
//    if (onClick) {
//        onClick(this);
////        SoundEffectHelper::shareSoundEffectHelper()->playbutton_click();
//    }
    if (_listener && _onClick)
    {
        (_listener->*_onClick)(this);
    }
}

void AvaButton::processLongClick(){
//    dismiss();
    if ( mAvaButton_Delegate) {
        mAvaButton_Delegate->avaLongClick( this);
    }
}

void AvaButton::beforeVisit(){
    
}

void AvaButton::dismiss(){
    if ( mIsVisiting) {
        if ( !mWaitDismiss) {
            mWaitDismiss = true;
            this->runAction(CCCallFunc::create(this, callfunc_selector(AvaButton::removeFromParent)));
        }
    }else{
        removeFromParent();
    }
}

cocos2d::CCTouch* AvaButton::getTouchTaget(){
    return _touchtaget;
}

void AvaButton::setTouchTaget(cocos2d::CCTouch* touch){
    if (_touchtaget != touch) {
        if (_touchtaget) {
            _touchtaget->release();
            _touchtaget = NULL;
        }
        _touchtaget = touch;
        if (_touchtaget) {
            _touchtaget->retain();
        }
    }
}

cocos2d::CCTouch* AvaButton::getTouchWhenClick(){
    return _touch_when_click;
}
//--------------------------AvaButtonUI------------------//
AvaButtonUI::AvaButtonUI(){
    mBackGround = NULL;
    mHighlight = NULL;
    mScaleWhenTouch = 0.96f;
    mAvaButtonStatus = AvaButtonStatus_Scale9Sprite;
    //
    textAndAction_count = 0;
    textAndAction_content = "";
    textAndAction_label = NULL;
    mLabelBackGround = NULL;
    mLabelHighlight = NULL;
}

AvaButtonUI::~AvaButtonUI(){
    
}

void AvaButtonUI::initWithDotaStyle(const cocos2d::CCSize& size, const BT_COLOR& bt_colour){
    if ( mBackGround) {
        return;
    }
    setContentSize( size);
    setTouchEnabled(true);
    Scale9SpriteEx * backGround = new Scale9SpriteEx();
    backGround->initWithFile(s_pPathDotaBtBack, CCRectZero, s_pPathDotaBtBack_Rect);
    backGround->setContentSize( size);
    addChild( backGround);
    backGround->release();
    backGround->setPosition( ccp( size.width * 0.5f, size.height * 0.5f));
    switch (bt_colour) {
        case KBT_COLOR_BLACK:
            backGround->setColor( ccc3(20, 20, 20));
            break;
        case KBT_COLOR_RED:
            backGround->setColor( ccc3(68, 44, 44));
            break;
        case KBT_COLOR_GREEN:
            backGround->setColor( ccc3(49, 60, 33));
            break;
    }
    
    int offset= 4;
    CCLayerGradient * light_background = CCLayerGradient::create(ccc4(255, 255, 255, 100), ccc4(0, 0, 0, 0), ccp(1, -1));
    ccBlendFunc blendFunc = { GL_SRC_ALPHA, GL_ONE };
    light_background->setBlendFunc(blendFunc);
    light_background->changeWidthAndHeight(size.width - offset * 2, size.height - offset * 2);
    backGround->addChild(light_background, 9999);
    light_background->setAnchorPoint(ANCHOR_TOP_LEFT);
    light_background->setPosition(ccp(offset, offset));
    
    //Highlight
    Scale9SpriteEx * highlight = new Scale9SpriteEx();
    highlight->initWithFile(s_pPathParticles2, CCRectZero, s_pPathParticles2Rect);
    highlight->setContentSize( CCSize( size.width + 20, size.height + 17));
    backGround->addChild( highlight, -1);
    highlight->release();
    highlight->setPosition( ccp( size.width * 0.5f, size.height * 0.5f));
    highlight->getBatchNode()->setBlendFunc(blendFunc);
    //
    mBackGround = backGround;
    mHighlight = highlight;
    mAvaButtonStatus = AvaButtonStatus_Scale9Sprite;
    mHighlight->setVisible(false);
    
    // not implement for CCNode in v2
//    mHighlight->setCascadeOpacityEnabled(true);
}

void AvaButtonUI::initWithLiveScoreStyle(const cocos2d::CCSize& size, const std::string& content, const CCPoint& anchorPointLayer, const ccColor4B& colour1, const ccColor4B& colour2){
    //BackGround
    CCNode * backGround = CCNode::create();
    backGround->setContentSize(size);
    backGround->setAnchorPoint(ANCHOR_MIDDLE);
    backGround->addChild(CCLayerColor::create(colour1, size.width, size.height));
    //Highlight
    CCNode * highlight = CCNode::create();
    highlight->setContentSize(size);
    highlight->setAnchorPoint(ANCHOR_MIDDLE);
    highlight->addChild(CCLayerColor::create(colour2, size.width, size.height));
    
    //
    do {
        CCLabelTTF * label = CCLabelTTF::create(content.c_str(), s_pPathFontMenu, 20.0f);
        label->setHorizontalAlignment(kCCTextAlignmentCenter);
        label->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        
        label->setAnchorPoint( anchorPointLayer);
        if (anchorPointLayer.x == 0.5f) {
            label->setPositionX( backGround->getContentSize().width / 2);
        }else if (anchorPointLayer.x == 0){
            label->setPositionX( 6);
        }else{
            label->setPositionX( backGround->getContentSize().width - 6);
        }
        label->setPositionY( size.height * 0.5f - 2);
        label->setColor(ccBLACK);
        backGround->addChild(label, 1);
        CCAssert(mLabelBackGround == NULL, "mLabelBackGround has init");
        mLabelBackGround = label;
    } while (0);
    do {
        CCLabelTTF * label = CCLabelTTF::create(content.c_str(), s_pPathFontMenu, 20);
        label->setHorizontalAlignment(kCCTextAlignmentCenter);
        label->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        
        label->setAnchorPoint( anchorPointLayer);
        if (anchorPointLayer.x == 0.5f) {
            label->setPositionX( backGround->getContentSize().width / 2);
        }else if (anchorPointLayer.x == 0){
            label->setPositionX( 6);
        }else{
            label->setPositionX( backGround->getContentSize().width - 14);
        }
        label->setPositionY( size.height * 0.5f - 2);
        label->setColor(ccWHITE);
        highlight->addChild( label, 1);
        CCAssert(mLabelHighlight == NULL, "mLabelHighlight has init");
        mLabelHighlight = label;
    } while (0);
    //
    this->initWithTwoPath(backGround, highlight);
    this->setOffsetTouchTop(0);
    this->setOffsetTouchBot(0);
    this->setOffsetTouchRight(0);
    this->setOffsetTouchLeft(0);
}

void AvaButtonUI::initWith9Sprite(const std::string& file, const cocos2d::CCRect& capInsets, const cocos2d::CCSize& size){
    if ( mBackGround) {
        return;
    }
    setContentSize( size);
    setTouchEnabled( true);
    Scale9SpriteEx * backGround = new Scale9SpriteEx();
    backGround->initWithFile(file, CCRectZero, capInsets);
    backGround->setContentSize( size);
    addChild( backGround);
    backGround->release();
    backGround->setPosition( ccp( size.width * 0.5f, size.height * 0.5f));
    //Highlight
    Scale9SpriteEx * highlight = new Scale9SpriteEx();
    highlight->initWithFile(s_pPathParticles1, CCRectZero, s_pPathParticles1Rect);
    highlight->setContentSize( CCSize( size.width + 30, size.height + 30));
    backGround->addChild( highlight, -1);
    highlight->release();
    highlight->setPosition( ccp( size.width * 0.5f, size.height * 0.5f));
    //
    mBackGround = backGround;
    mHighlight = highlight;
    mAvaButtonStatus = AvaButtonStatus_Scale9Sprite;
    mHighlight->setVisible(false);
    
    // not implement for Node v2
//    mHighlight->setCascadeOpacityEnabled( true);
}

void AvaButtonUI::initWithOneSprite(const std::string& file){
    if ( mBackGround) {
        return;
    }
    setTouchEnabled( true);
    CCSprite * backGround = CCSprite::create(file.c_str());
    CCSize size = backGround->getContentSize();
    this->setContentSize( size);
    addChild( backGround);
    
//    backGround->setPosition( ccp( size.width * 0.5f, size.height * 0.5f));
    backGround->setPosition(CCPointZero);
    
    //Highlight
    Scale9SpriteEx * highlight = new Scale9SpriteEx();
    highlight->initWithFile(s_pPathParticles1, CCRectZero, s_pPathParticles1Rect);
    highlight->setContentSize( CCSize( size.width + 30, size.height + 30));
    backGround->addChild( highlight, -1);
    highlight->release();
    
//    highlight->setPosition( ccp( size.width * 0.5f, size.height * 0.5f));
    highlight->setPosition(CCPointZero);
    
    //
    mBackGround = backGround;
    mHighlight = highlight;
    mAvaButtonStatus = AvaButtonStatus_Scale9Sprite;
    mHighlight->setVisible(false);
    
    // not implement for Node v2
//    mHighlight->setCascadeOpacityEnabled( true);

}

void AvaButtonUI::initWithTwoPath(const std::string& path1, const std::string& path2){
    if ( mBackGround) {
        return;
    }
    CCSprite * backGround = CCSprite::create(path1.c_str());
    //Highlight
    CCSprite * highlight = CCSprite::create(path2.c_str());
    //
    this->initWithTwoPath(backGround, highlight);
}

void AvaButtonUI::initWithTwoPath(cocos2d::CCNode * backGround, cocos2d::CCNode * highlight){
    if ( mBackGround) {
        return;
    }
    CCSize size = backGround->getContentSize();
    setContentSize( size);
    setTouchEnabled( true);
    
//    backGround->setPosition( ccp( size.width * 0.5f, size.height * 0.5f));
//    highlight->setPosition( ccp( size.width * 0.5f, size.height * 0.5f));
    backGround->setPosition(CCPointZero);
    highlight->setPosition(CCPointZero);
    
    addChild( backGround);
    addChild( highlight, -1);
    mBackGround = backGround;
    mHighlight = highlight;
    mAvaButtonStatus = AvaButtonStatus_TwoPath;
    mScaleWhenTouch = 1.0f;
    mHighlight->setVisible(false);
    
//    mHighlight->setCascadeOpacityEnabled( true);
}

void AvaButtonUI::initWithTwo9Sprite(const std::string& file1, const cocos2d::CCRect& capInsets1, const std::string& file2, const cocos2d::CCRect& capInsets2, const cocos2d::CCSize& size){
    Scale9SpriteEx * backGround = new Scale9SpriteEx();
    backGround->initWithFile(file1, CCRectZero, capInsets1);
    backGround->setContentSize( size);
    
    Scale9SpriteEx * highlight = new Scale9SpriteEx();
    highlight->initWithFile(file2, CCRectZero, capInsets2);
    highlight->setContentSize( size);
    
    this->initWithTwoPath(backGround, highlight);
    backGround->release();
    highlight->release();
}

void AvaButtonUI::initWithLightFromSpriteFrame(const std::string& spriteFrameName){
    CCSprite * back = CCSprite::createWithSpriteFrameName(spriteFrameName.c_str());
    CCSprite * light = CCSprite::createWithSpriteFrameName(spriteFrameName.c_str());
    CCSprite * light2 = CCSprite::createWithSpriteFrameName(spriteFrameName.c_str());
    light->addChild(light2);
    light2->setAnchorPoint(ANCHOR_BOTTOM_LEFT);
    ccBlendFunc blendFunc = { GL_SRC_ALPHA, GL_ONE };
    light2->setBlendFunc(blendFunc);
    light2->setOpacity(120);
    this->initWithTwoPath(back, light);
}

void AvaButtonUI::beforeVisit(){
    if ( mBackGround && mScaleWhenTouch != 1) {
        float scale_back_next = 1;
        if (mIsPushInSight) {
            scale_back_next = mScaleWhenTouch;
        }
        float curent = mBackGround->getScale();
        if ( curent != scale_back_next) {
            float offset = scale_back_next - curent;
            if ( offset < -0.01f || offset > 0.01f) {
                mBackGround->setScale( curent + offset * 0.2f);
            }else{
                mBackGround->setScale( scale_back_next);
            }
        }
    }
    if ( mAvaButtonStatus == AvaButtonStatus_TwoPath) {
        if (mBackGround) {
            mBackGround->setVisible( mIsPushInSight? false: true);
        }
    }
    if (mHighlight) {
        mHighlight->setVisible( mIsPushInSight? true: false);
        if ( mBackGround && mScaleWhenTouch != 1 && mHighlight->isVisible()) {
            
            //TODO: implement set opacity
//            float mu = (1 - mBackGround->getScale()) / (1 - mScaleWhenTouch);
//            mHighlight->setOpacity( (int)(mu * 255));
        }
    }
}

CCLabelTTF* AvaButtonUI::addText(const std::string& content,  const int& size_text, const ccColor3B& colour, const int& outline){
    CCLabelTTF * label = CCLabelTTF::create(content.c_str(), s_pPathFontCommon, size_text);
    label->setHorizontalAlignment(kCCTextAlignmentCenter);
    label->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    
    label->setAnchorPoint(ANCHOR_MIDDLE);
    if (mAvaButtonStatus == AvaButtonStatus_Scale9Sprite) {
        mBackGround->addChild( label, 1);
        label->setPosition( mBackGround->getContentSize() / 2);
    }else{
        this->addChild( label, 1);
//        label->setPosition( this->getContentSize() / 2);
        label->setPosition(CCPointZero);
    }
    label->setPositionY( label->getPositionY() + OFFSET_Y_LABEL);
    if(outline != 0){
        
        // not implement in v2
//        label->enableOutline(ccc4(0, 0, 0, 255), outline);
//        label->setAdditionalKerning(-(outline*2));
        
        label->enableStroke(ccBLACK, outline);
    }
    label->setColor(colour);
    
    return label;
}

void AvaButtonUI::addTextNoelStyle(const std::string& content, const char * path_icon){
    CCSprite * icon = CCSprite::createWithSpriteFrameName(path_icon);
    this->addChild(icon, 1);
    icon->setPosition(ccp(65, this->getContentSize().height / 2 + 2));
    
    CCLabelTTF * lb = this->addText(content, 32, ccWHITE, 0);
    
    // not implement in v2
//    lb->enableOutline(Color4B(50, 50, 50, 255), 2);
//    lb->setAdditionalKerning(-4);
    
    lb->setPositionX(this->getContentSize().width / 2 + 28);
}

CCLabelTTF* AvaButtonUI::addTextAndClickAffterTime(const std::string& content, const int& countTime, const int& size_text, const ccColor3B& colour, const int& outline){
    CCAssert(textAndAction_label == NULL, " textAndAction_label must NULL!");
    CCAssert(countTime > 0, " countTime must larger than 0!");
    textAndAction_content = content;
    textAndAction_count = countTime;
    Utilities::shareUtilities()->initWithFormat("%s (%d)", textAndAction_content.c_str(), textAndAction_count);
    textAndAction_label = this->addText( Utilities::shareUtilities()->getCString(), size_text, colour, outline);
    this->schedule( schedule_selector(AvaButtonUI::textandaction), 1, textAndAction_count, 1);
    return textAndAction_label;
}

void AvaButtonUI::textandaction(float dt){
    textAndAction_count--;
    Utilities::shareUtilities()->initWithFormat("%s (%d)", textAndAction_content.c_str(), textAndAction_count);
    textAndAction_label->setString(Utilities::shareUtilities()->getCString());
    if (textAndAction_count <= 0) {
        this->unschedule(schedule_selector(AvaButtonUI::textandaction));
        this->runAction(CCCallFunc::create(this, callfunc_selector(AvaButtonUI::processClick)));
    }
}

cocos2d::CCNode * AvaButtonUI::getBackGround(){
    return mBackGround;
}

cocos2d::CCNode * AvaButtonUI::getHighlight(){
    return mHighlight;
}

CCLabelTTF * AvaButtonUI::getLabelBackground(){
    return mLabelBackGround;
}

CCLabelTTF * AvaButtonUI::getLabelHighlight(){
    return mLabelHighlight;
}