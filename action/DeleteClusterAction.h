//
// Created by teunv on 1/17/2021.
//

#ifndef BLOCKS_DELETECLUSTERACTION_H
#define BLOCKS_DELETECLUSTERACTION_H

#include "NewClusterAction.h"

namespace action {

    class DeleteClusterAction : public NewClusterAction {

      public:
        DeleteClusterAction(view::CentralWidget* centralWidget, const model::Cluster& cluster);

        void                      undo() override;
        void                      redo() override;
        [[nodiscard]] ACTION_TYPE type() const override;
    };
} // namespace action

#endif // BLOCKS_DELETECLUSTERACTION_H
