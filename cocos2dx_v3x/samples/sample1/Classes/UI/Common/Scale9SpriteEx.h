//
//  Scale9SpriteHorizontal.h
//  TestGame
//
//  Created by DucHQ on 8/4/14.
//
//

#ifndef __TestGame__Scale9SpriteHorizontal__
#define __TestGame__Scale9SpriteHorizontal__

#include "Utilities.h"
#include "AppMacros.h"
#include "ResourceCommon.h"
#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;

class Scale9SpriteEx: public cocos2d::Node{
private:
    ui::Scale9Sprite* sprite9p;
    SpriteBatchNode* batchnode;
public:
    Scale9SpriteEx();
    virtual ~Scale9SpriteEx();
    
    virtual void initWithFile(const std::string& file, const Rect& rect,  const Rect& capInsets);
    
    virtual void setContentSize(const Size& contentSize);
    
    virtual void setColor(const Color3B& color);
    virtual void setOpacity(GLubyte opacity);
    
    ui::Scale9Sprite * getScale9Sprite();
    SpriteBatchNode* getBatchNode();
};
class Scale9SpriteEx2 : public ui::Scale9Sprite{
public:
    int listProtectedSize(){
        return _protectedChildren.size();
    }
    Node* listProtectedAt(int i){
        return _protectedChildren.at(i);
    }
    
    void setBlendFuncEx(const cocos2d::BlendFunc &blendFunc){
        _topLeftSprite->setBlendFunc(blendFunc);
        _topSprite->setBlendFunc(blendFunc);
        _topRightSprite->setBlendFunc(blendFunc);
        _leftSprite->setBlendFunc(blendFunc);
        _centerSprite->setBlendFunc(blendFunc);
        _rightSprite->setBlendFunc(blendFunc);
        _bottomLeftSprite->setBlendFunc(blendFunc);
        _bottomSprite->setBlendFunc(blendFunc);
        _bottomRightSprite->setBlendFunc(blendFunc);
    }
};

#endif /* defined(__TestGame__Scale9SpriteHorizontal__) */
