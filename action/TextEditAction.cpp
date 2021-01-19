//
// Created by teunv on 1/17/2021.
//

#include "TextEditAction.h"

#include "view/TextEdit.h"

action::TextEditAction::TextEditAction(view::TextEdit* textEdit) : m_textEdit(textEdit) {
    setText(QString("Text edit action"));
}

action::ACTION_TYPE action::TextEditAction::type() const {
    return ACTION_TYPE::TEXT_EDIT_ACTION;
}

void action::TextEditAction::undo() {
    m_textEdit->undo();
    m_textEdit->update();
    m_textEdit->setFocus();
}

void action::TextEditAction::redo() {
    if (not m_blockInitial) {
        m_textEdit->redo();
        m_textEdit->update();
        m_textEdit->setFocus();
    }
    m_blockInitial = false;
}
