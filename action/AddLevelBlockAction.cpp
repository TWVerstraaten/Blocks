//
// Created by teunv on 1/18/2021.
//

#include "AddLevelBlockAction.h"

#include "../model/Level.h"

namespace action {

    AddLevelBlockAction::AddLevelBlockAction(model::Level* level, model::DYNAMIC_BLOCK_TYPE type, const model::GridXy& gridXy)
        : m_type(type), m_gridXy(gridXy), m_level(level) {
        setText(QString("Adding dynamic block at (%1, %2)").arg(gridXy.x()).arg(gridXy.y()));
    }

    AddLevelBlockAction::AddLevelBlockAction(model::Level* level, model::INSTANT_BLOCK_TYPE type, const model::GridXy& gridXy)
        : m_type(type), m_gridXy(gridXy), m_level(level) {
        setText(QString("Adding instant block at (%1, %2)").arg(gridXy.x()).arg(gridXy.y()));
    }

    void AddLevelBlockAction::undo() {
        std::visit([this](const auto& a) { m_level->removeBlock(m_gridXy, a); }, m_type);
    }

    void AddLevelBlockAction::redo() {
        std::visit([this](const auto& a) { m_level->addBlock(m_gridXy, a); }, m_type);
    }

    ACTION_TYPE AddLevelBlockAction::type() const {
        return ACTION_TYPE::ADD_LEVEL_BLOCK;
    }
} // namespace action