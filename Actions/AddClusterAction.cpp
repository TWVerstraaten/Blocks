//
// Created by pc on 27-12-20.
//

#include "AddClusterAction.h"

#include "../Application/Application_Edit.h"

#include <cassert>
#include <utility>

action::AddClusterAction::AddClusterAction(model::Cluster cluster) : m_cluster(std::move(cluster)) {
}

void action::AddClusterAction::undoAction(Application_Edit& applicationEdit) {
    auto it = applicationEdit.model()->clusterWithIndex(m_cluster.index());
    assert(it != applicationEdit.model()->clusters().end());
    applicationEdit.model()->clusters().erase(it);
}

void action::AddClusterAction::redoAction(Application_Edit& applicationEdit) {
    applicationEdit.model()->clusters().push_back(m_cluster);
}
