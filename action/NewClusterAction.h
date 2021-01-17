//
// Created by teunv on 1/17/2021.
//

#ifndef BLOCKS_NEWCLUSTERACTION_H
#define BLOCKS_NEWCLUSTERACTION_H

#include "../model/Cluster.h"
#include "../view2/CommandEditBox.h"
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

        void                      undo() override;
        void                      redo() override;
        [[nodiscard]] ACTION_TYPE type() const override;

      protected:
        std::unique_ptr<view2::CommandEditBox> m_commandEditBox;

      private:
        view2::CentralWidget* m_centralWidget;
        model::Cluster        m_cluster;
    };
} // namespace action

#endif // BLOCKS_NEWCLUSTERACTION_H
