//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_SPLITDISCONNECTEDACTION_H
#define BLOCKS_SPLITDISCONNECTEDACTION_H

#include "../model/Cluster.h"
#include "Action.h"
#include "view/CommandEditWidget.h"

#include <list>
#include <memory>

namespace model {
    class Model;
}

namespace view {
    class CommandScrollArea;
}

namespace action {
    class SplitDisconnectedAction : public Action {

      public:
        SplitDisconnectedAction(model::Model* model, model::Cluster disconnected, view::CommandScrollArea* commandScrollArea);

        void                      undo() override;
        void                      redo() override;
        [[nodiscard]] ACTION_TYPE type() const override;

      private:
        model::Model*                                    m_model;
        const model::Cluster                             m_disconnected;
        std::unique_ptr<view::CommandEditWidget>            m_disconnectedCommandEditBox;
        std::list<model::Cluster>                        m_components;
        std::list<std::unique_ptr<view::CommandEditWidget>> m_componentCommandEditBoxes;
        view::CommandScrollArea*                         m_commandScrollArea;
    };
} // namespace action

#endif // BLOCKS_SPLITDISCONNECTEDACTION_H
