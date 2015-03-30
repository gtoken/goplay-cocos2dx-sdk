//
//  AvaButton.cpp
//  DotA
//
//  Created by DucHQ on 5/26/14.
//
//

#include "AvaButton.h"

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
    mListenerTouch_OneByOne = nullptr;
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
    mMat4WtoN_TouchCheck = nullptr;
    mSize_TouchCheck = nullptr;
    mMat4WtoN_TouchCheck_Begin = nullptr;
    mSize_TouchCheck_Begin = nullptr;
    mIsVisiting = false;
    mAvaButton_Delegate = nullptr;
    onClick = nullptr;
    onBeginTouchIn = nullptr;
    onMoveInSight = nullptr;
    onMoveOutSight = nullptr;
    onEndTouch = nullptr;
    _touchtaget = nullptr;
    _touch_when_click = nullptr;
    _last_touch_out_of_move = nullptr;
    mIsJustCheckBeginOnTouchAllAtOne = false;
}

AvaButton::~AvaButton(){
    this->setTouchTaget(nullptr);
    if (_touch_when_click) {
        _touch_when_click->release();
        _touch_when_click = nullptr;
    }
    if (mMat4WtoN_TouchCheck_Begin) {
        delete mMat4WtoN_TouchCheck_Begin;
        mMat4WtoN_TouchCheck_Begin = nullptr;
        delete mSize_TouchCheck_Begin;
        mSize_TouchCheck_Begin = nullptr;
    }
    
    if (mMat4WtoN_TouchCheck) {
        delete mMat4WtoN_TouchCheck;
        mMat4WtoN_TouchCheck = nullptr;
        delete mSize_TouchCheck;
        mSize_TouchCheck = nullptr;
    }
}

void AvaButton::initWith(){
    
}

void AvaButton::setTouchEnable( bool b){
    if (!b) {
        if ( mListenerTouch_OneByOne) {
            mListenerTouch_OneByOne->setEnabled( false);
        }
    }else{
        if( !mListenerTouch_OneByOne){
            mListenerTouch_OneByOne = EventListenerTouchOneByOne::create();
            mListenerTouch_OneByOne->setSwallowTouches(isSwallow);
            mListenerTouch_OneByOne->onTouchBegan = CC_CALLBACK_2(AvaButton::onTouchBegan, this);
            mListenerTouch_OneByOne->onTouchMoved = CC_CALLBACK_2(AvaButton::onTouchMoved, this);
            mListenerTouch_OneByOne->onTouchEnded = CC_CALLBACK_2(AvaButton::onTouchEnded, this);
            mListenerTouch_OneByOne->onTouchCancelled = CC_CALLBACK_2(AvaButton::onTouchCancelled, this);
            getEventDispatcher()->addEventListenerWithSceneGraphPriority( mListenerTouch_OneByOne, this);
        }else{
            mListenerTouch_OneByOne->setEnabled( true);
        }
    }
}

void AvaButton::setNoteTouchCheckBegin( cocos2d::Node * node_check){
    if ( node_check) {
        mMat4WtoN_TouchCheck_Begin =  new Mat4(node_check->getWorldToNodeTransform());
        mSize_TouchCheck_Begin = new Size(node_check->getContentSize());
    }else{
        if (mMat4WtoN_TouchCheck_Begin) {
            delete mMat4WtoN_TouchCheck_Begin;
            mMat4WtoN_TouchCheck_Begin = nullptr;
            delete mSize_TouchCheck_Begin;
            mSize_TouchCheck_Begin = nullptr;
        }
    }
}

bool AvaButton::isTouchInside_TouchCheckBegin(cocos2d::Touch* touch){
    if (!mMat4WtoN_TouchCheck_Begin) {
        return true;
    }
    Vec2 world = touch->getStartLocation();
    Vec3 vec3(world.x, world.y, 0);
    Vec3 ret;
    mMat4WtoN_TouchCheck_Begin->transformPoint(vec3,&ret);
    if (ret.x < 0) {
        return false;
    }
    if (ret.y < 0) {
        return false;
    }
    if (ret.x > mSize_TouchCheck_Begin->width) {
        return false;
    }
    if (ret.y > mSize_TouchCheck_Begin->height) {
        return false;
    }
    return true;
}

void AvaButton::setNoteTouchCheck( cocos2d::Node * node_check){
    if ( node_check) {
        mMat4WtoN_TouchCheck =  new Mat4(node_check->getWorldToNodeTransform());
        mSize_TouchCheck = new Size(node_check->getContentSize());
    }else{
        if (mMat4WtoN_TouchCheck) {
            delete mMat4WtoN_TouchCheck;
            mMat4WtoN_TouchCheck = nullptr;
            delete mSize_TouchCheck;
            mSize_TouchCheck = nullptr;
        }
    }
}

