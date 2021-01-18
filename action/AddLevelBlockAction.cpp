//
// Created by teunv on 1/18/2021.
//

#include "AddLevelBlockAction.h"

action::AddLevelBlockAction::AddLevelBlockAction(model::Model* model, model::DYNAMIC_BLOCK_TYPE type, const model::GridXY& gridXy)
    : m_type(type), m_gridXy(gridXy), m_model(model) {
}

action::AddLevelBlockAction::AddLevelBlockAction(model::Model* model, model::INSTANT_BLOCK_TYPE type, const model::GridXY& gridXy)
    : m_type(type), m_gridXy(gridXy), m_model(model) {
}

void action::AddLevelBlockAction::undo() {
    std::visit([this](const auto& a) { m_model->level().removeBlock(m_gridXy, a); }, m_type);
}

void action::AddLevelBlockAction::redo() {
    std::visit([this](const auto& a) { m_model->level().addBlock(m_gridXy, a); }, m_type);
}

action::ACTION_TYPE action::AddLevelBlockAction::type() const {
    return ACTION_TYPE::ADD_LEVEL_BLOCK;
}
