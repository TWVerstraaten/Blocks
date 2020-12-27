//
// Created by pc on 27-12-20.
//

#ifndef BLOCKS_CLEARBLOCKACTION_H
#define BLOCKS_CLEARBLOCKACTION_H

#include "../Cluster.h"
#include "../GridXY.h"
#include "Action.h"

namespace model {
    namespace action {

        class ClearBlockAction : public Action {
          public:
            ClearBlockAction(size_t cluster, const GridXY& clearedBlock, std::list<size_t>&& createdClusters);

            void                    redoAction(Application_Edit& applicationEdit) override;
            void                    undoAction(Application_Edit& applicationEdit) override;
            const size_t            m_cluster;
            const GridXY            m_clearedBlock;
            const std::list<size_t> m_createdClusters;
        };

    } // namespace action
} // namespace model

#endif // BLOCKS_CLEARBLOCKACTION_H
