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
#include <set>
#include <tuple>

class SDL_Renderer;

namespace model {

    class Cluster {

      public:
        Cluster(std::list<GridCoordinates>&& gridCoordinates, const GridCoordinates& offset);

        Cluster& operator=(const Cluster& other);

        void                              doAction();
        void                              addPendingOperation(const GridCoordinates& indexPair, Level::DYNAMIC_BLOCK_TYPE blockType);
        void                              performPendingOperation();
        void                              removeBLock(const GridCoordinates& indexPair);
        void                              update(double fractionOfPhase);
        void                              addAction(ClusterAction action);
        void                              kill();
        void                              clearActions();
        bool                              empty() const;
        bool                              intersects(const GridCoordinates& indexPair) const;
        bool                              isAlive() const;
        double                            angle() const;
        const GridCoordinates&            rotationPivot() const;
        WorldVector                       dynamicWorldOffset() const;
        const std::list<GridCoordinates>& gridCoordinates() const;
        const std::vector<ClusterAction>& clusterActions() const;
        std::set<model::WorldCoordinates> cornerPoints() const;

      private:
        enum class CURRENT_PHASE { NONE, TRANSLATING, ROTATING };
        typedef std::pair<const GridCoordinates&, Level::DYNAMIC_BLOCK_TYPE> Block;

        void rotateClockWiseAbout(const GridCoordinates& pivotIndexPair);
        void rotateCounterClockWiseAbout(const GridCoordinates& pivotIndexPair);
        void clearPhase();
        void setRotation(double angle, const GridCoordinates& pivot);

        static ClusterAction rotateActionClockWise(ClusterAction action);
        static ClusterAction rotateActionCounterClockWise(ClusterAction action);

        bool                       m_isAlive            = true;
        double                     m_fractionOfPhase    = 0.0;
        double                     m_angle              = 0.0;
        size_t                     m_clusterActionIndex = 0;
        CURRENT_PHASE              m_currentPhase       = CURRENT_PHASE::NONE;
        GridCoordinates            m_rotationPivot;
        WorldVector                m_worldOffset;
        std::list<GridCoordinates> m_gridCoordinates;
        std::vector<ClusterAction> m_clusterActions;
        std::vector<Block>         m_pendingOperations;
    };
} // namespace model

#endif // BLOCKS_CLUSTER_H
