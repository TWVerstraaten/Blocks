//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_CLUSTER_H
#define BLOCKS_CLUSTER_H

#include "Enums.h"
#include "Level.h"

#include <set>
#include <tuple>
#include <vector>

class SDL_Renderer;

namespace model {

    class Cluster {

      public:
        Cluster(std::set<IndexPair>&& indexPairs, const IndexPair& offset);

        void                       doAction();
        void                       addPendingOperation(const IndexPair& indexPair, Level::DYNAMIC_BLOCK_TYPE blockType);
        void                       performPendingOperation();
        void                       removeBLock(const IndexPair& indexPair);
        void                       update(double fractionOfPhase);
        bool                       empty() const;
        bool                       intersects(const IndexPair& indexPair) const;
        int                        rowOffset() const;
        int                        columnOffset() const;
        double                     dynamicRowOffset() const;
        double                     dynamicColumnOffset() const;
        enums::DIRECTION           adjacent(const IndexPair& indexPair) const;
        const std::set<IndexPair>& localIndexPairs() const;

        enum class Action { MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };
        void addAction(Action action);

      private:
        void rotateClockWiseAbout(const IndexPair& pivotIndexPair);
        void rotateCounterClockWiseAbout(const IndexPair& pivotIndexPair);

        static Action rotateActionClockWise(Action action);
        static Action rotateActionCounterClockWise(Action action);

        double              m_fractionOfPhase = 1.0;
        size_t              m_actionIndex     = 0;
        IndexPair           m_offset;
        IndexPair           m_previousOffset;
        std::set<IndexPair> m_localIndexPairs;
        std::vector<Action> m_actions;

        typedef std::pair<const IndexPair&, Level::DYNAMIC_BLOCK_TYPE> Block;
        std::vector<Block>                                             m_pendingOperations;
    };
} // namespace model

#endif // BLOCKS_CLUSTER_H
