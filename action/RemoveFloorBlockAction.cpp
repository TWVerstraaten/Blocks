//
// Created by teunv on 1/19/2021.
//

#include "RemoveFloorBlockAction.h"

action::RemoveFloorBlockAction::RemoveFloorBlockAction(model::Model* model, model::FLOOR_BLOCK_TYPE type, const model::GridXy& gridXy)
    : AddFloorBlockAction(model, type, gridXy) {
}

void action::RemoveFloorBlockAction::undo() {
    AddFloorBlockAction::redo();
}

void action::RemoveFloorBlockAction::redo() {
    AddFloorBlockAction::undo();
}
action::ACTION_TYPE action::RemoveFloorBlockAction::type() const {
    return action::ACTION_TYPE::REMOVE_FLOOR_BLOCK;
}
