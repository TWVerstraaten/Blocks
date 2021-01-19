//
// Created by teunv on 1/19/2021.
//

#include "RemoveLevelBlockAction.h"

action::RemoveLevelBlockAction::RemoveLevelBlockAction(model::Model* model, model::DYNAMIC_BLOCK_TYPE type, const model::GridXy& gridXy)
    : AddLevelBlockAction(model, type, gridXy) {
}

action::RemoveLevelBlockAction::RemoveLevelBlockAction(model::Model* model, model::INSTANT_BLOCK_TYPE type, const model::GridXy& gridXy)
    : AddLevelBlockAction(model, type, gridXy) {
}

void action::RemoveLevelBlockAction::undo() {
    AddLevelBlockAction::redo();
}

void action::RemoveLevelBlockAction::redo() {
    AddLevelBlockAction::undo();
}

action::ACTION_TYPE action::RemoveLevelBlockAction::type() const {
    return ACTION_TYPE::REMOVE_LEVEL_BLOCK;
}
