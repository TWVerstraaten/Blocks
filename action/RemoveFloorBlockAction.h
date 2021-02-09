//
// Created by teunv on 1/19/2021.
//

#ifndef BLOCKS_REMOVEFLOORBLOCKACTION_H
#define BLOCKS_REMOVEFLOORBLOCKACTION_H

#include "AddFloorBlockAction.h"

namespace action {

    class RemoveFloorBlockAction : public AddFloorBlockAction {

      public:
        RemoveFloorBlockAction(model::Level* level, model::FLOOR_BLOCK_TYPE type, const model::GridXy& gridXy);

        void                      undo() override;
        void                      redo() override;
        [[nodiscard]] ACTION_TYPE type() const override;
    };
} // namespace action

#endif // BLOCKS_REMOVEFLOORBLOCKACTION_H
