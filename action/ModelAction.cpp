//
// Created by teunv on 1/17/2021.
//

#include "ModelAction.h"

#include "../view2/CommandScrollArea.h"

#include <cassert>

action::ModelAction::ModelAction(model::Model* model, model::Model oldCopy, model::Model newCopy, view2::CommandScrollArea* commandScrollArea)
    : m_model(model), m_old(std::move(oldCopy)), m_new(std::move(newCopy)), m_commandScrollArea(commandScrollArea) {
}

void action::ModelAction::undoAction(app::ApplicationEdit& applicationEdit) {
    assert(false);
}

void action::ModelAction::redoAction(app::ApplicationEdit& applicationEdit) {
    assert(false);
}

void action::ModelAction::undoAction() {
    *m_model = m_old;
    m_commandScrollArea->removeUnnecessary(m_model->clusters());
}

void action::ModelAction::redoAction() {
    *m_model = m_new;
    m_commandScrollArea->addNeeded(m_model->clusters());
}

action::ACTION_TYPE action::ModelAction::type() const {
    return ACTION_TYPE::ADD_CLUSTER;
}
