//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_ADDLEVELBLOCKACTION_H
#define BLOCKS_ADDLEVELBLOCKACTION_H

#include "../model/GridXy.h"
#include "../model/Level_enums.h"
#include "Action.h"

#include <variant>

namespace model {
    class Model;
}

namespace action {

    class AddLevelBlockAction : public Action {

      public:
        AddLevelBlockAction(model::Model* model, model::DYNAMIC_BLOCK_TYPE type, const model::GridXy& gridXy);
        AddLevelBlockAction(model::Model* model, model::INSTANT_BLOCK_TYPE type, const model::GridXy& gridXy);

        void                      undo() override;
        void                      redo() override;
        [[nodiscard]] ACTION_TYPE type() const override;

      private:
        std::variant<model::DYNAMIC_BLOCK_TYPE, model::INSTANT_BLOCK_TYPE> m_type;
        model::GridXy                                                      m_gridXy;
        model::Model*                                                      m_model;
    };
} // namespace action

#endif // BLOCKS_ADDLEVELBLOCKACTION_H
