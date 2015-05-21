//
//  TextBoxEx.h
//  TestGame
//
//  Created by DucHQ on 8/14/14.
//
//

#ifndef __TestGame__TextBoxEx__
#define __TestGame__TextBoxEx__

#include "Scale9SpriteEx.h"

class TextBoxEx : public CCEditBox, public CCEditBoxDelegate{
private:
    CCEditBoxDelegate*  delegate;
    bool isCheckingVisible;
    bool isKeyboardEnable;
    CCPoint posLast;
    
    bool isSchedule;
    
public:
    TextBoxEx();
    virtual ~TextBoxEx();
    void initWith( const CCSize& size = CCSize(319, 40));
    
    void setDelegate(CCEditBoxDelegate* pDelegate);
    
//    virtual void visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;
    virtual void visit();
    
    //EditBoxDelegate
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
    
    void runCheckVisible();
    void checkVisible(float dt);
    
    void enableKeyboard(const bool& b);
};

#endif /* defined(__TestGame__TextBoxEx__) */
