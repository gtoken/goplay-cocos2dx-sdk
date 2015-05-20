//
//  AvaFrame.cpp
//  DotA
//
//  Created by DucHQ on 5/26/14.
//
//

#include "AvaFrame.h"
//
////-----------------CaptureScreen--------------------//
//
//void CaptureScreen::create(std::function<void(Texture2D * texture)> afterCaptured){
//    CaptureScreen * mCaptureScreen = new CaptureScreen();
//    mCaptureScreen->initWith(afterCaptured);
//    mCaptureScreen->release();
//}
//
//CaptureScreen::CaptureScreen(){
//    _onCaptureCallback = NULL;
//    _texture_cache = NULL;
//}
//
//CaptureScreen::~CaptureScreen(){
//    if (_texture_cache) {
//        _texture_cache->release();
//        _texture_cache = NULL;
//    }
//}
//
//
//void CaptureScreen::initWith(std::function<void(Texture2D * texture)> onCaptureCallback){
//    CCASSERT(_texture_cache == NULL, "_texture_cache not null");
//    CCASSERT(onCaptureCallback != NULL, "onCaptureCallback not has");
//    static CustomCommand captureScreenCommand;
//    captureScreenCommand.init(std::numeric_limits<float>::max());
//    captureScreenCommand.func = CC_CALLBACK_0(CaptureScreen::onCaptureScreen, this, onCaptureCallback);
//    Director::getInstance()->getRenderer()->addCommand(&captureScreenCommand);
//    this->retain();
//}
//
///**
// * Capture screen implementation, don't use it directly.
// */
//void CaptureScreen::onCaptureScreen(std::function<void(Texture2D * texture)>& onCaptureCallback)
//{
//    Texture2D * result = NULL;
//    auto glView = Director::getInstance()->getOpenGLView();
//    auto frameSize = glView->getFrameSize();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
//    frameSize = frameSize * glView->getFrameZoomFactor() * glView->getRetinaFactor();
//#endif
//    int width = static_cast<int>(frameSize.width);
//    int height = static_cast<int>(frameSize.height);
//    
//    do
//    {
//        std::shared_ptr<GLubyte> buffer(new GLubyte[width * height * 4], [](GLubyte* p){ CC_SAFE_DELETE_ARRAY(p); });
//        if (!buffer)
//        {
//            break;
//        }
//        
//        glPixelStorei(GL_PACK_ALIGNMENT, 1);
//        
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
//        // The frame buffer is always created with portrait orientation on WP8.
//        // So if the current device orientation is landscape, we need to rotate the frame buffer.
//        auto renderTargetSize = glView->getRenerTargetSize();
//        CCASSERT(width * height == static_cast<int>(renderTargetSize.width * renderTargetSize.height), "The frame size is not matched");
//        glReadPixels(0, 0, (int)renderTargetSize.width, (int)renderTargetSize.height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
//#else
//        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
//#endif
//        
//        std::shared_ptr<GLubyte> flippedBuffer(new GLubyte[width * height * 4], [](GLubyte* p) { CC_SAFE_DELETE_ARRAY(p); });
//        if (!flippedBuffer)
//        {
//            break;
//        }
//        
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
//        if (width == static_cast<int>(renderTargetSize.width))
//        {
//            // The current device orientation is portrait.
//            for (int row = 0; row < height; ++row)
//            {
//                memcpy(flippedBuffer.get() + (height - row - 1) * width * 4, buffer.get() + row * width * 4, width * 4);
//            }
//        }
//        else
//        {
//            // The current device orientation is landscape.
//            for (int row = 0; row < width; ++row)
//            {
//                for (int col = 0; col < height; ++col)
//                {
//                    *(int*)(flippedBuffer.get() + (height - col - 1) * width * 4 + row * 4) = *(int*)(buffer.get() + row * height * 4 + col * 4);
//                }
//            }
//        }
//#else
//        for (int row = 0; row < height; ++row)
//        {
//            memcpy(flippedBuffer.get() + (height - row - 1) * width * 4, buffer.get() + row * width * 4, width * 4);
//        }
//#endif
//        Image *image = new Image();
//        if (image)
//        {
//            if(image->initWithRawData(flippedBuffer.get(), width * height * 4, width, height, 8)){
//                Texture2D * texture = new (std::nothrow) Texture2D();
//                if(texture->initWithImage(image)){
//                    result = texture;
//                }else{
//                    texture->release();
//                    texture = NULL;
//                }
//            }
//            image->release();
//            image = NULL;
//        }
//    }while(0);
//        _texture_cache = result;
//        _onCaptureCallback = onCaptureCallback;
//        Director::getInstance()->getScheduler()->schedule(schedule_selector(CaptureScreen::onCallBack), this, 0, 0, 0, false);
//
//}
//
//void CaptureScreen::onCallBack(float dt){
//    CCASSERT(_texture_cache, "_texture_cache null");
//    _onCaptureCallback(_texture_cache);
//    if (_texture_cache) {
//        _texture_cache->release();
//        _texture_cache = NULL;
//    }
//    this->release();
//}
//
//
//
////-----------------CaptureScreenImage--------------------//
//
//void CaptureScreenImage::create(std::function<void(Image * image)> afterCaptured){
//    CaptureScreenImage * mCaptureScreen = new CaptureScreenImage();
//    mCaptureScreen->initWith(afterCaptured);
//    mCaptureScreen->release();
//}
//
//CaptureScreenImage::CaptureScreenImage(){
//    _onCaptureCallback = NULL;
//    _image_cache = NULL;
//}
//
//CaptureScreenImage::~CaptureScreenImage(){
//    if (_image_cache) {
//        _image_cache->release();
//        _image_cache = NULL;
//    }
//}
//
//
//void CaptureScreenImage::initWith(std::function<void(Image * image)> onCaptureCallback){
//    CCASSERT(_image_cache == NULL, "_texture_cache not null");
//    CCASSERT(onCaptureCallback != NULL, "onCaptureCallback not has");
//    static CustomCommand captureScreenCommand;
//    captureScreenCommand.init(std::numeric_limits<float>::max());
//    captureScreenCommand.func = CC_CALLBACK_0(CaptureScreenImage::onCaptureScreen, this, onCaptureCallback);
//    Director::getInstance()->getRenderer()->addCommand(&captureScreenCommand);
//    this->retain();
//}
///**
// * Capture screen implementation, don't use it directly.
// */
//void CaptureScreenImage::onCaptureScreen(std::function<void(Image * image)>& onCaptureCallback)
//{
//    Image * image = NULL;
//    auto glView = Director::getInstance()->getOpenGLView();
//    auto frameSize = glView->getFrameSize();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
//    frameSize = frameSize * glView->getFrameZoomFactor() * glView->getRetinaFactor();
//#endif
//    int i_scale = static_cast<int>(1);
//    int width_origin = static_cast<int>(frameSize.width);
//    int height_origin = static_cast<int>(frameSize.height);
//    int width = static_cast<int>(width_origin / i_scale);
//    int height = static_cast<int>(height_origin / i_scale);
//    do
//    {
//        std::shared_ptr<GLubyte> buffer(new GLubyte[width_origin * height_origin * 4], [](GLubyte* p){ CC_SAFE_DELETE_ARRAY(p); });
//        if (!buffer)
//        {
//            break;
//        }
//        
//        glPixelStorei(GL_PACK_ALIGNMENT, 1);
//        
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
//        // The frame buffer is always created with portrait orientation on WP8.
//        // So if the current device orientation is landscape, we need to rotate the frame buffer.
//        auto renderTargetSize = glView->getRenerTargetSize();
//        CCASSERT(width * height == static_cast<int>(renderTargetSize.width * renderTargetSize.height), "The frame size is not matched");
//        glReadPixels(0, 0, (int)renderTargetSize.width, (int)renderTargetSize.height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
//#else
//        glReadPixels(0, 0, width_origin, height_origin, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
//#endif
//        
//        std::shared_ptr<GLubyte> flippedBuffer(new GLubyte[width * height * 4], [](GLubyte* p) { CC_SAFE_DELETE_ARRAY(p); });
//        if (!flippedBuffer)
//        {
//            break;
//        }
//        
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
//        if (width == static_cast<int>(renderTargetSize.width))
//        {
//            // The current device orientation is portrait.
//            for (int row = 0; row < height; ++row)
//            {
//                memcpy(flippedBuffer.get() + (height - row - 1) * width * 4, buffer.get() + row * width * 4, width * 4);
//            }
//        }
//        else
//        {
//            // The current device orientation is landscape.
//            for (int row = 0; row < width; ++row)
//            {
//                for (int col = 0; col < height; ++col)
//                {
//                    *(int*)(flippedBuffer.get() + (height - col - 1) * width * 4 + row * 4) = *(int*)(buffer.get() + row * height * 4 + col * 4);
//                }
//            }
//        }
//#else
////        if (width == width_origin
////            && height == height_origin) {
////            for (int row = 0; row < height; ++row)
////            {
////                memcpy(flippedBuffer.get() + (height - row - 1) * width * 4, buffer.get() + row * width * 4, width * 4);
////            }
////        }else
//        {
//            for (int row = 0; row < height; ++row)
//            {
//                for (int col = 0; col < width; ++col)
//                {
//                    int row_origin = row * height_origin / height;
//                    if (row_origin < 0) {
//                        row_origin = 0;
//                    }else if ( row_origin >= height_origin){
//                        row_origin = height_origin - 1;
//                    }
//                    int col_origin = col * width_origin / width;
//                    if (col_origin < 0) {
//                        col_origin = 0;
//                    }else if ( col_origin >= width_origin){
//                        col_origin = width_origin - 1;
//                    }
//                
//                    memcpy(flippedBuffer.get() + (height - row - 1) * width * 4 + col * 4, buffer.get() + row_origin * width_origin * 4 + col_origin * 4, 4);
//                }
//            }
//        }
//#endif
//        image = new Image();
//        if (image)
//        {
//            if(!image->initWithRawData(flippedBuffer.get(), width * height * 4, width, height, 8)){
//                image->release();
//                image = NULL;
//            }
//        }
//    }while(0);
//    _image_cache = image;
//    _onCaptureCallback = onCaptureCallback;
//    Director::getInstance()->getScheduler()->schedule(schedule_selector(CaptureScreenImage::onCallBack), this, 0, 0, 0, false);
//    
//}
//
//void CaptureScreenImage::onCallBack(float dt){
//    _onCaptureCallback(_image_cache);
//    if (_image_cache) {
//        _image_cache->release();
//        _image_cache = NULL;
//    }
//    this->release();
//}
//

