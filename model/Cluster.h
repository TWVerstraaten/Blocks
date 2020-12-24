//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_CLUSTER_H
#define BLOCKS_CLUSTER_H

#include "Action.h"
#include "Level.h"
#include "WorldXY.h"

#include <list>
#include <memory>
#include <set>
#include <tuple>
#include <vector>

class SDL_Renderer;

namespace model {

    class Cluster {
        /****** PRIVATE STATICS  ******/
        static size_t s_maxClusterIndex;

      public:
        explicit Cluster(std::list<GridXY>&& gridXY, std::string name = "");
        Cluster(const Cluster& other) = default;
        Cluster& operator             =(const Cluster& other);
        Cluster& operator=(Cluster&& other) noexcept = default;

        /****** CONST GETTERS  ******/
        double                     angle() const;
        size_t                     index() const;
        size_t                     actionIndex() const;
        std::string                string() const;
        WorldXY                    dynamicWorldOffset() const;
        const GridXY&              rotationPivot() const;
        const std::set<WorldXY>    cornerPoints(int shrinkInWorld) const;
        const std::string&         name() const;
        const std::list<GridXY>&   gridXY() const;
        const std::vector<Action>& actions() const;

        /****** CONST FUNCTIONS  ******/
        bool empty() const;
        bool isAlive() const;
        bool isConnected() const;

        /****** NON CONST FUNCTIONS  ******/
        void                        preStep();
        void                        doAction();
        void                        addPendingOperation(const GridXY& gridXY, Level::DYNAMIC_BLOCK_TYPE blockType);
        void                        performPendingOperationOrNextAction();
        void                        update(double fractionOfPhase);
        void                        addAction(Action action);
        void                        kill();
        void                        clearActions();
        model::Cluster              getComponent();
        std::list<GridXY>::iterator removeBLock(const GridXY& gridXY);

      private:
        /****** PRIVATE ENUMS / TYPEDEFS  ******/
        enum class CURRENT_PHASE { NONE, TRANSLATING, ROTATING };
        typedef std::pair<const GridXY&, Level::DYNAMIC_BLOCK_TYPE> Block;

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
        bool                m_isAlive           = true;
        double              m_fractionOfPhase   = 0.0;
        double              m_angle             = 0.0;
        size_t              m_actionIndex       = 0;
        CURRENT_PHASE       m_currentPhase      = CURRENT_PHASE::NONE;
        WorldXY             m_worldOffset       = {0, 0};
        GridXY              m_rotationPivot     = {0, 0};
        std::vector<Action> m_actions           = {};
        std::vector<Block>  m_pendingOperations = {};
        std::list<GridXY>   m_gridXYList;
        std::string         m_name;
        size_t              m_index;
    };
} // namespace model

#endif // BLOCKS_CLUSTER_H
