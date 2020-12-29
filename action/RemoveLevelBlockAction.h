//
// Created by pc on 29-12-20.
//

#ifndef BLOCKS_REMOVELEVELBLOCKACTION_H
#define BLOCKS_REMOVELEVELBLOCKACTION_H

#include "../model/BlockType_typedef.h"
#include "../model/GridXY.h"
#include "Action.h"

#include <variant>

namespace action {
    class RemoveLevelBlockAction : public Action {

      public:
        RemoveLevelBlockAction(const std::variant<model::DYNAMIC_BLOCK_TYPE, model::INSTANT_BLOCK_TYPE, model::FLOOR_BLOCK_TYPE>& blockType,
                               const model::GridXY&                                                                               point);

        void        undoAction(app::Application_Edit& applicationEdit) override;
        void        redoAction(app::Application_Edit& applicationEdit) override;
        ACTION_TYPE type() const override;

      private:
        const std::variant<model::DYNAMIC_BLOCK_TYPE, model::INSTANT_BLOCK_TYPE, model::FLOOR_BLOCK_TYPE> m_blockType;
        const model::GridXY                                                                               m_point;
    };
} // namespace action
#endif // BLOCKS_REMOVELEVELBLOCKACTION_H
