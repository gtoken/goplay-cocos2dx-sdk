//
//  TextBoxEx.cpp
//  TestGame
//
//  Created by DucHQ on 8/14/14.
//
//

#include "TextBoxEx.h"

TextBoxEx::TextBoxEx(){
    delegate = NULL;
    isCheckingVisible = false;
    isKeyboardEnable = true;
    isSchedule = false;
}

TextBoxEx::~TextBoxEx(){
    
}

void TextBoxEx::initWith( const CCSize& size){
    CCScale9Sprite * back = CCScale9Sprite::create(s_pPathDotaInputName, CCRectZero, s_pPathDotaInputName_Rect);
    initWithSizeAndBackgroundSprite(size, back);
    back->setScale(1.1f);
    setFontName(s_pPathFontCommonBold);
    setPlaceholderFontName(s_pPathFontCommonBold);
    setPlaceholderFontColor(ccc3(200, 200, 200));
    CCEditBox::setDelegate(this);
    runCheckVisible();
}

void TextBoxEx::setDelegate(CCEditBoxDelegate* pDelegate){
    delegate = pDelegate;
}

void TextBoxEx::visit(){
    runCheckVisible();
    /*
    if ( GameControler::sharedGameControler()->isLoading()) {
        enableKeyboard( false);
    }else{
        enableKeyboard( true);
    }
     */
    enableKeyboard( true);
    CCEditBox::visit();
}

void TextBoxEx::editBoxEditingDidBegin(CCEditBox* editBox){
    if (delegate) {
        delegate->editBoxEditingDidBegin(editBox);
    }
//    log("editBoxEditingDidBegin");
}

void TextBoxEx::editBoxEditingDidEnd(CCEditBox* editBox){
    if (delegate) {
        delegate->editBoxEditingDidEnd(editBox);
    }
//    log("editBoxEditingDidEnd");
}

void TextBoxEx::editBoxTextChanged(CCEditBox* editBox, const std::string& text){
    if (delegate) {
        delegate->editBoxTextChanged(editBox, text);
    }
//    log("editBoxTextChanged");
}

void TextBoxEx::editBoxReturn(CCEditBox* editBox){
    if (delegate) {
        delegate->editBoxReturn(editBox);
    }
//    log("editBoxReturn");
}

void TextBoxEx::runCheckVisible(){
    if (isCheckingVisible) {
        return;
    }
    
    if (!isSchedule) {
        isCheckingVisible = true;
        isSchedule = true;
        this->schedule(schedule_selector(TextBoxEx::checkVisible));
    }
}

void TextBoxEx::checkVisible(float dt){
    isCheckingVisible = true;
    CCNode * nodeParent = getParent();
    while (nodeParent) {
        if( !nodeParent->isVisible()){
            //InVi
            isSchedule = false;
            this->unschedule(schedule_selector(TextBoxEx::checkVisible));
            enableKeyboard( false);
            isCheckingVisible = false;
            return;
        }
        nodeParent = nodeParent->getParent();
    }
}

void TextBoxEx::enableKeyboard(const bool& b)
{
    if (b!=isKeyboardEnable)
    {
        isKeyboardEnable = b;
        if (!isKeyboardEnable)
        {
            if (m_pEditBoxImpl != NULL)
            {
                CCEditBox::setVisible(false);
            }
        }
        else
        {
            if (m_pEditBoxImpl != NULL)
            {
                CCEditBox::setVisible(true);
            }
        }
    }
}