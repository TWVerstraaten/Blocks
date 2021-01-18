//
// Created by teunv on 1/17/2021.
//

#ifndef BLOCKS_REMOVEBLOCKFROMCLUSTERACTION_H
#define BLOCKS_REMOVEBLOCKFROMCLUSTERACTION_H

#include "Action.h"

#include <model/GridXY.h>

namespace model {
    class Model;
}

namespace action {

    class RemoveBlockFromClusterAction : public Action {

      public:
        RemoveBlockFromClusterAction(model::Model* model, size_t clusterIndex, const model::GridXY& gridXy);

        void                      undo() override;
        void                      redo() override;
        [[nodiscard]] ACTION_TYPE type() const override;

      protected:
        model::Model* m_model;
        size_t        m_clusterIndex;
        model::GridXY m_gridXy;
    };
} // namespace action

#endif // BLOCKS_REMOVEBLOCKFROMCLUSTERACTION_H
