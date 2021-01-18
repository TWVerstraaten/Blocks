//
// Created by teunv on 1/18/2021.
//

#include "AddBlockAction.h"

action::AddBlockAction::AddBlockAction(model::Model* model, size_t clusterIndex, const model::GridXY& gridXy)
    : RemoveBlockAction(model, clusterIndex, gridXy) {
    setText(QString("Add block (%1,%2) to cluster %3").arg(m_gridXy.x()).arg(m_gridXy.y()).arg(m_clusterIndex));
    m_blockInitial = false;
}

void action::AddBlockAction::undo() {
    RemoveBlockAction::redo();
}

void action::AddBlockAction::redo() {
    RemoveBlockAction::undo();
}

action::ACTION_TYPE action::AddBlockAction::type() const {
    return ACTION_TYPE::ADD_BLOCK_TO_CLUSTER;
}
