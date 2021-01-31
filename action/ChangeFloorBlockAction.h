//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_CHANGEFLOORBLOCKACTION_H
#define BLOCKS_CHANGEFLOORBLOCKACTION_H

#include "../model/Level_enums.h"
#include "Action.h"
#include "../model/GridXy.h"

namespace model {
    class Model;
}

namespace action {

    class ChangeFloorBlockAction : public Action {

      public:
        ChangeFloorBlockAction(model::Model* model, model::FLOOR_BLOCK_TYPE newType, model::FLOOR_BLOCK_TYPE oldType, const model::GridXy& gridXy);

        void                      undo() override;
        void                      redo() override;
        [[nodiscard]] ACTION_TYPE type() const override;

      private:
        model::FLOOR_BLOCK_TYPE m_newType;
        model::FLOOR_BLOCK_TYPE m_oldType;
        model::GridXy           m_gridXy;
        model::Model*           m_model;
    };

} // namespace action
#endif // BLOCKS_CHANGEFLOORBLOCKACTION_H
