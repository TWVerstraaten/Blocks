//
// Created by pc on 28-12-20.
//

#include "AddBlockToClusterAction.h"

#include "../app/ApplicationEdit.h"

#include <cassert>

action::AddBlockToClusterAction::AddBlockToClusterAction(size_t clusterIndex, const model::GridXY& gridXy)
    : m_clusterIndex(clusterIndex), m_gridXY(gridXy) {
}

void action::AddBlockToClusterAction::undoAction(app::ApplicationEdit& applicationEdit) {
    auto it = applicationEdit.model()->clusterWithIndex(m_clusterIndex);
    assert(it != applicationEdit.model()->clusters().end());
    it->removeBLock(m_gridXY);
}

void action::AddBlockToClusterAction::redoAction(app::ApplicationEdit& applicationEdit) {
    auto it = applicationEdit.model()->clusterWithIndex(m_clusterIndex);
    assert(it != applicationEdit.model()->clusters().end());
    it->addGridXY(m_gridXY);
}

action::ACTION_TYPE action::AddBlockToClusterAction::type() const {
    return ACTION_TYPE::ADD_BLOCK_TO_CLUSTER;
}
