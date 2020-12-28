//
// Created by pc on 27-12-20.
//

#ifndef BLOCKS_ADDCLUSTERACTION_H
#define BLOCKS_ADDCLUSTERACTION_H

#include "../model/Cluster.h"
#include "../model/GridXY.h"
#include "Action.h"

#include <cstddef>
namespace action {

    class AddClusterAction : public Action {

      public:
        AddClusterAction(model::Cluster cluster);

        void undoAction(Application_Edit& applicationEdit) override;
        void redoAction(Application_Edit& applicationEdit) override;

      private:
        const model::Cluster m_cluster;
    };

} // namespace action
#endif // BLOCKS_ADDCLUSTERACTION_H
