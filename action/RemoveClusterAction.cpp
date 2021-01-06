//
// Created by pc on 28-12-20.
//

#include "RemoveClusterAction.h"

#include "../app/Application_Edit.h"

action::RemoveClusterAction::RemoveClusterAction(const model::Cluster& cluster) : AddClusterAction(cluster) {
}

void action::RemoveClusterAction::undoAction(app::Application_Edit& applicationEdit) {
    AddClusterAction::redoAction(applicationEdit);
}

void action::RemoveClusterAction::redoAction(app::Application_Edit& applicationEdit) {
    AddClusterAction::undoAction(applicationEdit);
}
action::ACTION_TYPE action::RemoveClusterAction::type() const {
    return ACTION_TYPE::REMOVE_CLUSTER;
}
