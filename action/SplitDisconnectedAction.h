//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_SPLITDISCONNECTEDACTION_H
#define BLOCKS_SPLITDISCONNECTEDACTION_H

#include "../model/Cluster.h"
#include "Action.h"
#include "../view/widget/CommandScroll.h"

#include <list>
#include <memory>

namespace model {
    class Model;
}

namespace action {
    class SplitDisconnectedAction : public Action {

      public:
        SplitDisconnectedAction(model::Model* model, model::Cluster disconnected, view::CommandScroll* commandScrollArea);

        void                      undo() override;
        void                      redo() override;
        [[nodiscard]] ACTION_TYPE type() const override;

      private:
        model::Model*               m_model;
        const model::Cluster        m_disconnected;
        std::vector<model::Cluster> m_components;
        view::CommandScroll*    m_commandScrollArea;
    };
} // namespace action

#endif // BLOCKS_SPLITDISCONNECTEDACTION_H