//-----------------------AvaFrame----------------------//
AvaFrame::AvaFrame(){
    title = NULL;
    text = NULL;
    _bTouchOutToRemove = false;
    setTouchEnabled(true);
}

AvaFrame::~AvaFrame(){
    
}

void AvaFrame::initWith(){
    setTouchEnabled(true);
    CCSprite * b = CCSprite::create("common/dialog_back_02.png");
    this->ignoreAnchorPointForPosition(false);
    this->setContentSize(b->getContentSize());
    this->setAnchorPoint(ANCHOR_MIDDLE);
    this->addChild(b);
    b->setPosition( this->getContentSize() / 2);
}

AvaFrame* AvaFrame::setCenter(){
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint pos = ccp(visibleSize.width / 2.0f, visibleSize.height / 2.0f);
    setPosition(pos);
    return this;
}

AvaFrame* AvaFrame::setCenterDirectorAddBack(CCTexture2D * textureCapture, bool create_touch){
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint pos = ccp(visibleSize.width / 2.0f, visibleSize.height / 2.0f);
    CCPoint size = getContentSize();
    setPosition(pos);
    
    do {
        //Create Back
        if(textureCapture){
            CCSprite* sprite = CCSprite::createWithTexture(textureCapture);
            addChild( sprite, -2);
            sprite->setPosition(size * 0.5f);
            sprite->setScaleX(visibleSize.width / sprite->getTextureRect().size.width);
            sprite->setScaleY(visibleSize.height / sprite->getTextureRect().size.height);
        }
    } while (0);
    if (create_touch) {
        this->createTouch();
    }
    return this;
}

