//
// Created by teunv on 1/18/2021.
//

#include "SplitDisconnectedAction.h"

#include "../model/Model.h"
#include "view/CommandScrollArea.h"

#include <global/defines.h>

action::SplitDisconnectedAction::SplitDisconnectedAction(model::Model* model, model::Cluster disconnected, view::CommandScrollArea* commandScrollArea)
    : m_model(model), m_disconnected(std::move(disconnected)), m_commandScrollArea(commandScrollArea) {
    setText(QString("Splitting cluster %1").arg(m_disconnected.index()));

    auto copy    = m_disconnected;
    m_components = copy.collectAllButFirstComponent();
    copy.buildSides();
    assert(copy.isConnected());
    m_components.emplace_back(copy);

    m_disconnectedCommandEditBox = m_commandScrollArea->removeFromLayout(m_disconnected.index());
    for (auto& it : m_components) {
        m_commandScrollArea->add(it);
        it.buildSides();
    }
    for (const auto& it : m_components) {
        m_componentCommandEditBoxes.emplace_back(m_commandScrollArea->removeFromLayout(it.index()));
    }
    m_commandScrollArea->addToLayout(std::move(m_disconnectedCommandEditBox));
}

void action::SplitDisconnectedAction::undo() {
    for (const auto& it : m_components) {
        auto toRemoveIt = m_model->clusterWithIndex(it.index());
        assert(toRemoveIt != m_model->clusters().end());
        m_model->clusters().erase(toRemoveIt);
        m_componentCommandEditBoxes.emplace_back(m_commandScrollArea->removeFromLayout(it.index()));
    }
    m_model->clusters().emplace_back(m_disconnected);
    m_commandScrollArea->addToLayout(std::move(m_disconnectedCommandEditBox));
}

void action::SplitDisconnectedAction::redo() {
    auto disconnectedIt = m_model->clusterWithIndex(m_disconnected.index());
    assert(disconnectedIt != m_model->clusters().end());
    m_model->clusters().erase(disconnectedIt);
    m_disconnectedCommandEditBox = m_commandScrollArea->removeFromLayout(m_disconnected.index());

    for (const auto& it : m_components) {
        m_model->clusters().emplace_back(it);
    }
    for (auto& it : m_componentCommandEditBoxes) {
        m_commandScrollArea->addToLayout(std::move(it));
    }
    assert(std::all_of(D_CIT(m_componentCommandEditBoxes), D_FUNC(box, box == nullptr)));
    m_componentCommandEditBoxes.clear();
}

action::ACTION_TYPE action::SplitDisconnectedAction::type() const {
    return ACTION_TYPE::ADD_CLUSTER;
}
