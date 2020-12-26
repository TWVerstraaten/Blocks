//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_CLUSTER_H
#define BLOCKS_CLUSTER_H

#include "Action.h"
#include "Level.h"
#include "Line.h"
#include "WorldXY.h"

#include <functional>
#include <set>
#include <vector>

class SDL_Renderer;

namespace model {

    class Cluster {
        /****** PRIVATE STATICS  ******/
        static size_t s_maxClusterIndex;

      public:
        /****** PUBLIC ENUMS / TYPEDEFS  ******/
        enum class PHASE { NONE, TRANSLATING, ROTATING };

        explicit Cluster(std::set<GridXY>&& gridXY, const std::string& name = "");
        explicit Cluster(const std::set<GridXY>& gridXY, std::string name = "");

        Cluster& operator=(const Cluster& other) = default;
        Cluster(const Cluster& other)            = default;
        Cluster(Cluster&& other)                 = default;
        Cluster& operator=(Cluster&& other) = default;

        /****** CONST GETTERS  ******/
        double                     angle() const;
        size_t                     index() const;
        size_t                     actionIndex() const;
        std::string                string() const;
        PHASE                      phase() const;
        WorldXY                    dynamicWorldOffset() const;
        const GridXY&              rotationPivot() const;
        std::set<WorldXY>          cornerPoints(int shrinkInWorld) const;
        std::set<Line<WorldXY>>    sides(int shrinkInWorld) const;
        const std::string&         name() const;
        const std::set<GridXY>&    gridXY() const;
        std::set<GridXY>&          gridXY();
        const std::vector<Action>& actions() const;
        size_t                     blockCount() const;

        /****** CONST FUNCTIONS  ******/
        bool                                  empty() const;
        bool                                  isAlive() const;
        bool                                  isConnected() const;
        bool                                  contains(const GridXY& gridXY) const;
        bool                                  intersects(const Cluster& other, int shrinkInWorld) const;
        std::function<WorldXY(const WorldXY)> phaseTransformation() const;

        /****** NON CONST FUNCTIONS  ******/
        void                       addGridXY(const GridXY& gridXY);
        void                       preStep();
        void                       doAction();
        void                       addPendingOperation(const GridXY& gridXY, Level::DYNAMIC_BLOCK_TYPE blockType);
        void                       performPendingOperationOrNextAction();
        void                       update(double dPhase);
        void                       addAction(Action action);
        void                       kill();
        void                       clearActions();
        void                       collideWithLevel(const Level& level, int shrinkInWorld);
        model::Cluster             grabSecondComponent();
        std::set<GridXY>::iterator removeBLock(const GridXY& gridXY);

      private:
        /****** PRIVATE  CONST FUNCTIONS  ******/
        bool gridXUYAreUnique() const;

        /****** PRIVATE NON CONST FUNCTIONS  ******/
        void rotateClockWiseAbout(const GridXY& pivotGridXY);
        void rotateCounterClockWiseAbout(const GridXY& pivotGridXY);
        void resetPhase();
        void setRotation(double angle, const GridXY& pivot);
        void incrementActionIndex();
        void tryPendingOperation();

        /****** PRIVATE STATIC FUNCTIONS  ******/
        static Action rotateActionClockWise(Action action);
        static Action rotateActionCounterClockWise(Action action);

        /****** DATA MEMBERS  ******/
        bool                                              m_alive         = true;
        double                                            m_phaseFraction = 0.0;
        double                                            m_angle         = 0.0;
        size_t                                            m_index         = 0;
        size_t                                            m_actionIndex   = 0;
        PHASE                                             m_phase         = PHASE::NONE;
        WorldXY                                           m_worldOffset   = {0, 0};
        GridXY                                            m_rotationPivot = {0, 0};
        std::string                                       m_name;
        std::vector<Action>                               m_actions           = {};
        std::map<const GridXY, Level::DYNAMIC_BLOCK_TYPE> m_pendingOperations = {};
        std::set<GridXY>                                  m_gridXYVector      = {};
        std::set<Line<GridXY>>                            m_sides;
    };
} // namespace model

#endif // BLOCKS_CLUSTER_H