bool AvaButton::isTouchInside_TouchCheck(const cocos2d::Point& world){
    if (!mMat4WtoN_TouchCheck) {
        return true;
    }
    Vec3 vec3(world.x, world.y, 0);
    Vec3 ret;
    mMat4WtoN_TouchCheck->transformPoint(vec3,&ret);
    if (ret.x < 0) {
        return false;
    }
    if (ret.y < 0) {
        return false;
    }
    if (ret.x > mSize_TouchCheck->width) {
        return false;
    }
    if (ret.y > mSize_TouchCheck->height) {
        return false;
    }
    return true;
}


bool AvaButton::isTouchInside(Touch* touch){
    Point pTouch = touch->getLocation();
    if ( !this->isTouchInside_TouchCheckBegin(touch)) {
        return false;
    }
    if ( !this->isTouchInside_TouchCheck(pTouch)) {
        return false;
    }
    Point locationInNode = convertToNodeSpace(pTouch);
    Size s = getContentSize();
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
    Rect rect = Rect( -mOffsetTouchLeft - widthAdd * 0.5f,
                     -mOffsetTouchBot - heightAdd * 0.5f,
                     sizeW + widthAdd,
                     sizeH + heightAdd);
    if (!rect.containsPoint(locationInNode)){
        return false;
    }
    return true;
}

