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
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;

class Scale9SpriteEx: public CCNode {
private:
    CCScale9Sprite* sprite9p;
    CCSpriteBatchNode* batchnode;
public:
    Scale9SpriteEx();
    virtual ~Scale9SpriteEx();
    
    virtual void initWithFile(const std::string& file, const CCRect& rect,  const CCRect& capInsets);
    
    virtual void setContentSize(const CCSize& contentSize);
    
    virtual void setColor(const ccColor3B& color);
    virtual void setOpacity(GLubyte opacity);
    
    CCScale9Sprite * getScale9Sprite();
    CCSpriteBatchNode* getBatchNode();
};

class Scale9SpriteEx2 : public CCScale9Sprite {
public:
    int listProtectedSize(){
//        return _protectedChildren.size();
        
        // This feature not support in v2
        return 0;
    }
    
    CCNode* listProtectedAt(int i){
//        return _protectedChildren.at(i);
        
        // This feature not support in v2
        return NULL;
    }
    
    void setBlendFuncEx(const ccBlendFunc &blendFunc){
        _topLeft->setBlendFunc(blendFunc);
        _top->setBlendFunc(blendFunc);
        _topRight->setBlendFunc(blendFunc);
        _left->setBlendFunc(blendFunc);
        _centre->setBlendFunc(blendFunc);
        _right->setBlendFunc(blendFunc);
        _bottomLeft->setBlendFunc(blendFunc);
        _bottom->setBlendFunc(blendFunc);
        _bottomRight->setBlendFunc(blendFunc);
    }
};

#endif /* defined(__TestGame__Scale9SpriteHorizontal__) */
