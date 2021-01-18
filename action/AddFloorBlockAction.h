//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_ADDFLOORBLOCKACTION_H
#define BLOCKS_ADDFLOORBLOCKACTION_H

#include "../model/Level_enums.h"
#include "Action.h"
#include "model/GridXY.h"
#include "model/Model.h"

namespace action {

    class AddFloorBlockAction : public Action {

      public:
        AddFloorBlockAction(model::Model* model, model::FLOOR_BLOCK_TYPE type, const model::GridXY& gridXy);

        void                      undo() override;
        void                      redo() override;
        [[nodiscard]] ACTION_TYPE type() const override;

      private:
        model::FLOOR_BLOCK_TYPE m_type;
        model::GridXY           m_gridXy;
        model::Model*           m_model;
    };

} // namespace action

#endif // BLOCKS_ADDFLOORBLOCKACTION_H
