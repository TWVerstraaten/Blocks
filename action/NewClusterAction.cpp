//
// Created by teunv on 1/17/2021.
//

#include "NewClusterAction.h"

#include "../model/Model.h"
#include "../view2/CentralWidget.h"
#include "../view2/MainView.h"

#include <cassert>

action::NewClusterAction::NewClusterAction(view2::CentralWidget* centralWidget, model::Cluster cluster)
    : m_centralWidget(centralWidget), m_commandEditBox(nullptr), m_cluster(std::move(cluster)) {
}

void action::NewClusterAction::undoAction(app::ApplicationEdit& applicationEdit) {
    assert(false);
}

void action::NewClusterAction::redoAction(app::ApplicationEdit& applicationEdit) {
    assert(false);
}

void action::NewClusterAction::undoAction() {
    auto* model = m_centralWidget->mainView()->model();
    auto  it    = model->clusterWithIndex(m_cluster.index());
    assert(it != model->clusters().end());
    model->clusters().erase(it);

    m_commandEditBox = m_centralWidget->commandScrollArea()->removeFromLayout(m_cluster.index());
    m_centralWidget->update();
}

void action::NewClusterAction::redoAction() {
    auto* model = m_centralWidget->mainView()->model();
    auto  it    = model->clusterWithIndex(m_cluster.index());
    assert(it == model->clusters().end());
    model->clusters().push_back(m_cluster);

    m_centralWidget->commandScrollArea()->addToLayout(std::move(m_commandEditBox));
    assert(m_commandEditBox == nullptr);
    m_centralWidget->update();
}

action::ACTION_TYPE action::NewClusterAction::type() const {
    return ACTION_TYPE::ADD_CLUSTER;
}
