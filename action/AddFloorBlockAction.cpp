//
// Created by teunv on 1/18/2021.
//

#include "AddFloorBlockAction.h"

#include "../model/Model.h"

namespace action {

    AddFloorBlockAction::AddFloorBlockAction(model::Model* model, model::FLOOR_BLOCK_TYPE type, const model::GridXy& gridXy)
        : m_type(type), m_gridXy(gridXy), m_model(model) {
    }

    void AddFloorBlockAction::undo() {
        assert(m_model->level().floorBlocks().find(m_gridXy) != m_model->level().floorBlocks().end());
        assert(m_model->level().floorBlocks().at(m_gridXy) == m_type);
        m_model->level().removeBlock(m_gridXy, m_type);
    }

    void AddFloorBlockAction::redo() {
        m_model->level().addBlock(m_gridXy, m_type);
    }

    ACTION_TYPE AddFloorBlockAction::type() const {
        return ACTION_TYPE::ADD_FLOOR_BLOCK;
    }
} // namespace action