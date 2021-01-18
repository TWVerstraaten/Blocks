//
// Created by teunv on 1/17/2021.
//

#include "RemoveBlockFromClusterAction.h"

#include "../model/Model.h"

#include <cassert>

action::RemoveBlockFromClusterAction::RemoveBlockFromClusterAction(model::Model* model, size_t clusterIndex, const model::GridXY& gridXy)
    : m_model(model), m_clusterIndex(clusterIndex), m_gridXy(gridXy) {
    setText(QString("Remove block (%1,%2) from cluster %3").arg(m_gridXy.x()).arg(m_gridXy.y()).arg(m_clusterIndex));
}

void action::RemoveBlockFromClusterAction::undo() {
    auto it = m_model->clusterWithIndex(m_clusterIndex);
    assert(it != m_model->clusters().end());
    it->addGridXY(m_gridXy);
}

void action::RemoveBlockFromClusterAction::redo() {
    auto it = m_model->clusterWithIndex(m_clusterIndex);
    assert(it != m_model->clusters().end());
    it->removeGridXY(m_gridXy);
}

action::ACTION_TYPE action::RemoveBlockFromClusterAction::type() const {
    return ACTION_TYPE::REMOVE_BLOCK_FROM_CLUSTER;
}
