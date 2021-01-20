//
// Created by teunv on 1/19/2021.
//

#include "RemoveFloorBlockAction.h"

namespace action {
    RemoveFloorBlockAction::RemoveFloorBlockAction(model::Model* model, model::FLOOR_BLOCK_TYPE type, const model::GridXy& gridXy)
        : AddFloorBlockAction(model, type, gridXy) {
    }

    void RemoveFloorBlockAction::undo() {
        AddFloorBlockAction::redo();
    }

    void RemoveFloorBlockAction::redo() {
        AddFloorBlockAction::undo();
    }
    ACTION_TYPE RemoveFloorBlockAction::type() const {
        return ACTION_TYPE::REMOVE_FLOOR_BLOCK;
    }
} // namespace action