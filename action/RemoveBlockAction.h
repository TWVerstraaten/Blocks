//
// Created by teunv on 1/17/2021.
//

#ifndef BLOCKS_REMOVEBLOCKACTION_H
#define BLOCKS_REMOVEBLOCKACTION_H

#include "Action.h"

#include <model/GridXY.h>

namespace model {
    class Model;
}

namespace action {

    class RemoveBlockAction : public Action {

      public:
        RemoveBlockAction(model::Model* model, size_t clusterIndex, const model::GridXY& gridXy);

        void                      undo() override;
        void                      redo() override;
        [[nodiscard]] ACTION_TYPE type() const override;

      protected:
        model::Model* m_model;
        size_t        m_clusterIndex;
        model::GridXY m_gridXy;
    };
} // namespace action

#endif // BLOCKS_REMOVEBLOCKACTION_H
