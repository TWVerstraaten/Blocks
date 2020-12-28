//
// Created by pc on 28-12-20.
//

#ifndef BLOCKS_REMOVEBLOCKFROMCLUSTERACTION_H
#define BLOCKS_REMOVEBLOCKFROMCLUSTERACTION_H

#include "AddBlockToClusterAction.h"

namespace action {
    class RemoveBlockFromClusterAction : public AddBlockToClusterAction {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        RemoveBlockFromClusterAction(size_t clusterIndex, const model::GridXY& gridXy);

        /****** VIRTUAL FUNCTIONS  ******/
        void        undoAction(app::Application_Edit& applicationEdit) override;
        void        redoAction(app::Application_Edit& applicationEdit) override;
        ACTION_TYPE type() const override;
    };
} // namespace action
#endif // BLOCKS_REMOVEBLOCKFROMCLUSTERACTION_H
