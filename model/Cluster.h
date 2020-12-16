//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_CLUSTER_H
#define BLOCKS_CLUSTER_H

#include "../model/Matrix.h"
#include "Enums.h"
#include "Level.h"

#include <set>
#include <tuple>
#include <vector>

class SDL_Renderer;

namespace model {

    class Cluster {

      public:
        Cluster(Matrix&& matrix, size_t rowOffset, size_t columnOffset);

        void                       doStep();
        void                       doAction();
        void                       rotateClockWiseAbout(const IndexPair& pivotIndexPair);
        void                       removeBLock(const IndexPair& indexPair);
        void                       rotateCounterClockWiseAbout(const IndexPair& pivotIndexPair);
        void                       interactWithBlock(const IndexPair& indexPair, Level::BLOCK_TYPE blockType);
        void                       performPendingActions();
        bool                       empty() const;
        bool                       intersects(const IndexPair& indexPair) const;
        int                        rowOffset() const;
        int                        columnOffset() const;
        enums::DIRECTION           adjacent(const IndexPair& indexPair) const;
        const std::set<IndexPair>& indexPairs() const;

        enum class Action { MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };
        void addAction(Action action);

      private:
        static Action rotateActionClockWise(Action action);
        static Action rotateActionCounterClockWise(Action action);

        int                 m_rowOffset    = 0;
        int                 m_columnOffset = 0;
        size_t              m_actionIndex  = 0;
        std::set<IndexPair> m_indexPairs;
        std::vector<Action> m_actions;

        typedef std::pair<const IndexPair&, Level::BLOCK_TYPE> Block;
        std::vector<Block>                                     m_pendingOperations;
    };
} // namespace model

#endif // BLOCKS_CLUSTER_H
