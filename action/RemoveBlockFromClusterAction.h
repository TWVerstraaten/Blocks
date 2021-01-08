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
        void        undoAction(app::ApplicationEdit& applicationEdit) override;
        void        redoAction(app::ApplicationEdit& applicationEdit) override;
        [[nodiscard]] ACTION_TYPE type() const override;
    };
} // namespace action
#endif // BLOCKS_REMOVEBLOCKFROMCLUSTERACTION_H
