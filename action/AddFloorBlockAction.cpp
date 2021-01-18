//
// Created by teunv on 1/18/2021.
//

#include "AddFloorBlockAction.h"

action::AddFloorBlockAction::AddFloorBlockAction(model::Model* model, model::FLOOR_BLOCK_TYPE type, const model::GridXY& gridXy)
    : m_type(type), m_gridXy(gridXy), m_model(model) {
}

void action::AddFloorBlockAction::undo() {
    assert(m_model->level().floorBlocks().find(m_gridXy) != m_model->level().floorBlocks().end());
    assert(m_model->level().floorBlocks().at(m_gridXy) == m_type);
    m_model->level().removeBlock(m_gridXy, m_type);
}

void action::AddFloorBlockAction::redo() {
    m_model->level().addBlock(m_gridXy, m_type);
}

action::ACTION_TYPE action::AddFloorBlockAction::type() const {
    return ACTION_TYPE::ADD_FLOOR_BLOCK;
}
