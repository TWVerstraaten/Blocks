//
// Created by teunv on 1/17/2021.
//

#ifndef BLOCKS_NEWCLUSTERACTION_H
#define BLOCKS_NEWCLUSTERACTION_H

#include "../model/Cluster.h"
#include "Action.h"

#include <memory>

namespace view {
    class CommandScroll;
    class CentralWidget;
} // namespace view

namespace action {

    class NewClusterAction : public Action {

      public:
        NewClusterAction(view::CentralWidget* centralWidget, model::Cluster cluster);

        void                      undo() override;
        void                      redo() override;
        [[nodiscard]] ACTION_TYPE type() const override;

      protected:
        model::Cluster m_cluster;

      private:
        view::CentralWidget* m_centralWidget;
    };
} // namespace action

#endif // BLOCKS_NEWCLUSTERACTION_H
