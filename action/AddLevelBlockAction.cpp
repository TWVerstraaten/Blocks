//
// Created by pc on 29-12-20.
//

#include "AddLevelBlockAction.h"

#include "../app/Application_Edit.h"
#include "../global/overloaded.h"

action::AddLevelBlockAction::AddLevelBlockAction(
    std::set<std::variant<model::DYNAMIC_BLOCK_TYPE, model::INSTANT_BLOCK_TYPE, model::FLOOR_BLOCK_TYPE>> blockTypes,
    const model::GridXY&                                                                                  point)
    : m_blockTypes(std::move(blockTypes)), m_point(point) {
}

void action::AddLevelBlockAction::undoAction(app::Application_Edit& applicationEdit) {
    for (const auto& it : m_blockTypes) {
        std::visit(overloaded{[&](const auto type) { applicationEdit.model()->level().removeBlock(m_point, type); }}, it);
    }
}

void action::AddLevelBlockAction::redoAction(app::Application_Edit& applicationEdit) {
    for (const auto& it : m_blockTypes) {
        std::visit(overloaded{[&](const auto type) { applicationEdit.model()->level().addBlock(m_point, type); }}, it);
    }
}

action::ACTION_TYPE action::AddLevelBlockAction::type() const {
    return ACTION_TYPE::ADD_LEVEL_BLOCK_ACTION;
}
