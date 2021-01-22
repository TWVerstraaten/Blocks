//
// Created by teunv on 1/17/2021.
//

#include "NewClusterAction.h"

#include "../model/Model.h"
#include "view/widget/CentralWidget.h"
#include "view/widget/CommandScrollArea.h"
#include "view/widget/MainView.h"

#include <cassert>

namespace action {

    NewClusterAction::NewClusterAction(view::CentralWidget* centralWidget, model::Cluster cluster)
        : m_commandEditBox(nullptr), m_cluster(std::move(cluster)), m_centralWidget(centralWidget) {
        setText(QString("Creating cluster %1").arg(m_cluster.index()));
    }

    void NewClusterAction::undo() {
        auto* model = m_centralWidget->mainView()->model();
        auto  it    = model->clusterWithIndex(m_cluster.index());
        assert(it != model->clusters().end());
        model->clusters().erase(it);

        m_commandEditBox = m_centralWidget->commandScrollArea()->removeFromLayout(m_cluster.index());
    }

    void NewClusterAction::redo() {
        if (not m_blockInitial) {
            auto* model = m_centralWidget->mainView()->model();
            auto  it    = model->clusterWithIndex(m_cluster.index());
            assert(it == model->clusters().end());
            model->clusters().push_back(m_cluster);

            m_centralWidget->commandScrollArea()->addToLayout(std::move(m_commandEditBox));
            assert(m_commandEditBox == nullptr);
        }
        m_blockInitial = false;
    }

    ACTION_TYPE NewClusterAction::type() const {
        return ACTION_TYPE::ADD_CLUSTER;
    }
} // namespace action