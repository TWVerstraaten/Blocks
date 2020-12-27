//
// Created by pc on 27-12-20.
//

#include "ClearBlockAction.h"

#include <utility>

model::action::ClearBlockAction::ClearBlockAction(const size_t         cluster,
                                                  const model::GridXY& clearedBlock,
                                                  std::list<size_t>&&  createdClusters)
    : m_cluster(cluster), m_clearedBlock(clearedBlock), m_createdClusters(std::move(createdClusters)) {
}

void model::action::ClearBlockAction::undoAction(Application_Edit& applicationEdit) {
}

void model::action::ClearBlockAction::redoAction(Application_Edit& applicationEdit) {
}
