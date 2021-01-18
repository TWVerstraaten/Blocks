//
// Created by teunv on 1/17/2021.
//

#include "GenericTextEditAction.h"

#include "../view2/TextEdit.h"

action::GenericTextEditAction::GenericTextEditAction(view2::TextEdit* mTextEdit) : m_textEdit(mTextEdit) {
    setText(QString("Text edit action"));
}

action::ACTION_TYPE action::GenericTextEditAction::type() const {
    return ACTION_TYPE::GENERIC_TEXT_EDIT_ACTION;
}

void action::GenericTextEditAction::undo() {
    m_textEdit->undo();
    m_textEdit->update();
    m_textEdit->setFocus();
}

void action::GenericTextEditAction::redo() {
    if (not m_blockInitial) {
        m_textEdit->redo();
        m_textEdit->update();
        m_textEdit->setFocus();
    }
    m_blockInitial = false;
}
