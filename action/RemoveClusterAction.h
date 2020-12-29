//
// Created by pc on 28-12-20.
//

#ifndef BLOCKS_REMOVECLUSTERACTION_H
#define BLOCKS_REMOVECLUSTERACTION_H

#include "AddClusterAction.h"

namespace action {
    class RemoveClusterAction : public AddClusterAction {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        RemoveClusterAction(const model::Cluster& cluster, const view::widget::CommandEditBox& commandEditBox);

        /****** VIRTUAL FUNCTIONS  ******/
        void        undoAction(app::Application_Edit& applicationEdit) override;
        void        redoAction(app::Application_Edit& applicationEdit) override;
        ACTION_TYPE type() const override;
    };

} // namespace action

#endif // BLOCKS_REMOVECLUSTERACTION_H
