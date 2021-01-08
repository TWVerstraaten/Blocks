//
// Created by pc on 28-12-20.
//

#include "RemoveBlockFromClusterAction.h"

action::RemoveBlockFromClusterAction::RemoveBlockFromClusterAction(size_t clusterIndex, const model::GridXY& gridXy)
    : AddBlockToClusterAction(clusterIndex, gridXy) {
}

void action::RemoveBlockFromClusterAction::undoAction(app::ApplicationEdit& applicationEdit) {
    AddBlockToClusterAction::redoAction(applicationEdit);
}

void action::RemoveBlockFromClusterAction::redoAction(app::ApplicationEdit& applicationEdit) {
    AddBlockToClusterAction::undoAction(applicationEdit);
}

action::ACTION_TYPE action::RemoveBlockFromClusterAction::type() const {
    return ACTION_TYPE::REMOVE_BLOCK_FROM_CLUSTER;
}
