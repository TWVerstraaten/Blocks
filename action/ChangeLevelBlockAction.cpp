//
// Created by teunv on 1/18/2021.
//

#include "ChangeLevelBlockAction.h"

action::ChangeLevelBlockAction::ChangeLevelBlockAction(model::Model*                                                             model,
                                                       const std::variant<model::DYNAMIC_BLOCK_TYPE, model::INSTANT_BLOCK_TYPE>& newType,
                                                       const std::variant<model::DYNAMIC_BLOCK_TYPE, model::INSTANT_BLOCK_TYPE>& oldType,
                                                       const model::GridXy&                                                      gridXy)
    : m_model(model), m_newType(newType), m_oldType(oldType), m_gridXy(gridXy) {
}

void action::ChangeLevelBlockAction::undo() {
    std::visit([this](const auto& a) { m_model->level().removeBlock(m_gridXy, a); }, m_newType);
    std::visit([this](const auto& a) { m_model->level().addBlock(m_gridXy, a); }, m_oldType);
}

void action::ChangeLevelBlockAction::redo() {
    std::visit([this](const auto& a) { m_model->level().removeBlock(m_gridXy, a); }, m_oldType);
    std::visit([this](const auto& a) { m_model->level().addBlock(m_gridXy, a); }, m_newType);
}

action::ACTION_TYPE action::ChangeLevelBlockAction::type() const {
    return ACTION_TYPE::CHANGE_FLOOR_BLOCK;
}
