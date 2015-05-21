//
//  AvaFrame.cpp
//  DotA
//
//  Created by DucHQ on 5/26/14.
//
//

#include "AvaFrame.h"

//-----------------CaptureScreen--------------------//

void CaptureScreen::create(std::function<void(Texture2D * texture)> afterCaptured){
    CaptureScreen * mCaptureScreen = new CaptureScreen();
    mCaptureScreen->initWith(afterCaptured);
    mCaptureScreen->release();
}

CaptureScreen::CaptureScreen(){
    _onCaptureCallback = nullptr;
    _texture_cache = nullptr;
}

CaptureScreen::~CaptureScreen(){
    if (_texture_cache) {
        _texture_cache->release();
        _texture_cache = nullptr;
    }
}


void CaptureScreen::initWith(std::function<void(Texture2D * texture)> onCaptureCallback){
    CCASSERT(_texture_cache == nullptr, "_texture_cache not null");
    CCASSERT(onCaptureCallback != nullptr, "onCaptureCallback not has");
    static CustomCommand captureScreenCommand;
    captureScreenCommand.init(std::numeric_limits<float>::max());
    captureScreenCommand.func = CC_CALLBACK_0(CaptureScreen::onCaptureScreen, this, onCaptureCallback);
    Director::getInstance()->getRenderer()->addCommand(&captureScreenCommand);
    this->retain();
}

/**
 * Capture screen implementation, don't use it directly.
 */
