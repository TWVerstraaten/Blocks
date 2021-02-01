//
// Created by teunv on 1/17/2021.
//

#include "DeleteClusterAction.h"

namespace action {
    DeleteClusterAction::DeleteClusterAction(view::widget::CentralWidget* centralWidget, const model::Cluster& cluster)
        : NewClusterAction(centralWidget, cluster) {
        setText(QString("Deleting cluster %1").arg(m_cluster.index()));
        m_blockInitial = false;
    }

    void DeleteClusterAction::undo() {
        NewClusterAction::redo();
    }

    void DeleteClusterAction::redo() {
        NewClusterAction::undo();
    }

    ACTION_TYPE DeleteClusterAction::type() const {
        return ACTION_TYPE::REMOVE_CLUSTER;
    }
} // namespace action