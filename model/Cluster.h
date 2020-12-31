//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_CLUSTER_H
#define BLOCKS_CLUSTER_H

#include "Cluster_enums.h"
#include "Command_Temp.h"
#include "GridXY.h"
#include "Level_enums.h"
#include "Line.h"
#include "WorldXY.h"

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
        /****** PUBLIC ENUMS / TYPEDEFS  ******/
        enum class PHASE { NONE, TRANSLATING, ROTATING };

        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        explicit Cluster(std::set<GridXY>&& gridXY, std::string name);
        Cluster(const Cluster& other) = default;
        Cluster(Cluster&& other)      = default;
        Cluster& operator=(const Cluster& other) = default;
        Cluster& operator=(Cluster&& other) = default;

        Cluster(std::set<GridXY>&& gridXY, std::vector<Command_Temp> commands, size_t commandIndex, std::string name);

        /****** CONST GETTERS  ******/
        [[nodiscard]] double                           angle() const;
        [[nodiscard]] size_t                           index() const;
        [[nodiscard]] size_t                           commandIndex() const;
        [[nodiscard]] size_t                           size() const;
        [[nodiscard]] CLUSTER_STATE                    state() const;
        [[nodiscard]] COMMAND_MODIFIER                 currentModifier() const;
        [[nodiscard]] PHASE                            phase() const;
        [[nodiscard]] WorldXY                          dynamicWorldOffset() const;
        [[nodiscard]] WorldXY                          approximateCenter() const;
        [[nodiscard]] std::string                      string() const;
        [[nodiscard]] std::set<WorldXY>                cornerPoints(int shrinkInWorld) const;
        [[nodiscard]] std::set<Line<WorldXY>>          sides(int shrinkInWorld) const;
        [[nodiscard]] const GridXY&                    rotationPivot() const;
        [[nodiscard]] const std::string&               name() const;
        [[nodiscard]] const std::set<GridXY>&          gridXY() const;
        [[nodiscard]] const std::vector<Command_Temp>& commands() const;

        /****** CONST FUNCTIONS  ******/
        [[nodiscard]] bool                                   empty() const;
        [[nodiscard]] bool                                   isAlive() const;
        [[nodiscard]] bool                                   isConnected() const;
        [[nodiscard]] bool                                   contains(const GridXY& gridXY) const;
        [[nodiscard]] bool                                   intersects(const Cluster& other, int shrinkInWorld) const;
        [[nodiscard]] bool                                   isAdjacent(const Cluster& other) const;
        [[nodiscard]] std::function<WorldXY(const WorldXY&)> phaseTransformation() const;

        /****** NON CONST FUNCTIONS  ******/
        void                       addGridXY(const GridXY& gridXY);
        void                       preStep();
        void                       doCommand(model::Model& model);
        void                       update(double phaseFraction);
        void                       addCommand(Command_Temp command);
        void                       kill();
        void                       buildSides();
        void                       incrementCommandIndex();
        void                       clearCommands();
        void                       collideWithLevel(const Level& level, int shrinkInWorld);
        void                       stopIfNeeded();
        void                       spliceIfNeeded(model::Model& model);
        void                       doOperation(const GridXY& point, DYNAMIC_BLOCK_TYPE type);
        model::Cluster             grabAllButFirstComponent();
        std::set<GridXY>::iterator removeBLock(const GridXY& gridXY);
        std::list<Cluster>         collectAllButFirstComponent();
        std::set<GridXY>&          gridXY();

        /****** FRIENDS  ******/
        friend std::ostream& operator<<(std::ostream& out, const Cluster& other);

      private:
        /****** PRIVATE  CONST FUNCTIONS  ******/
        [[nodiscard]] bool gridXUYAreUnique() const;

        /****** PRIVATE NON CONST FUNCTIONS  ******/
        void rotateClockWiseAbout(const GridXY& pivotGridXY);
        void rotateCounterClockWiseAbout(const GridXY& pivotGridXY);
        void setRotation(double angle, const GridXY& pivot);
        void grabAdjacentStoppedClusters(Level& level);
        void spliceCluster(Level& level);
        void resetPhase();

        /****** PRIVATE STATIC FUNCTIONS  ******/
        static Command_Temp rotateCommandClockWise(Command_Temp command);
        static Command_Temp rotateCommandCounterClockWise(Command_Temp command);

        /****** DATA MEMBERS  ******/
        CLUSTER_STATE             m_state         = CLUSTER_STATE::ALIVE;
        double                    m_phaseFraction = 0.0;
        double                    m_angle         = 0.0;
        size_t                    m_index;
        PHASE                     m_phase         = PHASE::NONE;
        WorldXY                   m_worldOffset   = {0, 0};
        GridXY                    m_rotationPivot = {0, 0};
        std::set<GridXY>          m_gridXYVector;
        std::vector<Command_Temp> m_commands;
        size_t                    m_commandIndex = 0;
        std::set<Line<WorldXY>>   m_sides;
        std::string               m_name;
    };
} // namespace model

#endif // BLOCKS_CLUSTER_H
