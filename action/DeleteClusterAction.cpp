//
// Created by teunv on 1/17/2021.
//

#include "DeleteClusterAction.h"

#include <cassert>

action::DeleteClusterAction::DeleteClusterAction(view2::CentralWidget*                    centralWidget,
                                                 const model::Cluster&                    cluster,
                                                 std::unique_ptr<view2::CommandEditBox>&& commandEditBox)
    : NewClusterAction(centralWidget, cluster) {
    m_commandEditBox = std::move(commandEditBox);
}

void action::DeleteClusterAction::undoAction(app::ApplicationEdit& applicationEdit) {
    assert(false);
}

void action::DeleteClusterAction::redoAction(app::ApplicationEdit& applicationEdit) {
    assert(false);
}

void action::DeleteClusterAction::undoAction() {
    NewClusterAction::redoAction();
}

void action::DeleteClusterAction::redoAction() {
    NewClusterAction::undoAction();
}

action::ACTION_TYPE action::DeleteClusterAction::type() const {
    return ACTION_TYPE::REMOVE_CLUSTER;
}
