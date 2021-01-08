//
// Created by pc on 27-12-20.
//

#include "GenericModelAction.h"

#include "../app/ApplicationEdit.h"

action::GenericModelAction::GenericModelAction(const model::Model& oldModel, const model::Model& newModel)
    : m_old(oldModel), m_new(newModel) {
}

void action::GenericModelAction::undoAction(app::ApplicationEdit& applicationEdit) {
    *applicationEdit.model() = m_old;
}

void action::GenericModelAction::redoAction(app::ApplicationEdit& applicationEdit) {
    *applicationEdit.model() = m_new;
}

action::ACTION_TYPE action::GenericModelAction::type() const {
    return ACTION_TYPE::GENERIC_MODEL;
}
