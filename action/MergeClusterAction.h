//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_MERGECLUSTERACTION_H
#define BLOCKS_MERGECLUSTERACTION_H

#include "../model/Cluster.h"
#include "Action.h"

#include <memory>

namespace view::widget {
    class CommandScroll;
} // namespace view::widget

namespace action {
    class MergeClusterAction : public Action {

      public:
        MergeClusterAction(model::Model* model, model::Cluster target, model::Cluster second, view::widget::CommandScroll* commandScrollArea);

        void                      undo() override;
        void                      redo() override;
        [[nodiscard]] ACTION_TYPE type() const override;

      private:
        model::Model*                m_model;
        const model::Cluster         m_target;
        const model::Cluster         m_second;
        view::widget::CommandScroll* m_commandScrollArea;
    };
} // namespace action

#endif // BLOCKS_MERGECLUSTERACTION_H
