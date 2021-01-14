//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_CLUSTER_H
#define BLOCKS_CLUSTER_H

#include "Cluster_enums.h"
#include "GridXY.h"
#include "Level_enums.h"
#include "Model_typedefs.h"
#include "WorldLine.h"
#include "WorldXY.h"
#include "command/Command.h"
#include "command/CommandVector.h"

#include <functional>
#include <iostream>
#include <list>
#include <set>
#include <vector>

namespace model {

    class Level;
    class Model;

    class Cluster {
        /****** PRIVATE STATIC DATA MEMBERS  ******/
        static size_t s_maxClusterIndex;

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        explicit Cluster(GridXYSet&& gridXY, std::string name);
        Cluster(const Cluster& other) = default;
        Cluster(Cluster&& other)      = default;
        Cluster& operator=(const Cluster& other) = default;
        Cluster& operator=(Cluster&& other) = default;
        Cluster(GridXYSet&& gridXY, CommandVector commandVector, std::string name);

        /****** CONST GETTERS  ******/
        [[nodiscard]] double                angle() const;
        [[nodiscard]] size_t                index() const;
        [[nodiscard]] size_t                size() const;
        [[nodiscard]] CLUSTER_STATE         state() const;
        [[nodiscard]] PENDING_DYNAMIC_MOVES pendingDynamicMoves() const;
        [[nodiscard]] COMMAND_MODIFIER      currentModifier() const;
        [[nodiscard]] WorldXY               dynamicWorldOffset() const;
        [[nodiscard]] WorldXY               approximateCenter() const;
        [[nodiscard]] std::string           string() const;
        [[nodiscard]] WorldXYSet            cornerPoints(int shrinkInWorld) const;
        [[nodiscard]] WorldLineSet          sides(int shrinkInWorld) const;
        [[nodiscard]] const GridXY&         rotationPivot() const;
        [[nodiscard]] const std::string&    name() const;
        [[nodiscard]] const GridXYSet&      gridXY() const;
        [[nodiscard]] const CommandVector&  commandVector() const;

        /****** CONST FUNCTIONS  ******/
        [[nodiscard]] bool                                   isEmpty() const;
        [[nodiscard]] bool                                   isAlive() const;
        [[nodiscard]] bool                                   isConnected() const;
        [[nodiscard]] bool                                   isAdjacent(const Cluster& other) const;
        [[nodiscard]] bool                                   gridXYIsAdjacent(const GridXY& point) const;
        [[nodiscard]] bool                                   contains(const GridXY& gridXY) const;
        [[nodiscard]] bool                                   intersects(const Cluster& other, int shrinkInWorld) const;
        [[nodiscard]] std::function<WorldXY(const WorldXY&)> phaseTransformation() const;

        /****** NON CONST FUNCTIONS  ******/
        void                addGridXY(const GridXY& gridXY);
        void                doCommand(Model& model);
        void                update(double phaseFraction);
        void                kill();
        void                buildSides();
        void                incrementCommandIndex();
        void                clearCommands();
        void                collideWithLevel(const Level& level, int shrinkInWorld);
        void                handleDynamicBlock(const GridXY& point, DYNAMIC_BLOCK_TYPE type);
        void                setState(CLUSTER_STATE state);
        void                setWorldOffset(const WorldXY& worldOffset);
        void                setPhase(PHASE phase);
        void                resetPhase();
        void                setPendingDynamicMoves(PENDING_DYNAMIC_MOVES pendingDynamicMoves);
        void                spliceCluster(Level& level);
        Cluster             grabAllButFirstComponent();
        CommandVector&      commandVector();
        GridXYSet::iterator removeBLock(const GridXY& gridXY);
        std::list<Cluster>  collectAllButFirstComponent();
        GridXYSet&          gridXY();

        /****** FRIENDS  ******/
        friend std::ostream& operator<<(std::ostream& out, const Cluster& other);
        friend void          doAction(const Command_Simple& command, Cluster& cluster, Level& level);
        friend void          doAction(const Command_Modified& command, Cluster& cluster, Level& level);
        friend void          doAction(const Command_RepeatWrapper& command, Cluster& cluster, Level& level);

      private:
        /****** PRIVATE NON CONST FUNCTIONS  ******/
        void rotateClockWiseAbout(const GridXY& pivotGridXY);
        void rotateCounterClockWiseAbout(const GridXY& pivotGridXY);
        void setRotation(double angle, const GridXY& pivot);
        void grabAdjacentStoppedClusters(Level& level);

        /****** DATA MEMBERS  ******/
        double                m_phaseFraction = 0.0;
        double                m_angle         = 0.0;
        size_t                m_index;
        PENDING_DYNAMIC_MOVES m_pendingDynamicMoves = PENDING_DYNAMIC_MOVES::ZERO;
        CLUSTER_STATE         m_state               = CLUSTER_STATE::ALIVE;
        PHASE                 m_phase               = PHASE::NONE;
        WorldXY               m_worldOffset         = {0, 0};
        GridXY                m_rotationPivot       = {0, 0};
        CommandVector         m_commandVector;
        GridXYSet             m_gridXYSet;
        WorldLineSet          m_sides;
        std::string           m_name;
    };

} // namespace model

#endif // BLOCKS_CLUSTER_H
