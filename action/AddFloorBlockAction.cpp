//
// Created by teunv on 1/18/2021.
//

#include "AddFloorBlockAction.h"

#include "../model/Level.h"

namespace action {

    AddFloorBlockAction::AddFloorBlockAction(model::Level* level, model::FLOOR_BLOCK_TYPE type, const model::GridXy& gridXy)
        : m_type(type), m_gridXy(gridXy), m_level(level) {
    }

    void AddFloorBlockAction::undo() {
        assert(m_level->floorBlocks().find(m_gridXy) != m_level->floorBlocks().end());
        assert(m_level->floorBlocks().at(m_gridXy) == m_type);
        m_level->removeBlock(m_gridXy, m_type);
    }

    void AddFloorBlockAction::redo() {
        m_level->addBlock(m_gridXy, m_type);
    }

    ACTION_TYPE AddFloorBlockAction::type() const {
        return ACTION_TYPE::ADD_FLOOR_BLOCK;
    }
} // namespace action