//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_CLUSTER_H
#define BLOCKS_CLUSTER_H

#include "Action.h"
#include "Level.h"
#include "WorldVector.h"

#include <list>
#include <set>
#include <tuple>
#include <vector>

class SDL_Renderer;

namespace model {

    class Cluster {

      public:
        explicit Cluster(std::list<GridXY>&& gridXY, std::string name = "");

        Cluster& operator=(const Cluster& other);

        void                        preStep();
        void                        doAction();
        void                        addPendingOperation(const GridXY& gridXY, Level::DYNAMIC_BLOCK_TYPE blockType);
        void                        performPendingOperationOrNextAction();
        std::list<GridXY>::iterator removeBLock(const GridXY& gridXY);
        void                        update(double fractionOfPhase);
        void                        addAction(Action action);
        void                        kill();
        void                        clearActions();
        bool                        empty() const;
        bool                        isAlive() const;
        double                      angle() const;
        size_t                      index() const;
        size_t                      currentActionIndex() const;
        const GridXY&               rotationPivot() const;
        WorldVector                 dynamicWorldOffset() const;
        const std::list<GridXY>&    gridXY() const;
        const std::vector<Action>&  actions() const;
        std::set<WorldXY>           cornerPoints(int shrinkInWorld) const;
        const std::string&          getName() const;
        bool                        isConnected() const;
        Cluster                     getComponent();

      private:
        enum class CURRENT_PHASE { NONE, TRANSLATING, ROTATING };
        typedef std::pair<const GridXY&, Level::DYNAMIC_BLOCK_TYPE> Block;

        std::string toString() const;

        void rotateClockWiseAbout(const GridXY& pivotGridXY);
        void rotateCounterClockWiseAbout(const GridXY& pivotGridXY);
        void clearPhase();
        void setRotation(double angle, const GridXY& pivot);
        void incrementActionIndex();
        void tryPendingOperation();

        static Action rotateActionClockWise(Action action);
        static Action rotateActionCounterClockWise(Action action);

        bool                m_isAlive         = true;
        double              m_fractionOfPhase = 0.0;
        double              m_angle           = 0.0;
        size_t              m_actionIndex     = 0;
        CURRENT_PHASE       m_currentPhase    = CURRENT_PHASE::NONE;
        GridXY              m_rotationPivot;
        WorldVector         m_worldOffset{0, 0};
        std::list<GridXY>   m_gridXYList;
        std::vector<Action> m_actions;
        std::vector<Block>  m_pendingOperations;
        std::string         m_name;

        static size_t s_maxClusterIndex;
        size_t        m_index;
    };
} // namespace model

#endif // BLOCKS_CLUSTER_H
