//
// Created by teunv on 1/17/2021.
//

#include "NewClusterAction.h"

#include "../model/Model.h"
#include "../view/widget/CommandScroll.h"

namespace action {

    NewClusterAction::NewClusterAction(model::Model* model, view::widget::CommandScroll* commandScroll, model::Cluster cluster)
        : m_cluster(std::move(cluster)), m_model(model), m_commandScroll(commandScroll) {
        setText(QString("Creating cluster %1").arg(m_cluster.index()));
    }

    void NewClusterAction::undo() {
        auto it = m_model->clusterWithIndex(m_cluster.index());
        assert(it != m_model->clusters().end());
        m_model->clusters().erase(it);
        m_commandScroll->removeFromLayout(m_cluster.index());
    }

    void NewClusterAction::redo() {
        if (not m_blockInitial) {
            assert(m_model->clusterWithIndex(m_cluster.index()) == m_model->clusters().end());
            m_model->clusters().push_back(m_cluster);

            m_commandScroll->addNeeded(m_model->clusters());
        }
        m_blockInitial = false;
    }

    ACTION_TYPE NewClusterAction::type() const {
        return ACTION_TYPE::ADD_CLUSTER;
    }
} // namespace action