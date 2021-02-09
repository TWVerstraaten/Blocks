//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_CHANGELEVELBLOCKACTION_H
#define BLOCKS_CHANGELEVELBLOCKACTION_H

#include "../model/GridXy.h"
#include "../model/Level_enums.h"
#include "Action.h"

#include <variant>

namespace model {
    class Level;
}

namespace action {

    class ChangeLevelBlockAction : public Action {

      public:
        ChangeLevelBlockAction(model::Level*                                                             level,
                               const std::variant<model::DYNAMIC_BLOCK_TYPE, model::INSTANT_BLOCK_TYPE>& newType,
                               const std::variant<model::DYNAMIC_BLOCK_TYPE, model::INSTANT_BLOCK_TYPE>& oldType,
                               const model::GridXy&                                                      gridXy);

        void                      undo() override;
        void                      redo() override;
        [[nodiscard]] ACTION_TYPE type() const override;

      private:
        model::Level*                                                      m_level;
        std::variant<model::DYNAMIC_BLOCK_TYPE, model::INSTANT_BLOCK_TYPE> m_newType;
        std::variant<model::DYNAMIC_BLOCK_TYPE, model::INSTANT_BLOCK_TYPE> m_oldType;
        model::GridXy                                                      m_gridXy;
    };

} // namespace action

#endif // BLOCKS_CHANGELEVELBLOCKACTION_H
