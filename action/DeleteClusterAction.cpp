//
// Created by teunv on 1/17/2021.
//

#include "DeleteClusterAction.h"

action::DeleteClusterAction::DeleteClusterAction(view::CentralWidget* centralWidget, const model::Cluster& cluster)
    : NewClusterAction(centralWidget, cluster) {
    setText(QString("Deleting cluster %1").arg(m_cluster.index()));
    m_blockInitial = false;
}

void action::DeleteClusterAction::undo() {
    NewClusterAction::redo();
}

void action::DeleteClusterAction::redo() {
    NewClusterAction::undo();
}

action::ACTION_TYPE action::DeleteClusterAction::type() const {
    return ACTION_TYPE::REMOVE_CLUSTER;
}
