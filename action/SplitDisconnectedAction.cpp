//
// Created by teunv on 1/18/2021.
//

#include "SplitDisconnectedAction.h"

#include "../misc/defines.h"
#include "../model/Model.h"
#include "view/widget/CommandScrollArea.h"

namespace action {

    SplitDisconnectedAction::SplitDisconnectedAction(model::Model* model, model::Cluster disconnected, view::CommandScrollArea* commandScrollArea)
        : m_model(model), m_disconnected(std::move(disconnected)), m_commandScrollArea(commandScrollArea) {
        setText(QString("Splitting cluster %1").arg(m_disconnected.index()));

        auto copy    = m_disconnected;
        m_components = copy.collectAllButFirstComponent();
        assert(copy.isConnected());
        m_components.emplace_back(copy);
    }

    void SplitDisconnectedAction::undo() {
        for (const auto& it : m_components) {
            auto toRemoveIt = m_model->clusterWithIndex(it.index());
            assert(toRemoveIt != m_model->clusters().end());
            m_model->clusters().erase(toRemoveIt);
            m_commandScrollArea->removeFromLayout(it.index());
        }
        m_model->clusters().emplace_back(m_disconnected);
        m_commandScrollArea->addNeeded(m_model->clusters());
    }

    void SplitDisconnectedAction::redo() {
        auto disconnectedIt = m_model->clusterWithIndex(m_disconnected.index());
        assert(disconnectedIt != m_model->clusters().end());
        m_model->clusters().erase(disconnectedIt);
        m_commandScrollArea->removeFromLayout(m_disconnected.index());

        for (const auto& it : m_components) {
            m_model->clusters().emplace_back(it);
        }
        m_commandScrollArea->addNeeded(m_model->clusters());
    }

    ACTION_TYPE SplitDisconnectedAction::type() const {
        return ACTION_TYPE::ADD_CLUSTER;
    }

} // namespace action