void CaptureScreen::onCaptureScreen(std::function<void(Texture2D * texture)>& onCaptureCallback)
{
    Texture2D * result = nullptr;
    auto glView = Director::getInstance()->getOpenGLView();
    auto frameSize = glView->getFrameSize();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    frameSize = frameSize * glView->getFrameZoomFactor() * glView->getRetinaFactor();
#endif
    int width = static_cast<int>(frameSize.width);
    int height = static_cast<int>(frameSize.height);
    
    do
    {
        std::shared_ptr<GLubyte> buffer(new GLubyte[width * height * 4], [](GLubyte* p){ CC_SAFE_DELETE_ARRAY(p); });
        if (!buffer)
        {
            break;
        }
        
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
        // The frame buffer is always created with portrait orientation on WP8.
        // So if the current device orientation is landscape, we need to rotate the frame buffer.
        auto renderTargetSize = glView->getRenerTargetSize();
        CCASSERT(width * height == static_cast<int>(renderTargetSize.width * renderTargetSize.height), "The frame size is not matched");
        glReadPixels(0, 0, (int)renderTargetSize.width, (int)renderTargetSize.height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
#else
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
#endif
        
        std::shared_ptr<GLubyte> flippedBuffer(new GLubyte[width * height * 4], [](GLubyte* p) { CC_SAFE_DELETE_ARRAY(p); });
        if (!flippedBuffer)
        {
            break;
        }
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
        if (width == static_cast<int>(renderTargetSize.width))
        {
            // The current device orientation is portrait.
            for (int row = 0; row < height; ++row)
            {
                memcpy(flippedBuffer.get() + (height - row - 1) * width * 4, buffer.get() + row * width * 4, width * 4);
            }
        }
        else
        {
            // The current device orientation is landscape.
            for (int row = 0; row < width; ++row)
            {
                for (int col = 0; col < height; ++col)
                {
                    *(int*)(flippedBuffer.get() + (height - col - 1) * width * 4 + row * 4) = *(int*)(buffer.get() + row * height * 4 + col * 4);
                }
            }
        }
#else
        for (int row = 0; row < height; ++row)
        {
            memcpy(flippedBuffer.get() + (height - row - 1) * width * 4, buffer.get() + row * width * 4, width * 4);
        }
#endif
        Image *image = new Image();
        if (image)
        {
            if(image->initWithRawData(flippedBuffer.get(), width * height * 4, width, height, 8)){
                Texture2D * texture = new (std::nothrow) Texture2D();
                if(texture->initWithImage(image)){
                    result = texture;
                }else{
                    texture->release();
                    texture = nullptr;
                }
            }
            image->release();
            image = nullptr;
        }
    }while(0);
        _texture_cache = result;
        _onCaptureCallback = onCaptureCallback;
        Director::getInstance()->getScheduler()->schedule(schedule_selector(CaptureScreen::onCallBack), this, 0, 0, 0, false);

}

void CaptureScreen::onCallBack(float dt){
    CCASSERT(_texture_cache, "_texture_cache null");
    _onCaptureCallback(_texture_cache);
    if (_texture_cache) {
        _texture_cache->release();
        _texture_cache = nullptr;
    }
    this->release();
}



//-----------------CaptureScreenImage--------------------//

void CaptureScreenImage::create(std::function<void(Image * image)> afterCaptured){
    CaptureScreenImage * mCaptureScreen = new CaptureScreenImage();
    mCaptureScreen->initWith(afterCaptured);
    mCaptureScreen->release();
}

CaptureScreenImage::CaptureScreenImage(){
    _onCaptureCallback = nullptr;
    _image_cache = nullptr;
}

CaptureScreenImage::~CaptureScreenImage(){
    if (_image_cache) {
        _image_cache->release();
        _image_cache = nullptr;
    }
}


void CaptureScreenImage::initWith(std::function<void(Image * image)> onCaptureCallback){
    CCASSERT(_image_cache == nullptr, "_texture_cache not null");
    CCASSERT(onCaptureCallback != nullptr, "onCaptureCallback not has");
    static CustomCommand captureScreenCommand;
    captureScreenCommand.init(std::numeric_limits<float>::max());
    captureScreenCommand.func = CC_CALLBACK_0(CaptureScreenImage::onCaptureScreen, this, onCaptureCallback);
    Director::getInstance()->getRenderer()->addCommand(&captureScreenCommand);
    this->retain();
}
/**
 * Capture screen implementation, don't use it directly.
 */
void CaptureScreenImage::onCaptureScreen(std::function<void(Image * image)>& onCaptureCallback)
{
    Image * image = nullptr;
    auto glView = Director::getInstance()->getOpenGLView();
    auto frameSize = glView->getFrameSize();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    frameSize = frameSize * glView->getFrameZoomFactor() * glView->getRetinaFactor();
#endif
    int i_scale = static_cast<int>(1);
    int width_origin = static_cast<int>(frameSize.width);
    int height_origin = static_cast<int>(frameSize.height);
    int width = static_cast<int>(width_origin / i_scale);
    int height = static_cast<int>(height_origin / i_scale);
    do
    {
        std::shared_ptr<GLubyte> buffer(new GLubyte[width_origin * height_origin * 4], [](GLubyte* p){ CC_SAFE_DELETE_ARRAY(p); });
        if (!buffer)
        {
            break;
        }
        
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
        // The frame buffer is always created with portrait orientation on WP8.
        // So if the current device orientation is landscape, we need to rotate the frame buffer.
        auto renderTargetSize = glView->getRenerTargetSize();
        CCASSERT(width * height == static_cast<int>(renderTargetSize.width * renderTargetSize.height), "The frame size is not matched");
        glReadPixels(0, 0, (int)renderTargetSize.width, (int)renderTargetSize.height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
#else
        glReadPixels(0, 0, width_origin, height_origin, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
#endif
        
        std::shared_ptr<GLubyte> flippedBuffer(new GLubyte[width * height * 4], [](GLubyte* p) { CC_SAFE_DELETE_ARRAY(p); });
        if (!flippedBuffer)
        {
            break;
        }
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
        if (width == static_cast<int>(renderTargetSize.width))
        {
            // The current device orientation is portrait.
            for (int row = 0; row < height; ++row)
            {
                memcpy(flippedBuffer.get() + (height - row - 1) * width * 4, buffer.get() + row * width * 4, width * 4);
            }
        }
        else
        {
            // The current device orientation is landscape.
            for (int row = 0; row < width; ++row)
            {
                for (int col = 0; col < height; ++col)
                {
                    *(int*)(flippedBuffer.get() + (height - col - 1) * width * 4 + row * 4) = *(int*)(buffer.get() + row * height * 4 + col * 4);
                }
            }
        }
#else
//        if (width == width_origin
//            && height == height_origin) {
//            for (int row = 0; row < height; ++row)
//            {
//                memcpy(flippedBuffer.get() + (height - row - 1) * width * 4, buffer.get() + row * width * 4, width * 4);
//            }
//        }else
        {
            for (int row = 0; row < height; ++row)
            {
                for (int col = 0; col < width; ++col)
                {
                    int row_origin = row * height_origin / height;
                    if (row_origin < 0) {
                        row_origin = 0;
                    }else if ( row_origin >= height_origin){
                        row_origin = height_origin - 1;
                    }
                    int col_origin = col * width_origin / width;
                    if (col_origin < 0) {
                        col_origin = 0;
                    }else if ( col_origin >= width_origin){
                        col_origin = width_origin - 1;
                    }
                
                    memcpy(flippedBuffer.get() + (height - row - 1) * width * 4 + col * 4, buffer.get() + row_origin * width_origin * 4 + col_origin * 4, 4);
                }
            }
        }
#endif
        image = new Image();
        if (image)
        {
            if(!image->initWithRawData(flippedBuffer.get(), width * height * 4, width, height, 8)){
                image->release();
                image = nullptr;
            }
        }
    }while(0);
    _image_cache = image;
    _onCaptureCallback = onCaptureCallback;
    Director::getInstance()->getScheduler()->schedule(schedule_selector(CaptureScreenImage::onCallBack), this, 0, 0, 0, false);
    
}

void CaptureScreenImage::onCallBack(float dt){
    _onCaptureCallback(_image_cache);
    if (_image_cache) {
        _image_cache->release();
        _image_cache = nullptr;
    }
    this->release();
}


//-----------------------AvaFrame----------------------//
AvaFrame::AvaFrame(){
    title = nullptr;
    text = nullptr;
    listener = nullptr;
}

AvaFrame::~AvaFrame(){
}

void AvaFrame::initWith(){
    Sprite * b = Sprite::create("common/dialog_back_02.png");
    this->setContentSize(b->getContentSize());
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(b);
    b->setPosition( this->getContentSize() / 2);
}

AvaFrame* AvaFrame::setCenter(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 pos = visibleSize/2;
    Vec2 size = getContentSize();
    setPosition(pos);
    return this;
}

AvaFrame* AvaFrame::setCenterDirectorAddBack(Texture2D * textureCapture, bool create_touch){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 pos = visibleSize/2;
    Vec2 size = getContentSize();
    setPosition(pos);
    
    do {
        //Create Back
        if(textureCapture){
            auto sprite = Sprite::createWithTexture(textureCapture);
            addChild( sprite, -2);
            sprite->setPosition( size * 0.5f);
            sprite->setScaleX(visibleSize.width / sprite->getTextureRect().size.width);
            sprite->setScaleY(visibleSize.height / sprite->getTextureRect().size.height);
        }
    } while (0);
    if (create_touch) {
        this->createTouch();
    }
    return this;
}

AvaFrame* AvaFrame::createTouch(){
    if (listener) {
        return this;
    }
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    AvaFrame * mAvaFrame = this;
    listener->onTouchBegan = [mAvaFrame](Touch* touch, Event* event) -> bool{
        if (!mAvaFrame->isVisible()) {
            return false;
        }
        Node * nodeParent = mAvaFrame->getParent();
        while (nodeParent) {
            if( !nodeParent->isVisible()){
                return false;
            }
            nodeParent = nodeParent->getParent();
        }
        return true;
    };
    listener->onTouchMoved = [](Touch* touch, Event* event){
    };
    listener->onTouchEnded = [](Touch* touch, Event* event){
    };
    listener->onTouchCancelled = [](Touch* touch, Event* event){
    };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority( listener, this);
    return this;
}

AvaFrame* AvaFrame::createTouchTouchOutToRemove(){
    if (listener) {
        return this;
    }
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [&](Touch* touch, Event* event) -> bool{
        if (!this->isVisible()) {
            return false;
        }
        Node * nodeParent = this->getParent();
        while (nodeParent) {
            if( !nodeParent->isVisible()){
                return false;
            }
            nodeParent = nodeParent->getParent();
        }
        Vec2 v = this->convertTouchToNodeSpace(touch);
        if (v.x < 0
            || v.y < 0
            || v.x > this->getContentSize().width
            || v.y > this->getContentSize().height) {
            return true;
        }
        return false;
    };
    listener->onTouchMoved = [](Touch* touch, Event* event){
    };
    listener->onTouchEnded = [&](Touch* touch, Event* event){
        this->runAction(CallFunc::create([&](){
            this->removeFromParent();
        }));
    };
    listener->onTouchCancelled = [](Touch* touch, Event* event){
    };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority( listener, this);
    return this;
}

AvaFrame* AvaFrame::setTile(const std::string& content){
    if (!title) {
        title = Label::createWithTTF(content, s_pPathFontCommonBold, 26, Size( 440, 0), TextHAlignment::CENTER, TextVAlignment::CENTER);
        title->setAnchorPoint( Vec2::ANCHOR_MIDDLE);
        addChild( title);
        title->setPositionX( this->getContentSize().width * 0.5f);
        title->setPositionY( this->getContentSize().height - 36);
        title->enableOutline(Color4B(0, 0, 0, 80), 2);
        title->setAdditionalKerning(-4);
//        title->setColor(Color3B(244, 244, 244));
    }else{
        title->setString( content);
    }
    return this;
}

AvaFrame* AvaFrame::setText(const std::string& content, float muY){
    if (!text) {
        text = Label::createWithTTF(content, s_pPathFontCommon, 22, Size( 440, 0), TextHAlignment::CENTER, TextVAlignment::CENTER);
        text->setAnchorPoint( Vec2::ANCHOR_MIDDLE);
        addChild( text);
        text->setPositionX( this->getContentSize().width * 0.5f);
        text->setPositionY( this->getContentSize().height * muY);
//        text->enableOutline(Color4B::BLACK, 2);
        text->setColor(Color3B(139, 139, 139));
    }else{
        text->setString( content);
    }
    return this;
}

AvaFrame* AvaFrame::createBt(const std::string& text, const BT_TYPE& type, const BT_COLOR& bt_colour, std::function<void(AvaFrame*)> onClick, const int& countTime){
    AvaButtonUI* bt= new AvaButtonUI();
    bt->initWithTwoPath(Sprite::create("common/dialog_bt_01.png"), Sprite::create("common/dialog_bt_02.png"));
    addChild( bt);
    bt->release();
    switch (type) {
        case BT_TYPE::KBT_TYPE_LEFT:
        {
            bt->setPositionX( this->getContentSize().width * 0.28f);
            bt->setOffsetTouchRight(0);
        }
            break;
        case BT_TYPE::KBT_TYPE_RIGHT:
        {
            bt->setPositionX( this->getContentSize().width * (1.0f - 0.28f));
            bt->setOffsetTouchLeft(0);
        }
            break;
        case BT_TYPE::KBT_TYPE_CENTER:
        default:
        {
            bt->setPositionX( this->getContentSize().width * 0.5f);
        }
            break;
    }
    bt->setPositionY(18);
    bt->setAnchorPoint( Vec2::ANCHOR_MIDDLE);
    if (countTime <= 0) {
        //        bt->addText(text);
        bt->addText(text, 24, Color3B::WHITE, 0);
    }else{
        bt->addTextAndClickAffterTime(text, countTime);
    }
    AvaFrame * frame = this;
    bt->onClick = [frame, onClick](AvaButton* ava_bt){
        onClick(frame);
    };
    return this;
}
