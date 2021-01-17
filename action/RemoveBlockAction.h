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

        void                      undoAction(app::ApplicationEdit& applicationEdit) override;
        void                      redoAction(app::ApplicationEdit& applicationEdit) override;
        void                      undoAction() override;
        void                      redoAction() override;
        [[nodiscard]] ACTION_TYPE type() const override;

      private:
        model::Model* m_model;
        size_t        m_clusterIndex;
        model::GridXY m_gridXy;
    };
} // namespace action

#endif // BLOCKS_REMOVEBLOCKACTION_H
