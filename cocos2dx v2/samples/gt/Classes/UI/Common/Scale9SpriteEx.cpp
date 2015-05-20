//
//  Scale9SpriteHorizontal.cpp
//  TestGame
//
//  Created by DucHQ on 8/4/14.
//
//

#include "Scale9SpriteEx.h"


Scale9SpriteEx::Scale9SpriteEx(){
    sprite9p = NULL;
    batchnode = NULL;
}

Scale9SpriteEx::~Scale9SpriteEx(){
    
}

void Scale9SpriteEx::initWithFile(const std::string& file, const CCRect& rect,  const CCRect& capInsets){
    setAnchorPoint( ANCHOR_MIDDLE);
    sprite9p = new CCScale9Sprite();
    
    batchnode = CCSpriteBatchNode::create(file.c_str(), 9);
    sprite9p->initWithBatchNode(batchnode, rect, capInsets);
    sprite9p->setAnchorPoint(CCPointZero);
    addChild(sprite9p);
    sprite9p->release();
}

void Scale9SpriteEx::setContentSize(const CCSize& size){
    CCNode::setContentSize(size);
    if (!sprite9p) {
        return;
    }
    
    CCSize sizeNext = size;
    CCSize sizeMin = sprite9p->getPreferredSize();
    if ( sizeMin.width > 0 && size.width < sizeMin.width) {
        sprite9p->setScaleX( size.width / sizeMin.width);
        sizeNext.width = sizeMin.width;
    }else{
        sprite9p->setScaleX(1);
    }
    
    if ( sizeMin.height > 0 && size.height < sizeMin.height) {
        sprite9p->setScaleY( size.height / sizeMin.height);
        sizeNext.height = sizeMin.height;
    }else{
        sprite9p->setScaleY(1);
    }
    sprite9p->setContentSize( sizeNext);
}

void Scale9SpriteEx::setColor(const ccColor3B& color){
//    cocos2d::CCNode::setColor( color);
    if (sprite9p) {
        sprite9p->setColor( color);
    }
}

void Scale9SpriteEx::setOpacity(GLubyte opacity){
//    cocos2d::Node::setOpacity( opacity);
    if (sprite9p) {
        sprite9p->setOpacity( opacity);
    }
}

CCScale9Sprite * Scale9SpriteEx::getScale9Sprite(){
    return sprite9p;
}

CCSpriteBatchNode* Scale9SpriteEx::getBatchNode(){
    return batchnode;
}