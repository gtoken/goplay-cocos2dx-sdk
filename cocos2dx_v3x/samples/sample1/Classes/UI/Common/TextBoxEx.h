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

class TextBoxEx : public ui::EditBox, public ui::EditBoxDelegate{
private:
    EditBoxDelegate*  delegate;
    bool isCheckingVisible;
    bool isKeyboardEnable;
    Vec2 posLast;
public:
    TextBoxEx();
    virtual ~TextBoxEx();
    void initWith( const Size& size = Size(319, 40));
    
    void setDelegate(EditBoxDelegate* pDelegate);
    
    virtual void visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;
    
    //EditBoxDelegate
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
    
    void runCheckVisible();
    void checkVisible(float dt);
    
    void enableKeyboard(const bool& b);
};

#endif /* defined(__TestGame__TextBoxEx__) */
