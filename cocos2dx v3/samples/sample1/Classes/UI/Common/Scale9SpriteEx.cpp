//
//  Scale9SpriteHorizontal.cpp
//  TestGame
//
//  Created by DucHQ on 8/4/14.
//
//

#include "Scale9SpriteEx.h"


Scale9SpriteEx::Scale9SpriteEx(){
    sprite9p = nullptr;
    batchnode = nullptr;
}

Scale9SpriteEx::~Scale9SpriteEx(){
    
}

void Scale9SpriteEx::initWithFile(const std::string& file, const Rect& rect,  const Rect& capInsets){
    setAnchorPoint( Vec2::ANCHOR_MIDDLE);
    sprite9p = new Scale9Sprite();
    
    batchnode = SpriteBatchNode::create(file, 9);
    sprite9p->initWithBatchNode(batchnode, rect, capInsets);
    sprite9p->setAnchorPoint( Vec2::ZERO);
    addChild(sprite9p);
    sprite9p->release();
}

void Scale9SpriteEx::setContentSize(const Size& size){
    Node::setContentSize( size);
    if (!sprite9p) {
        return;
    }
    Size sizeNext = size;
    Size sizeMin = sprite9p->getPreferredSize();
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

void Scale9SpriteEx::setColor(const Color3B& color){
    cocos2d::Node::setColor( color);
    if (sprite9p) {
        sprite9p->setColor( color);
    }
}

void Scale9SpriteEx::setOpacity(GLubyte opacity){
    cocos2d::Node::setOpacity( opacity);
    if (sprite9p) {
        sprite9p->setOpacity( opacity);
    }
}

ui::Scale9Sprite * Scale9SpriteEx::getScale9Sprite(){
    return sprite9p;
}

SpriteBatchNode* Scale9SpriteEx::getBatchNode(){
    return batchnode;
}