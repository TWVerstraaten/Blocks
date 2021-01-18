//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_MERGECLUSTERACTION_H
#define BLOCKS_MERGECLUSTERACTION_H

#include "../model/Cluster.h"
#include "../view2/CommandEditBox.h"
#include "Action.h"

#include <memory>

namespace view2 {
    class CommandScrollArea;
} // namespace view2

namespace action {
    class MergeClusterAction : public Action {

      public:
        MergeClusterAction(model::Model* model, model::Cluster target, model::Cluster second, view2::CommandScrollArea* commandScrollArea);

        void                      undo() override;
        void                      redo() override;
        [[nodiscard]] ACTION_TYPE type() const override;

      private:
        model::Model*                          m_model;
        const model::Cluster                   m_target;
        const model::Cluster                   m_second;
        view2::CommandScrollArea*              m_commandScrollArea;
        std::unique_ptr<view2::CommandEditBox> m_commandEditBox = nullptr;
    };
} // namespace action

#endif // BLOCKS_MERGECLUSTERACTION_H
