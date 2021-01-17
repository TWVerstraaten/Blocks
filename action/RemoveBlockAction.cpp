//
// Created by teunv on 1/17/2021.
//

#include "RemoveBlockAction.h"

#include "../model/Model.h"

#include <cassert>

action::RemoveBlockAction::RemoveBlockAction(model::Model* model, size_t clusterIndex, const model::GridXY& gridXy)
    : m_model(model), m_clusterIndex(clusterIndex), m_gridXy(gridXy) {
}

void action::RemoveBlockAction::undo() {
    auto it = m_model->clusterWithIndex(m_clusterIndex);
    assert(it != m_model->clusters().end());
    it->addGridXY(m_gridXy);
}

void action::RemoveBlockAction::redo() {
    auto it = m_model->clusterWithIndex(m_clusterIndex);
    assert(it != m_model->clusters().end());
    it->removeGridXY(m_gridXy);
}

action::ACTION_TYPE action::RemoveBlockAction::type() const {
    return ACTION_TYPE::REMOVE_BLOCK_FROM_CLUSTER;
}
