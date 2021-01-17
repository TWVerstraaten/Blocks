//
// Created by teunv on 1/17/2021.
//

#ifndef BLOCKS_MODELACTION_H
#define BLOCKS_MODELACTION_H

#include "../model/Model.h"
#include "Action.h"

namespace view2 {
    class CommandScrollArea;
}

namespace action {

    class ModelAction : public Action {

      public:
        ModelAction(model::Model* model, model::Model oldCopy, model::Model newCopy, view2::CommandScrollArea* commandScrollArea);

        void                      undo() override;
        void                      redo() override;
        [[nodiscard]] ACTION_TYPE type() const override;

      private:
        model::Model*             m_model;
        model::Model              m_old;
        model::Model              m_new;
        view2::CommandScrollArea* m_commandScrollArea;
    };

} // namespace action

#endif // BLOCKS_MODELACTION_H
