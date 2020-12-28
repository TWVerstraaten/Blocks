//
// Created by pc on 28-12-20.
//

#include "GenericCommandEditBoxAction.h"

#include "../app/Application_Edit.h"
#include "../view/widget/ScrollArea.h"

#include <cassert>

action::GenericCommandEditBoxAction::GenericCommandEditBoxAction(const view::widget::CommandEditBox& oldCommandEditBox,
                                                                 const view::widget::CommandEditBox& newCommandEditBox)
    : m_index(oldCommandEditBox.index()), m_old(oldCommandEditBox), m_new(newCommandEditBox) {
    assert(m_old.index() == m_new.index());
}

void action::GenericCommandEditBoxAction::undoAction(app::Application_Edit& applicationEdit) {
    auto it = applicationEdit.scrollArea()->findCommandEditBox(m_index);
    assert(it != applicationEdit.scrollArea()->children().end());
    *it = m_old;
}

void action::GenericCommandEditBoxAction::redoAction(app::Application_Edit& applicationEdit) {
    auto it = applicationEdit.scrollArea()->findCommandEditBox(m_index);
    assert(it != applicationEdit.scrollArea()->children().end());
    *it = m_new;
}