bool AvaFrame::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!this->isVisible()) {
        return false;
    }
    CCNode * nodeParent = this->getParent();
    
    if (_bTouchOutToRemove)
    {
        while (nodeParent) {
            if( !nodeParent->isVisible()){
                return false;
            }
            nodeParent = nodeParent->getParent();
        }
        CCPoint v = this->convertTouchToNodeSpace(pTouch);
        if (v.x < 0
            || v.y < 0
            || v.x > this->getContentSize().width
            || v.y > this->getContentSize().height) {
            return true;
        }
        return false;
    }
    else
    {
        while (nodeParent) {
            if( !nodeParent->isVisible()){
                return false;
            }
            nodeParent = nodeParent->getParent();
        }
        return true;
    }
}

void AvaFrame::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
}

void AvaFrame::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (_bTouchOutToRemove)
    {
        this->removeFromParent();
    }
}

void AvaFrame::registerWithTouchDispatcher()
{
    //TODO: touch swallow
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, true);
}

AvaFrame* AvaFrame::createTouch(){
    _bTouchOutToRemove = false;
    return this;
}

AvaFrame* AvaFrame::createTouchTouchOutToRemove() {
    _bTouchOutToRemove = true;
    return this;
}

AvaFrame* AvaFrame::setTile(const std::string& content){
    if (!title) {
        title = CCLabelTTF::create(content.c_str(), s_pPathFontCommonBold, 26, CCSize(440.0f, 0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        title->setAnchorPoint(ANCHOR_MIDDLE);
        addChild( title);
        title->setPositionX( this->getContentSize().width * 0.5f);
        title->setPositionY( this->getContentSize().height - 36);
        
//        title->enableOutline(Color4B(0, 0, 0, 80), 2);
//        title->setAdditionalKerning(-4);

    } else {
        title->setString(content.c_str());
    }
    return this;
}

AvaFrame* AvaFrame::setText(const std::string& content, float muY){
    if (!text) {
        text = CCLabelTTF::create(content.c_str(), s_pPathFontCommon, 22, CCSize(440.0f, 0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        text->setAnchorPoint(ANCHOR_MIDDLE);
        addChild(text);
        text->setPositionX( this->getContentSize().width * 0.5f);
        text->setPositionY( this->getContentSize().height * muY);
//        text->enableOutline(Color4B::BLACK, 2);
        text->setColor(ccc3(139, 139, 139));
    }else{
        text->setString(content.c_str());
    }
    return this;
}

AvaFrame* AvaFrame::createBt(const std::string& text,
                             CCObject* target,
                             SEL_CallFuncN onClick,
                             const BT_TYPE& type /* = KBT_TYPE_CENTER */,
                             const BT_COLOR& bt_colour /* = KBT_COLOR_BLACK */,
                             const int& countTime /* = 0 */ ) {
    AvaButtonUI* bt= new AvaButtonUI();
    bt->initWithTwoPath(CCSprite::create("common/dialog_bt_01.png"), CCSprite::create("common/dialog_bt_02.png"));
    addChild(bt);
    bt->release();
    switch (type) {
        case KBT_TYPE_LEFT:
        {
            bt->setPositionX( this->getContentSize().width * 0.28f);
            bt->setOffsetTouchRight(0);
        }
            break;
        case KBT_TYPE_RIGHT:
        {
            bt->setPositionX( this->getContentSize().width * (1.0f - 0.28f));
            bt->setOffsetTouchLeft(0);
        }
            break;
        case KBT_TYPE_CENTER:
        default:
        {
            bt->setPositionX( this->getContentSize().width * 0.5f);
        }
            break;
    }
    bt->setPositionY(18);
    bt->setAnchorPoint(ANCHOR_BOTTOM_LEFT);
//    bt->setAnchorPoint(ANCHOR_MIDDLE);
    if (countTime <= 0) {
        //        bt->addText(text);
        bt->addText(text, 24, ccWHITE, 0);
    }else{
        bt->addTextAndClickAffterTime(text, countTime);
    }
    
    if (target && onClick) {
        bt->setEventListeners(target, onClick, NULL, NULL, NULL, NULL);
    } else {
        bt->setEventListeners(this, callfuncN_selector(AvaFrame::onClickDefault), NULL, NULL, NULL, NULL);
    }
    
    return this;
}

void AvaFrame::onClickDefault(cocos2d::CCNode *node)
{
    this->removeFromParent();
}
