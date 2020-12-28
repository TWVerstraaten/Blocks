//
// Created by pc on 27-12-20.
//

#include "GenericModelAction.h"

#include "../Application/Application_Edit.h"

action::GenericModelAction::GenericModelAction(const model::Model& old, const model::Model& aNew) : m_old(old), m_new(aNew) {
}

void action::GenericModelAction::undoAction(Application_Edit& applicationEdit) {
    *applicationEdit.model() = m_old;
}

void action::GenericModelAction::redoAction(Application_Edit& applicationEdit) {
    *applicationEdit.model() = m_new;
}
