//
// Created by pc on 28-12-20.
//

#include "RemoveClusterAction.h"

#include "../app/ApplicationEdit.h"

action::RemoveClusterAction::RemoveClusterAction(model::Cluster cluster) : AddClusterAction(std::move(cluster)) {
}

void action::RemoveClusterAction::undoAction(app::ApplicationEdit& applicationEdit) {
    AddClusterAction::redoAction(applicationEdit);
}

void action::RemoveClusterAction::redoAction(app::ApplicationEdit& applicationEdit) {
    AddClusterAction::undoAction(applicationEdit);
}

action::ACTION_TYPE action::RemoveClusterAction::type() const {
    return ACTION_TYPE::REMOVE_CLUSTER;
}
