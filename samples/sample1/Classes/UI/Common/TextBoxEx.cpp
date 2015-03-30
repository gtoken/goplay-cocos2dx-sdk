//
//  TextBoxEx.cpp
//  TestGame
//
//  Created by DucHQ on 8/14/14.
//
//

#include "TextBoxEx.h"

TextBoxEx::TextBoxEx(){
    delegate = nullptr;
    isCheckingVisible = false;
    isKeyboardEnable = true;
}

TextBoxEx::~TextBoxEx(){
    
}

void TextBoxEx::initWith( const Size& size){
    ui::Scale9Sprite * back = Scale9Sprite::create(s_pPathDotaInputName, Rect::ZERO, s_pPathDotaInputName_Rect);
    initWithSizeAndBackgroundSprite(size, back);
    back->setScale(1.1f);
    setFontName(s_pPathFontCommonBold);
    setPlaceholderFontName(s_pPathFontCommonBold);
    setPlaceholderFontColor(Color3B(200, 200, 200));
    EditBox::setDelegate(this);
    runCheckVisible();
}

void TextBoxEx::setDelegate(EditBoxDelegate* pDelegate){
    delegate = pDelegate;
}

void TextBoxEx::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags){
    runCheckVisible();
    /*
    if ( GameControler::sharedGameControler()->isLoading()) {
        enableKeyboard( false);
    }else{
        enableKeyboard( true);
    }
     */
    enableKeyboard( true);
    EditBox::visit(renderer, parentTransform, parentFlags);
}

void TextBoxEx::editBoxEditingDidBegin(EditBox* editBox){
    if (delegate) {
        delegate->editBoxEditingDidBegin(editBox);
    }
//    log("editBoxEditingDidBegin");
}

void TextBoxEx::editBoxEditingDidEnd(EditBox* editBox){
    if (delegate) {
        delegate->editBoxEditingDidEnd(editBox);
    }
//    log("editBoxEditingDidEnd");
}

void TextBoxEx::editBoxTextChanged(EditBox* editBox, const std::string& text){
    if (delegate) {
        delegate->editBoxTextChanged(editBox, text);
    }
//    log("editBoxTextChanged");
}

void TextBoxEx::editBoxReturn(EditBox* editBox){
    if (delegate) {
        delegate->editBoxReturn(editBox);
    }
//    log("editBoxReturn");
}

void TextBoxEx::runCheckVisible(){
    if (isCheckingVisible) {
        return;
    }
    if(!this->isScheduled(schedule_selector(TextBoxEx::checkVisible))){
        isCheckingVisible = true;
        this->schedule(schedule_selector(TextBoxEx::checkVisible));
    }
}

void TextBoxEx::checkVisible(float dt){
    isCheckingVisible = true;
    Node * nodeParent = getParent();
    while (nodeParent) {
        if( !nodeParent->isVisible()){
            //InVi
            this->unschedule(schedule_selector(TextBoxEx::checkVisible));
            enableKeyboard( false);
            isCheckingVisible = false;
            return;
        }
        nodeParent = nodeParent->getParent();
    }
}

void TextBoxEx::enableKeyboard(const bool& b){
    if (b!=isKeyboardEnable) {
        isKeyboardEnable = b;
        if (!isKeyboardEnable) {
            if (_editBoxImpl != nullptr)
            {
//                _editBoxImpl->setVisible(false);
                ui::EditBox::setVisible(false);
            }
//            posLast = getPosition();
//            Size visibleSize = Director::getInstance()->getVisibleSize();
//            Vec2 pos = this->convertToNodeSpace( Vec2(0, visibleSize.height));
//            this->setPosition(pos);
        }else{
            if (_editBoxImpl != nullptr)
            {
//                _editBoxImpl->setVisible(true);
                ui::EditBox::setVisible(true);
            }
//            this->setPosition(posLast);
        }
    }
}