//
// Created by teunv on 1/19/2021.
//

#include "RemoveLevelBlockAction.h"

namespace action {
    RemoveLevelBlockAction::RemoveLevelBlockAction(model::Level* level, model::DYNAMIC_BLOCK_TYPE type, const model::GridXy& gridXy)
        : AddLevelBlockAction(level, type, gridXy) {
    }

    RemoveLevelBlockAction::RemoveLevelBlockAction(model::Level* level, model::INSTANT_BLOCK_TYPE type, const model::GridXy& gridXy)
        : AddLevelBlockAction(level, type, gridXy) {
    }

    void RemoveLevelBlockAction::undo() {
        AddLevelBlockAction::redo();
    }

    void RemoveLevelBlockAction::redo() {
        AddLevelBlockAction::undo();
    }

    ACTION_TYPE RemoveLevelBlockAction::type() const {
        return ACTION_TYPE::REMOVE_LEVEL_BLOCK;
    }
} // namespace action