//OneByOne
bool AvaButton::onTouchBegan(Touch* touch, Event* event){
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
    Node * nodeParent = getParent();
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

void AvaButton::onTouchMoved(Touch* touch, Event* event){
    if (this->getTouchTaget() != touch) {
        CCASSERT(!this->getTouchTaget(), "_touch_taget has not release");
        return;
    }
    this->p_onTouchMoved(touch);
    if (mIsPushed) {
        event->stopPropagation();
    }
}

void AvaButton::onTouchEnded(Touch* touch, Event* event){
    if (!mIsPushed) {
        return;
    }
    if (touch != this->getTouchTaget()) {
        CCASSERT(!this->getTouchTaget(), "_touch_taget has not release");
        return;
    }
    endTouchIn();
    this->setTouchTaget(nullptr);
    //Check CLick
    if (!isVisible()) {
        return;
    }
    Node * nodeParent = getParent();
    while (nodeParent) {
        if( !nodeParent->isVisible()){
            return;
        }
        nodeParent = nodeParent->getParent();
    }
    if ( isTouchInside(touch)) {
        if (_touch_when_click) {
            _touch_when_click->release();
            _touch_when_click = nullptr;
        }
        _touch_when_click = touch;
        _touch_when_click->retain();
        mWaitClick = true;
    }

}

void AvaButton::onTouchCancelled(Touch* touch, Event* event){
    if (!mIsPushed) {
        return;
    }
    if (touch != this->getTouchTaget()) {
        CCASSERT(!this->getTouchTaget(), "_touch_taget has not release");
        return;
    }
    endTouchIn();
    this->setTouchTaget(nullptr);
}

void AvaButton::p_onTouchMoved(cocos2d::Touch* touch){
    if ( !mIsPushed) {
        return;
    }
    if ( mCheckOutOffMove) {
        Point touch_last = touch->getStartLocation();
        Point touchLocation = touch->getLocation();
        float disTanceTouch = (touchLocation.x - touch_last.x) * (touchLocation.x - touch_last.x) + (touchLocation.y - touch_last.y) * (touchLocation.y - touch_last.y);
        if( disTanceTouch > mOffset_CheckOutOffMove)
        {
            _last_touch_out_of_move = touch;
            endTouchIn();
            this->setTouchTaget(nullptr);
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

void AvaButton::beginTouchIn(){
    mIsPushed = true;
    mIsPushInSight = true;
    mCountLongClick = 0;
    schedule(schedule_selector(AvaButton::updateTouch));
    //
    if (onBeginTouchIn) {
        onBeginTouchIn(this);
    }
}

void AvaButton::moveInSight(){
    mCountLongClick = 0;
    if (onMoveInSight) {
        onMoveInSight(this);
    }
}

void AvaButton::moveOutSight(){
    if (onMoveOutSight) {
        onMoveOutSight(this);
    }
}

void AvaButton::endTouchIn(){
    mIsPushed = false;
    mIsPushInSight = false;
    if (onEndTouch) {
        onEndTouch(this);
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
            _touch_when_click = nullptr;
        }
        if (this->getReferenceCount() == 1) {
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
            if (this->getReferenceCount() == 1) {
                this->release();
                return;
            }else{
                this->release();
            }
        }
    }
}

void AvaButton::visit(Renderer *renderer, const Mat4& parentTransform, FLAG_DRAW parentFlags){
    if ( mWaitDismiss) {
        return;
    }
    mIsVisiting = true;
    beforeVisit();
    Node::visit(renderer, parentTransform, parentFlags);
    mIsVisiting = false;
}

void AvaButton::processClick(){
//    dismiss();
    if ( mAvaButton_Delegate) {
        mAvaButton_Delegate->avaClick( this);
    }
    if (onClick) {
        onClick(this);
//        SoundEffectHelper::shareSoundEffectHelper()->playbutton_click();
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
            this->runAction( CallFunc::create(CC_CALLBACK_0(AvaButton::removeFromParent, this)));
        }
    }else{
        removeFromParent();
    }
}

cocos2d::Touch* AvaButton::getTouchTaget(){
    return _touchtaget;
}

void AvaButton::setTouchTaget(cocos2d::Touch* touch){
    if (_touchtaget != touch) {
        if (_touchtaget) {
            _touchtaget->release();
            _touchtaget = nullptr;
        }
        _touchtaget = touch;
        if (_touchtaget) {
            _touchtaget->retain();
        }
    }
}

cocos2d::Touch* AvaButton::getTouchWhenClick(){
    return _touch_when_click;
}
//--------------------------AvaButtonUI------------------//
AvaButtonUI::AvaButtonUI(){
    mBackGround = nullptr;
    mHighlight = nullptr;
    mScaleWhenTouch = 0.96f;
    mAvaButtonStatus = AvaButtonStatus_Scale9Sprite;
    //
    textAndAction_count = 0;
    textAndAction_content = "";
    textAndAction_label = nullptr;
    mLabelBackGround = nullptr;
    mLabelHighlight = nullptr;
}

AvaButtonUI::~AvaButtonUI(){
    
}

void AvaButtonUI::initWithDotaStyle(const cocos2d::Size& size, const BT_COLOR& bt_colour){
    if ( mBackGround) {
        return;
    }
    setContentSize( size);
    setTouchEnable( true);
    Scale9SpriteEx * backGround = new Scale9SpriteEx();
    backGround->initWithFile(s_pPathDotaBtBack, Rect::ZERO, s_pPathDotaBtBack_Rect);
    backGround->setContentSize( size);
    addChild( backGround);
    backGround->release();
    backGround->setPosition( Point( size.width * 0.5f, size.height * 0.5f));
    switch (bt_colour) {
        case BT_COLOR::KBT_COLOR_BLACK:
            backGround->setColor( Color3B(20, 20, 20));
            break;
        case BT_COLOR::KBT_COLOR_RED:
            backGround->setColor( Color3B(68, 44, 44));
            break;
        case BT_COLOR::KBT_COLOR_GREEN:
            backGround->setColor( Color3B(49, 60, 33));
            break;
    }
    
    int offset= 4;
    LayerGradient * light_background = LayerGradient::create(Color4B(255, 255, 255, 100), Color4B(0, 0, 0, 0), Vec2(1, -1));
    light_background->setBlendFunc(BlendFunc::ADDITIVE);
    light_background->changeWidthAndHeight(size.width - offset * 2, size.height - offset * 2);
    backGround->addChild(light_background, 9999);
    light_background->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    light_background->setPosition( Point(offset, offset));
//    light_background->setPositionY(backGround->getContentSize().height);
    
    //Highlight
    Scale9SpriteEx * highlight = new Scale9SpriteEx();
    highlight->initWithFile(s_pPathParticles2, Rect::ZERO, s_pPathParticles2Rect);
    highlight->setContentSize( Size( size.width + 20, size.height + 17));
    backGround->addChild( highlight, -1);
    highlight->release();
    highlight->setPosition( Point( size.width * 0.5f, size.height * 0.5f));
    highlight->getBatchNode()->setBlendFunc(BlendFunc::ADDITIVE);
    //
    mBackGround = backGround;
    mHighlight = highlight;
    mAvaButtonStatus = AvaButtonStatus_Scale9Sprite;
    mHighlight->setVisible(false);
    mHighlight->setCascadeOpacityEnabled( true);
}

void AvaButtonUI::initWithLiveScoreStyle(const cocos2d::Size& size, const std::string& content, const Vec2& anchorPointLayer, const Color4B& colour1, const Color4B& colour2){
    //BackGround
    Node * backGround = Node::create();
    backGround->setContentSize(size);
    backGround->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    backGround->addChild(LayerColor::create(colour1, size.width, size.height));
    //Highlight
    Node * highlight = Node::create();
    highlight->setContentSize(size);
    highlight->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    highlight->addChild(LayerColor::create(colour2, size.width, size.height));
    //
    do {
        Label * label = Label::createWithTTF(content, s_pPathFontMenu, 20, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setAnchorPoint( anchorPointLayer);
        if (anchorPointLayer.x == 0.5f) {
            label->setPositionX( backGround->getContentSize().width / 2);
        }else if (anchorPointLayer.x == 0){
            label->setPositionX( 6);
        }else{
            label->setPositionX( backGround->getContentSize().width - 6);
        }
        label->setPositionY( size.height * 0.5f - 2);
        label->setColor(Color3B::BLACK);
        backGround->addChild( label, 1);
        CCASSERT(mLabelBackGround == nullptr, "mLabelBackGround has init");
        mLabelBackGround = label;
    } while (0);
    do {
        Label * label = Label::createWithTTF(content, s_pPathFontMenu, 20, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setAnchorPoint( anchorPointLayer);
        if (anchorPointLayer.x == 0.5f) {
            label->setPositionX( backGround->getContentSize().width / 2);
        }else if (anchorPointLayer.x == 0){
            label->setPositionX( 6);
        }else{
            label->setPositionX( backGround->getContentSize().width - 14);
        }
        label->setPositionY( size.height * 0.5f - 2);
        label->setColor(Color3B::WHITE);
        highlight->addChild( label, 1);
        CCASSERT(mLabelHighlight == nullptr, "mLabelHighlight has init");
        mLabelHighlight = label;
    } while (0);
    //
    this->initWithTwoPath(backGround, highlight);
    this->setOffsetTouchTop(0);
    this->setOffsetTouchBot(0);
    this->setOffsetTouchRight(0);
    this->setOffsetTouchLeft(0);
}

void AvaButtonUI::initWith9Sprite(const std::string& file, const cocos2d::Rect& capInsets, const cocos2d::Size& size){
    if ( mBackGround) {
        return;
    }
    setContentSize( size);
    setTouchEnable( true);
    Scale9SpriteEx * backGround = new Scale9SpriteEx();
    backGround->initWithFile(file, Rect::ZERO, capInsets);
    backGround->setContentSize( size);
    addChild( backGround);
    backGround->release();
    backGround->setPosition( Point( size.width * 0.5f, size.height * 0.5f));
    //Highlight
    Scale9SpriteEx * highlight = new Scale9SpriteEx();
    highlight->initWithFile(s_pPathParticles1, Rect::ZERO, s_pPathParticles1Rect);
    highlight->setContentSize( Size( size.width + 30, size.height + 30));
    backGround->addChild( highlight, -1);
    highlight->release();
    highlight->setPosition( Point( size.width * 0.5f, size.height * 0.5f));
    //
    mBackGround = backGround;
    mHighlight = highlight;
    mAvaButtonStatus = AvaButtonStatus_Scale9Sprite;
    mHighlight->setVisible(false);
    mHighlight->setCascadeOpacityEnabled( true);
}

void AvaButtonUI::initWithOneSprite(const std::string& file){
    if ( mBackGround) {
        return;
    }
    setTouchEnable( true);
    Sprite * backGround = Sprite::create(file);
    Size size = backGround->getContentSize();
    this->setContentSize( size);
    addChild( backGround);
    backGround->setPosition( Point( size.width * 0.5f, size.height * 0.5f));
    //Highlight
    Scale9SpriteEx * highlight = new Scale9SpriteEx();
    highlight->initWithFile(s_pPathParticles1, Rect::ZERO, s_pPathParticles1Rect);
    highlight->setContentSize( Size( size.width + 30, size.height + 30));
    backGround->addChild( highlight, -1);
    highlight->release();
    highlight->setPosition( Point( size.width * 0.5f, size.height * 0.5f));
    //
    mBackGround = backGround;
    mHighlight = highlight;
    mAvaButtonStatus = AvaButtonStatus_Scale9Sprite;
    mHighlight->setVisible(false);
    mHighlight->setCascadeOpacityEnabled( true);

}

void AvaButtonUI::initWithTwoPath(const std::string& path1, const std::string& path2){
    if ( mBackGround) {
        return;
    }
    Sprite * backGround = Sprite::create(path1);
    //Highlight
    Sprite * highlight = Sprite::create(path2);
    //
    this->initWithTwoPath(backGround, highlight);
}

void AvaButtonUI::initWithTwoPath(cocos2d::Node * backGround, cocos2d::Node * highlight){
    if ( mBackGround) {
        return;
    }
    Size size = backGround->getContentSize();
    setContentSize( size);
    setTouchEnable( true);
    backGround->setPosition( Point( size.width * 0.5f, size.height * 0.5f));
    highlight->setPosition( Point( size.width * 0.5f, size.height * 0.5f));
    addChild( backGround);
    addChild( highlight, -1);
    mBackGround = backGround;
    mHighlight = highlight;
    mAvaButtonStatus = AvaButtonStatus_TwoPath;
    mScaleWhenTouch = 1.0f;
    mHighlight->setVisible(false);
    mHighlight->setCascadeOpacityEnabled( true);
}

void AvaButtonUI::initWithTwo9Sprite(const std::string& file1, const cocos2d::Rect& capInsets1, const std::string& file2, const cocos2d::Rect& capInsets2, const cocos2d::Size& size){
    Scale9SpriteEx * backGround = new Scale9SpriteEx();
    backGround->initWithFile(file1, Rect::ZERO, capInsets1);
    backGround->setContentSize( size);
    
    Scale9SpriteEx * highlight = new Scale9SpriteEx();
    highlight->initWithFile(file2, Rect::ZERO, capInsets2);
    highlight->setContentSize( size);
    
    this->initWithTwoPath(backGround, highlight);
    backGround->release();
    highlight->release();
}

void AvaButtonUI::initWithLightFromSpriteFrame(const std::string& spriteFrameName){
    Sprite * back = Sprite::createWithSpriteFrameName(spriteFrameName);
    Sprite * light = Sprite::createWithSpriteFrameName(spriteFrameName);
    Sprite * light2 = Sprite::createWithSpriteFrameName(spriteFrameName);
    light->addChild(light2);
    light2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    light2->setBlendFunc(cocos2d::BlendFunc::ADDITIVE);
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
            float mu = (1 - mBackGround->getScale()) / (1 - mScaleWhenTouch);
            mHighlight->setOpacity( (int)(mu * 255));
        }
    }
}

Label* AvaButtonUI::addText(const std::string& content,  const int& size_text, const Color3B& colour, const int& outline){
    Label * label = Label::createWithTTF(content, s_pPathFontCommon, size_text, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    label->setAnchorPoint( Vec2::ANCHOR_MIDDLE);
    if (mAvaButtonStatus == AvaButtonStatus::AvaButtonStatus_Scale9Sprite) {
        mBackGround->addChild( label, 1);
        label->setPosition( mBackGround->getContentSize() / 2);
    }else{
        this->addChild( label, 1);
        label->setPosition( this->getContentSize() / 2);
    }
    label->setPositionY( label->getPositionY() + OFFSET_Y_LABEL);
    if(outline != 0){
        label->enableOutline(Color4B::BLACK, outline);
        label->setAdditionalKerning(-(outline*2));
    }
    label->setColor(colour);
    
    return label;
}

void AvaButtonUI::addTextNoelStyle(const std::string& content, const char * path_icon){
    Sprite * icon = Sprite::createWithSpriteFrameName(path_icon);
    this->addChild(icon, 1);
    icon->setPosition(65, this->getContentSize().height / 2 + 2);
    
    Label * lb = this->addText(content, 32, Color3B::WHITE, 0);
    lb->enableOutline(Color4B(50, 50, 50, 255), 2);
    lb->setAdditionalKerning(-4);
    lb->setPositionX(this->getContentSize().width / 2 + 28);
}

Label* AvaButtonUI::addTextAndClickAffterTime(const std::string& content, const int& countTime, const int& size_text, const Color3B& colour, const int& outline){
    CCASSERT(textAndAction_label == nullptr, " textAndAction_label must nullptr!");
    CCASSERT(countTime > 0, " countTime must larger than 0!");
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
        this->runAction(CallFunc::create(CC_CALLBACK_0(AvaButtonUI::processClick,this)));
    }
}

cocos2d::Node * AvaButtonUI::getBackGround(){
    return mBackGround;
}

cocos2d::Node * AvaButtonUI::getHighlight(){
    return mHighlight;
}

Label * AvaButtonUI::getLabelBackground(){
    return mLabelBackGround;
}

Label * AvaButtonUI::getLabelHighlight(){
    return mLabelHighlight;
}