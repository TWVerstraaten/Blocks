//
// Created by teunv on 1/19/2021.
//

#include "RemoveLevelBlockAction.h"

namespace action {
    RemoveLevelBlockAction::RemoveLevelBlockAction(model::Model* model, model::DYNAMIC_BLOCK_TYPE type, const model::GridXy& gridXy)
        : AddLevelBlockAction(model, type, gridXy) {
    }

    RemoveLevelBlockAction::RemoveLevelBlockAction(model::Model* model, model::INSTANT_BLOCK_TYPE type, const model::GridXy& gridXy)
        : AddLevelBlockAction(model, type, gridXy) {
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