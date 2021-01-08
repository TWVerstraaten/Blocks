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
        explicit AddClusterAction(model::Cluster cluster);

        /****** VIRTUAL FUNCTIONS  ******/
        void                      undoAction(app::ApplicationEdit& applicationEdit) override;
        void                      redoAction(app::ApplicationEdit& applicationEdit) override;
        [[nodiscard]] ACTION_TYPE type() const override;

      private:
        /****** DATA MEMBERS  ******/
        const model::Cluster m_cluster;
    };

} // namespace action
#endif // BLOCKS_ADDCLUSTERACTION_H
