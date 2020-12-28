//
// Created by pc on 27-12-20.
//

#include "AddClusterAction.h"

#include "../app/Application_Edit.h"

#include <cassert>

action::AddClusterAction::AddClusterAction(model::Cluster cluster) : m_cluster(std::move(cluster)) {
}

void action::AddClusterAction::undoAction(app::Application_Edit& applicationEdit) {
    auto it = applicationEdit.model()->clusterWithIndex(m_cluster.index());
    assert(it != applicationEdit.model()->clusters().end());
    applicationEdit.model()->clusters().erase(it);
}

void action::AddClusterAction::redoAction(app::Application_Edit& applicationEdit) {
    assert(applicationEdit.model()->clusterWithIndex(m_cluster.index()) == applicationEdit.model()->clusters().end());
    applicationEdit.model()->clusters().push_back(m_cluster);
}

action::ACTION_TYPE action::AddClusterAction::type() const {
    return ACTION_TYPE::ADD_CLUSTER;
}
