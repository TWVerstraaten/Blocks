//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_CHANGELEVELBLOCKACTION_H
#define BLOCKS_CHANGELEVELBLOCKACTION_H

#include "../model/Level_enums.h"
#include "Action.h"
#include "model/GridXy.h"
#include "model/Model.h"

namespace action {

    class ChangeLevelBlockAction : public Action {

      public:
        ChangeLevelBlockAction(model::Model*                                                             model,
                               const std::variant<model::DYNAMIC_BLOCK_TYPE, model::INSTANT_BLOCK_TYPE>& newType,
                               const std::variant<model::DYNAMIC_BLOCK_TYPE, model::INSTANT_BLOCK_TYPE>& oldType,
                               const model::GridXy&                                                      gridXy);

        void                      undo() override;
        void                      redo() override;
        [[nodiscard]] ACTION_TYPE type() const override;

      private:
        model::Model*                                                      m_model;
        std::variant<model::DYNAMIC_BLOCK_TYPE, model::INSTANT_BLOCK_TYPE> m_newType;
        std::variant<model::DYNAMIC_BLOCK_TYPE, model::INSTANT_BLOCK_TYPE> m_oldType;
        model::GridXy                                                      m_gridXy;
    };

} // namespace action

#endif // BLOCKS_CHANGELEVELBLOCKACTION_H
