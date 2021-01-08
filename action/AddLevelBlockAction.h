
//
// Created by pc on 29-12-20.
//

#ifndef BLOCKS_ADDLEVELBLOCKACTION_H
#define BLOCKS_ADDLEVELBLOCKACTION_H

#include "../model/BlockType_typedef.h"
#include "../model/GridXY.h"
#include "Action.h"

#include <set>
#include <variant>

namespace action {

    class AddLevelBlockAction : public Action {
      public:
        AddLevelBlockAction(std::set<std::variant<model::DYNAMIC_BLOCK_TYPE, model::INSTANT_BLOCK_TYPE, model::FLOOR_BLOCK_TYPE>> blockTypes,
                            const model::GridXY&                                                                                  point);

        void                      undoAction(app::Application_Edit& applicationEdit) override;
        void                      redoAction(app::Application_Edit& applicationEdit) override;
        [[nodiscard]] ACTION_TYPE type() const override;

      private:
        const std::set<std::variant<model::DYNAMIC_BLOCK_TYPE, model::INSTANT_BLOCK_TYPE, model::FLOOR_BLOCK_TYPE>> m_blockTypes;
        const model::GridXY                                                                                         m_point;
    };

} // namespace action

#endif // BLOCKS_ADDLEVELBLOCKACTION_H
