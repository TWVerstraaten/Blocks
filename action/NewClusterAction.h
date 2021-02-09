//
// Created by teunv on 1/17/2021.
//

#ifndef BLOCKS_NEWCLUSTERACTION_H
#define BLOCKS_NEWCLUSTERACTION_H

#include "../model/Cluster.h"
#include "Action.h"

#include <memory>

namespace view::widget {
    class CommandScroll;
} // namespace view::widget

namespace model {
    class Model;
}

namespace action {

    class NewClusterAction : public Action {

      public:
        NewClusterAction(model::Model* model, view::widget::CommandScroll* commandScroll, model::Cluster cluster);

        void                      undo() override;
        void                      redo() override;
        [[nodiscard]] ACTION_TYPE type() const override;

      protected:
        model::Cluster m_cluster;

      private:
        model::Model*                m_model;
        view::widget::CommandScroll* m_commandScroll;
    };
} // namespace action

#endif // BLOCKS_NEWCLUSTERACTION_H
