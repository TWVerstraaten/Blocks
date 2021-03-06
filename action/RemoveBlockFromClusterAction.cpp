//
// Created by teunv on 1/17/2021.
//

#include "RemoveBlockFromClusterAction.h"

#include "../model/Model.h"

#include <cassert>

namespace action {

    RemoveBlockFromClusterAction::RemoveBlockFromClusterAction(model::Model* model, size_t clusterIndex, const model::GridXy& gridXy)
        : m_model(model), m_clusterIndex(clusterIndex), m_gridXy(gridXy) {
        setText(QString("Remove block (%1,%2) from cluster %3").arg(m_gridXy.x()).arg(m_gridXy.y()).arg(m_clusterIndex));
    }

    void RemoveBlockFromClusterAction::undo() {
        auto it = m_model->clusterWithIndex(m_clusterIndex);
        assert(it != m_model->clusters().end());
        it->addGridXy(m_gridXy);
    }

    void RemoveBlockFromClusterAction::redo() {
        auto it = m_model->clusterWithIndex(m_clusterIndex);
        assert(it != m_model->clusters().end());
        it->removeGridXy(m_gridXy);
    }

    ACTION_TYPE RemoveBlockFromClusterAction::type() const {
        return ACTION_TYPE::REMOVE_BLOCK_FROM_CLUSTER;
    }
} // namespace action