//
// Created by teunv on 1/19/2021.
//

#ifndef BLOCKS_REMOVELEVELBLOCKACTION_H
#define BLOCKS_REMOVELEVELBLOCKACTION_H

#include "AddLevelBlockAction.h"

namespace action {

    class RemoveLevelBlockAction : public AddLevelBlockAction {

      public:
        RemoveLevelBlockAction(model::Level* level, model::DYNAMIC_BLOCK_TYPE type, const model::GridXy& gridXy);
        RemoveLevelBlockAction(model::Level* level, model::INSTANT_BLOCK_TYPE type, const model::GridXy& gridXy);

        void                      undo() override;
        void                      redo() override;
        [[nodiscard]] ACTION_TYPE type() const override;
    };
} // namespace action

#endif // BLOCKS_REMOVELEVELBLOCKACTION_H
