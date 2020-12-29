//
// Created by pc on 27-12-20.
//

#ifndef BLOCKS_ADDCLUSTERACTION_H
#define BLOCKS_ADDCLUSTERACTION_H

#include "../model/Cluster.h"
#include "../view/widget/CommandEditBox.h"
#include "Action.h"

namespace action {

    class AddClusterAction : public Action {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        AddClusterAction(model::Cluster cluster, view::widget::CommandEditBox commandEditBox);

        /****** VIRTUAL FUNCTIONS  ******/
        void        undoAction(app::Application_Edit& applicationEdit) override;
        void        redoAction(app::Application_Edit& applicationEdit) override;
        ACTION_TYPE type() const override;

      private:
        /****** DATA MEMBERS  ******/
        const model::Cluster               m_cluster;
        const view::widget::CommandEditBox m_commandEditBox;
    };

} // namespace action
#endif // BLOCKS_ADDCLUSTERACTION_H
