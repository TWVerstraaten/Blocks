//
// Created by teunv on 1/17/2021.
//

#ifndef BLOCKS_NEWCLUSTERACTION_H
#define BLOCKS_NEWCLUSTERACTION_H

#include "../model/Cluster.h"
#include "Action.h"

#include <memory>

namespace view2 {
    class CommandScrollArea;
    class CentralWidget;
    class CommandEditBox;
} // namespace view2

namespace action {

    class NewClusterAction : public Action {

      public:
        NewClusterAction(view2::CentralWidget* centralWidget, model::Cluster cluster);

        void                      undoAction(app::ApplicationEdit& applicationEdit) override;
        void                      redoAction(app::ApplicationEdit& applicationEdit) override;
        void                      undoAction() override;
        void                      redoAction() override;
        [[nodiscard]] ACTION_TYPE type() const override;

      private:
        view2::CentralWidget*                  m_centralWidget;
        std::unique_ptr<view2::CommandEditBox> m_commandEditBox;
        model::Cluster                         m_cluster;
    };
} // namespace action
#endif // BLOCKS_NEWCLUSTERACTION_H
