//
// Created by pc on 29-12-20.
//

#include "RemoveLevelBlockAction.h"

#include "../app/Application_Edit.h"
#include "../global/defines.h"

action::RemoveLevelBlockAction::RemoveLevelBlockAction(
    const std::variant<model::DYNAMIC_BLOCK_TYPE, model::INSTANT_BLOCK_TYPE, model::FLOOR_BLOCK_TYPE>& blockType, const model::GridXY& point)
    : m_blockType(blockType), m_point(point) {
}

void action::RemoveLevelBlockAction::undoAction(app::Application_Edit& applicationEdit) {
    std::visit(_FUNC_(type, applicationEdit.model()->level().addBlock(m_point, type)), m_blockType);
}

void action::RemoveLevelBlockAction::redoAction(app::Application_Edit& applicationEdit) {
    std::visit(_FUNC_(type, applicationEdit.model()->level().removeBlock(m_point)), m_blockType);
}

action::ACTION_TYPE action::RemoveLevelBlockAction::type() const {
    return ACTION_TYPE::REMOVE_LEVEL_BLOCK_ACTION;
}
