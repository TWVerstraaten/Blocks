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

void action::DeleteClusterAction::undo() {
    NewClusterAction::redo();
}

void action::DeleteClusterAction::redo() {
    NewClusterAction::undo();
}

action::ACTION_TYPE action::DeleteClusterAction::type() const {
    return ACTION_TYPE::REMOVE_CLUSTER;
}
