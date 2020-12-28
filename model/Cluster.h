//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_CLUSTER_H
#define BLOCKS_CLUSTER_H

#include "Command.h"
#include "Level.h"
#include "Line.h"
#include "WorldXY.h"

#include <functional>
#include <iostream>
#include <list>
#include <set>
#include <vector>

class SDL_Renderer;

namespace model {

    class Cluster {
        /****** PRIVATE STATIC DATA MEMBERS  ******/
        static size_t s_maxClusterIndex;

      public:
        /****** PUBLIC ENUMS / TYPEDEFS  ******/
        enum class PHASE { NONE, TRANSLATING, ROTATING };

        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        explicit Cluster(std::set<GridXY>&& gridXY, const std::string& name = "");
        explicit Cluster(const std::set<GridXY>& gridXY, std::string name = "");
        Cluster(const Cluster& other) = default;
        Cluster(Cluster&& other)      = default;
        Cluster& operator=(const Cluster& other) = default;
        Cluster& operator=(Cluster&& other) = default;

        /****** CONST GETTERS  ******/
        double                      angle() const;
        size_t                      index() const;
        size_t                      commandIndex() const;
        size_t                      size() const;
        PHASE                       phase() const;
        WorldXY                     dynamicWorldOffset() const;
        WorldXY                     approximateCenter() const;
        Command                     currentCommand() const;
        std::string                 string() const;
        std::set<WorldXY>           cornerPoints(int shrinkInWorld) const;
        std::set<Line<WorldXY>>     sides(int shrinkInWorld) const;
        std::set<GridXY>&           gridXY();
        const GridXY&               rotationPivot() const;
        const std::string&          name() const;
        const std::set<GridXY>&     gridXY() const;
        const std::vector<Command>& commands() const;

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
        void                       doCommand();
        void                       addPendingOperation(const GridXY& gridXY, DYNAMIC_BLOCK_TYPE blockType);
        void                       performPendingOperationOrNextCommand();
        void                       update(double phaseFraction);
        void                       addCommand(Command command);
        void                       kill();
        void                       clearCommands();
        void                       collideWithLevel(const Level& level, int shrinkInWorld);
        model::Cluster             grabAllButFirstComponent();
        std::set<GridXY>::iterator removeBLock(const GridXY& gridXY);
        std::list<Cluster>         collectAllButFirstComponent();

        /****** FRIENDS  ******/
        friend std::ostream& operator<<(std::ostream& out, const Cluster& other);

      private:
        /****** PRIVATE  CONST FUNCTIONS  ******/
        bool gridXUYAreUnique() const;

        /****** PRIVATE NON CONST FUNCTIONS  ******/
        void rotateClockWiseAbout(const GridXY& pivotGridXY);
        void rotateCounterClockWiseAbout(const GridXY& pivotGridXY);
        void resetPhase();
        void setRotation(double angle, const GridXY& pivot);
        void incrementCommandIndex();
        void tryPendingOperation();

        /****** PRIVATE STATIC FUNCTIONS  ******/
        static Command rotateCommandClockWise(Command command);
        static Command rotateCommandCounterClockWise(Command command);

        /****** DATA MEMBERS  ******/
        bool                                       m_alive         = true;
        double                                     m_phaseFraction = 0.0;
        double                                     m_angle         = 0.0;
        size_t                                     m_index;
        size_t                                     m_commandIndex  = 0;
        PHASE                                      m_phase         = PHASE::NONE;
        WorldXY                                    m_worldOffset   = {0, 0};
        GridXY                                     m_rotationPivot = {0, 0};
        std::string                                m_name;
        std::vector<Command>                       m_commands;
        std::map<const GridXY, DYNAMIC_BLOCK_TYPE> m_pendingOperations;
        std::set<GridXY>                           m_gridXYVector;
        std::set<Line<GridXY>>                     m_sides;
    };
} // namespace model

#endif // BLOCKS_CLUSTER_H
