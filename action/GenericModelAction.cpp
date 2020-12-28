//
// Created by pc on 27-12-20.
//

#include "GenericModelAction.h"

#include "../app/Application_Edit.h"

action::GenericModelAction::GenericModelAction(const model::Model& oldModel, const model::Model& newModel)
    : m_old(oldModel), m_new(newModel) {
}

void action::GenericModelAction::undoAction(app::Application_Edit& applicationEdit) {
    *applicationEdit.model() = m_old;
}

void action::GenericModelAction::redoAction(app::Application_Edit& applicationEdit) {
    *applicationEdit.model() = m_new;
}
