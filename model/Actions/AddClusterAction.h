//
// Created by pc on 27-12-20.
//

#ifndef BLOCKS_ADDCLUSTERACTION_H
#define BLOCKS_ADDCLUSTERACTION_H

#include "../Cluster.h"
#include "../GridXY.h"
#include "Action.h"

#include <cstddef>
namespace model {
    namespace action {

        class AddClusterAction : public Action {

          public:
            AddClusterAction(const Cluster& index, const GridXY& gridXy);

            void undoAction(Application_Edit& applicationEdit) override;
            void redoAction(Application_Edit& applicationEdit) override;

          private:
            const Cluster m_cluster;
            const GridXY  m_gridXy;
        };

    } // namespace action
} // namespace model
#endif // BLOCKS_ADDCLUSTERACTION_H
