//
// Created by teunv on 1/18/2021.
//

#include "MergeClusterAction.h"

#include "../model/Model.h"
#include "../view2/CommandScrollArea.h"

#include <utility>

action::MergeClusterAction::MergeClusterAction(model::Model*             model,
                                               model::Cluster            target,
                                               model::Cluster            second,
                                               view2::CommandScrollArea* commandScrollArea)
    : m_model(model), m_target(std::move(target)), m_second(std::move(second)), m_commandScrollArea(commandScrollArea) {
    setText(QString("Merging clusters %1 and %2").arg(m_target.index()).arg(m_second.index()));
}

void action::MergeClusterAction::undo() {
    auto targetIt = m_model->clusterWithIndex(m_target.index());
    assert(targetIt != m_model->clusters().end());
    for (const auto& point : m_second.gridXY()) {
        assert(targetIt->contains(point));
        targetIt->removeGridXY(point);
    }
    m_model->clusters().emplace_back(m_second);
    m_commandScrollArea->addToLayout(std::move(m_commandEditBox));
}

void action::MergeClusterAction::redo() {
    auto secondIt = m_model->clusterWithIndex(m_second.index());
    assert(secondIt != m_model->clusters().end());
    auto targetIt = m_model->clusterWithIndex(m_target.index());
    assert(targetIt != m_model->clusters().end());
    targetIt->gridXY().merge(secondIt->gridXY());
    m_model->clusters().erase(secondIt);
    m_commandEditBox = m_commandScrollArea->removeFromLayout(m_second.index());
}

action::ACTION_TYPE action::MergeClusterAction::type() const {
    return ACTION_TYPE::MERGE_CLUSTERS;
}
