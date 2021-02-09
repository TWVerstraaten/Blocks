//
// Created by teunv on 1/18/2021.
//

#include "ChangeFloorBlockAction.h"

#include "../model/Level.h"

action::ChangeFloorBlockAction::ChangeFloorBlockAction(model::Level*           level,
                                                       model::FLOOR_BLOCK_TYPE newType,
                                                       model::FLOOR_BLOCK_TYPE oldType,
                                                       const model::GridXy&    gridXy)
    : m_newType(newType), m_oldType(oldType), m_gridXy(gridXy), m_level(level) {
}

void action::ChangeFloorBlockAction::undo() {
    assert(m_level->floorBlocks().find(m_gridXy) != m_level->floorBlocks().end());
    assert(m_level->floorBlocks().at(m_gridXy) != m_oldType);
    m_level->addBlock(m_gridXy, m_oldType);
}

void action::ChangeFloorBlockAction::redo() {
    assert(m_level->floorBlocks().find(m_gridXy) != m_level->floorBlocks().end());
    assert(m_level->floorBlocks().at(m_gridXy) != m_newType);
    m_level->addBlock(m_gridXy, m_newType);
}

action::ACTION_TYPE action::ChangeFloorBlockAction::type() const {
    return action::ACTION_TYPE::CHANGE_FLOOR_BLOCK;
}
