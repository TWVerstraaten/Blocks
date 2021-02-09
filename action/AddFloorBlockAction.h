//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_ADDFLOORBLOCKACTION_H
#define BLOCKS_ADDFLOORBLOCKACTION_H

#include "../model/GridXy.h"
#include "../model/Level_enums.h"
#include "Action.h"

namespace model {
    class Level;
}

namespace action {

    class AddFloorBlockAction : public Action {

      public:
        AddFloorBlockAction(model::Level* level, model::FLOOR_BLOCK_TYPE type, const model::GridXy& gridXy);

        void                      undo() override;
        void                      redo() override;
        [[nodiscard]] ACTION_TYPE type() const override;

      private:
        model::FLOOR_BLOCK_TYPE m_type;
        model::GridXy           m_gridXy;
        model::Level*           m_level;
    };

} // namespace action

#endif // BLOCKS_ADDFLOORBLOCKACTION_H
