//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_CLUSTER_H
#define BLOCKS_CLUSTER_H

#include "../misc/AlignedRectangle.h"
#include "Cluster_enums.h"
#include "GridXy.h"
#include "Level_enums.h"
#include "Model_typedefs.h"
#include "PhaseTransformation.h"
#include "WorldLine.h"
#include "WorldXy.h"
#include "command/Command.h"
#include "command/CommandVector.h"

#include <functional>
#include <iostream>
#include <list>
#include <set>

namespace model {

    class Level;
    class Model;

    class Cluster {
        /****** PRIVATE STATIC DATA MEMBERS  ******/
        static size_t s_maxClusterIndex;

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        Cluster(GridXyVector&& gridXy, std::string name);
        Cluster(const GridXy& gridXy, std::string name);
        Cluster(GridXyVector&& gridXy, CommandVector commandVector, std::string name);

        /****** CONST GETTERS  ******/
        [[nodiscard]] double                angle() const;
        [[nodiscard]] size_t                index() const;
        [[nodiscard]] size_t                size() const;
        [[nodiscard]] CLUSTER_STATE         state() const;
        [[nodiscard]] PENDING_DYNAMIC_MOVES pendingDynamicMoves() const;
        [[nodiscard]] COMMAND_MODIFIER      currentModifier() const;
        [[nodiscard]] COMMAND_TYPE          currentType() const;
        [[nodiscard]] WorldXy               dynamicWorldOffset() const;
        [[nodiscard]] WorldXy               approximateCenter() const;
        [[nodiscard]] std::string           string() const;
        [[nodiscard]] WorldXyVector         cornerPoints(int shrinkInWorld) const;
        [[nodiscard]] WorldLineVector       sides(int shrinkInWorld) const;
        [[nodiscard]] const GridXy&         rotationPivot() const;
        [[nodiscard]] const std::string&    name() const;
        [[nodiscard]] const GridXyVector&   gridXyVector() const;
        [[nodiscard]] const CommandVector&  commandVector() const;
        [[nodiscard]] PHASE                 phase() const;

        /****** CONST FUNCTIONS  ******/
        void                                        buildSides() const;
        [[nodiscard]] bool                          isEmpty() const;
        [[nodiscard]] bool                          isAlive() const;
        [[nodiscard]] bool                          isConnected() const;
        [[nodiscard]] bool                          isAdjacent(const Cluster& other) const;
        [[nodiscard]] bool                          gridXyIsAdjacent(const GridXy& point) const;
        [[nodiscard]] bool                          contains(const GridXy& gridXy) const;
        [[nodiscard]] bool                          intersects(const Cluster& other, int shrinkInWorld) const;
        [[nodiscard]] bool                          isValid() const;
        [[nodiscard]] PhaseTransformation           phaseTransformation() const;
        [[nodiscard]] const geom::AlignedRectangle& boundingAlignedRectangle() const;

        /****** NON CONST FUNCTIONS  ******/
        void                 addGridXy(const GridXy& gridXy);
        void                 doCommand(Model& model);
        void                 update(double phaseFraction);
        void                 kill();
        void                 incrementCommandIndex();
        void                 clearCommands();
        void                 collideWithLevel(const Level& level, int shrinkInWorld);
        void                 handleDynamicBlock(const GridXy& point, DYNAMIC_BLOCK_TYPE type);
        void                 setState(CLUSTER_STATE state);
        void                 setWorldOffset(const WorldXy& worldOffset);
        void                 setPhase(PHASE phase);
        void                 resetPhase();
        void                 setPendingDynamicMoves(PENDING_DYNAMIC_MOVES pendingDynamicMoves);
        void                 spliceCluster(Level& level);
        void                 removeGridXy(const GridXy& gridXy);
        void                 sortGridXy() const;
        void                 swapGridXy(GridXyVector& other);
        void                 appendGridXy(const GridXyVector& other);
        Cluster              grabAllButFirstComponent();
        CommandVector&       commandVector();
        std::vector<Cluster> collectAllButFirstComponent();

        /****** FRIENDS  ******/
        friend std::ostream& operator<<(std::ostream& out, const Cluster& other);
        friend void          handleCommand(const Command_Simple& command, Cluster& cluster, Level& level);
        friend void          handleCommand(const Command_Modified& command, Cluster& cluster, Level& level);
        friend void          handleCommand(const Command_RepeatWrapper& command, Cluster& cluster, Level& level);

        void buildBoundingAlignedRectangle();

      private:
        /****** PRIVATE NON CONST FUNCTIONS  ******/
        void rotateClockWiseAbout(const GridXy& pivotGridXy);
        void rotateCounterClockWiseAbout(const GridXy& pivotGridXy);
        void setRotation(double angle, const GridXy& pivot);
        void grabAdjacentStoppedClusters(Level& level);

        /****** DATA MEMBERS  ******/
        mutable bool            m_gridXyAreSorted = false;
        mutable bool            m_sidesAreCorrect = false;
        double                  m_phaseFraction   = 0.0;
        double                  m_angle           = 0.0;
        size_t                  m_index;
        PENDING_DYNAMIC_MOVES   m_pendingDynamicMoves = PENDING_DYNAMIC_MOVES::ZERO;
        CLUSTER_STATE           m_state               = CLUSTER_STATE::ALIVE;
        PHASE                   m_phase               = PHASE::NONE;
        WorldXy                 m_worldOffset         = {0, 0};
        GridXy                  m_rotationPivot       = {0, 0};
        CommandVector           m_commandVector;
        mutable GridXyVector    m_gridXyVector;
        mutable WorldLineVector m_sides;
        std::string             m_name;
        geom::AlignedRectangle  m_boundingAlignedRectangle;
    };

} // namespace model

#endif // BLOCKS_CLUSTER_H
