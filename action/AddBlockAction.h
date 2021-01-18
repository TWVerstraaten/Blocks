//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_ADDBLOCKACTION_H
#define BLOCKS_ADDBLOCKACTION_H

#include "Action.h"
#include "RemoveBlockAction.h"

#include <model/GridXY.h>
namespace action {

    class AddBlockAction : public RemoveBlockAction {

      public:
        AddBlockAction(model::Model* model, size_t clusterIndex, const model::GridXY& gridXy);

        void        undo() override;
        void        redo() override;
        ACTION_TYPE type() const override;
    };
} // namespace action

#endif // BLOCKS_ADDBLOCKACTION_H
