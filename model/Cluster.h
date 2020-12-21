//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_CLUSTER_H
#define BLOCKS_CLUSTER_H

#include "ClusterAction.h"
#include "Enums.h"
#include "Level.h"
#include "WorldVector.h"

#include <list>
#include <tuple>

class SDL_Renderer;

namespace model {

    class Cluster {

      public:
        Cluster(std::list<GridCoordinates>&& gridCoordinates, const GridCoordinates& offset);

        void                              doAction();
        void                              addPendingOperation(const GridCoordinates& indexPair, Level::DYNAMIC_BLOCK_TYPE blockType);
        void                              performPendingOperation();
        void                              removeBLock(const GridCoordinates& indexPair);
        void                              update(double fractionOfPhase);
        void                              addAction(ClusterAction action);
        bool                              empty() const;
        bool                              intersects(const GridCoordinates& indexPair) const;
        double                            angle() const;
        double                            dynamicRowOffset() const;
        double                            dynamicColumnOffset() const;
        enums::DIRECTION                  adjacent(const GridCoordinates& indexPair) const;
        const std::list<GridCoordinates>& gridCoordinates() const;
        const GridCoordinates&            rotationPivot() const;
        const std::vector<ClusterAction>& clusterActions() const;
        const WorldVector&                worldOffset() const;
        WorldVector                       dynamicWorldOffset() const;

      private:
        void rotateClockWiseAbout(const GridCoordinates& pivotIndexPair);
        void rotateCounterClockWiseAbout(const GridCoordinates& pivotIndexPair);

        static ClusterAction rotateActionClockWise(ClusterAction action);
        static ClusterAction rotateActionCounterClockWise(ClusterAction action);

        double                     m_fractionOfPhase    = 0.0;
        double                     m_angle              = 0.0;
        size_t                     m_clusterActionIndex = 0;
        GridCoordinates            m_rotationPivot;
        WorldVector                m_worldOffset;
        std::list<GridCoordinates> m_gridCoordinates;
        std::vector<ClusterAction> m_clusterActions;

        typedef std::pair<const GridCoordinates&, Level::DYNAMIC_BLOCK_TYPE> Block;
        std::vector<Block>                                                   m_pendingOperations;
    };
} // namespace model

#endif // BLOCKS_CLUSTER_H
