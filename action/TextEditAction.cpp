//
// Created by teunv on 1/17/2021.
//

#include "TextEditAction.h"

#include "../view/widget/TextEdit.h"

namespace action {
    TextEditAction::TextEditAction(view::widget::TextEdit* textEdit) : m_textEdit(textEdit) {
        setText(QString("Text edit action"));
    }

    ACTION_TYPE TextEditAction::type() const {
        return ACTION_TYPE::TEXT_EDIT_ACTION;
    }

    void TextEditAction::undo() {
        m_textEdit->QPlainTextEdit::undo();
        m_textEdit->setFocus();
    }

    void TextEditAction::redo() {
        if (not m_blockInitial) {
            m_textEdit->QPlainTextEdit::redo();
            m_textEdit->setFocus();
        }
        m_blockInitial = false;
    }
} // namespace action