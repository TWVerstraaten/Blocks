//
// Created by pc on 27-12-20.
//

#include "AddClusterAction.h"

#include "../../Application/Application_Edit.h"

#include <cassert>

model::action::AddClusterAction::AddClusterAction(const model::Cluster& index, const model::GridXY& gridXy)
    : m_cluster(index), m_gridXy(gridXy) {
}

void model::action::AddClusterAction::undoAction(Application_Edit& applicationEdit) {
    std::cout << " undo\n";
    auto it = applicationEdit.model()->clusterWithIndex(m_cluster.index());
    assert(it != applicationEdit.model()->clusters().end());
    applicationEdit.model()->clusters().erase(it);
}

void model::action::AddClusterAction::redoAction(Application_Edit& applicationEdit) {
    applicationEdit.model()->clusters().push_back(m_cluster);
}
