//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_CLUSTER_H
#define BLOCKS_CLUSTER_H

#include "../misc/AlignedRectangle.h"
#include "Cluster_enums.h"
#include "GridXy.h"
#include "GridXyContainer.h"
#include "Level_enums.h"
#include "Model_typedefs.h"
#include "PhaseTransformation.h"
#include "WorldLine.h"
#include "WorldXy.h"
#include "cmd/Command.h"
#include "cmd/CommandVector.h"

namespace io {
    std::istream& operator>>(std::istream& in, model::Cluster& cluster);
} // namespace io

namespace model {

    class Level;
    class Model;

    class Cluster : public GridXyContainer {

        /****** PRIVATE STATIC DATA MEMBERS  ******/
        static size_t s_maxClusterIndex;

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        Cluster();
        Cluster(GridXyContainer&& gridXyContainer, std::string name);
        Cluster(GridXyContainer&& gridXyContainer, CommandVector commandVector, std::string name);

        /****** CONST GETTERS  ******/
        [[nodiscard]] double               angle() const;
        [[nodiscard]] size_t               index() const;
        [[nodiscard]] CLUSTER_STATE        state() const;
        [[nodiscard]] COMMAND_TYPE         currentType() const;
        [[nodiscard]] WorldXy              dynamicWorldOffset() const;
        [[nodiscard]] std::string          string() const;
        [[nodiscard]] const std::string&   name() const;
        [[nodiscard]] const CommandVector& commandVector() const;
        [[nodiscard]] PHASE                phase() const;

        /****** CONST FUNCTIONS  ******/
        [[nodiscard]] bool                          isAlive() const;
        [[nodiscard]] bool                          intersects(const Cluster& other, int shrinkInWorld) const;
        [[nodiscard]] PhaseTransformation           phaseTransformation() const;
        [[nodiscard]] const geom::AlignedRectangle& boundingAlignedRectangle() const;
        [[nodiscard]] WorldLineVector               sides(int shrinkInWorld) const;

        /****** NON CONST FUNCTIONS  ******/
        void                 doCommand(Model& model);
        void                 update(double phaseFraction);
        void                 kill();
        void                 incrementCommandIndex();
        void                 intersectWithLevel(const Level& level, int shrinkInWorld);
        void                 handleDynamicBlock(const GridXy& point, DYNAMIC_BLOCK_TYPE type);
        void                 setState(CLUSTER_STATE state);
        void                 setWorldOffset(const WorldXy& worldOffset);
        void                 setPhase(PHASE phase);
        void                 resetPhase();
        void                 spliceCluster(Level& level);
        void                 setPhaseFraction(double phaseFraction);
        CommandVector&       commandVector();
        std::vector<Cluster> collectAllButFirstComponent();

        static void   setMaxClusterIndex(size_t maxClusterIndex);
        static size_t maxClusterIndex();

        /****** FRIENDS  ******/
        friend void handleCommand(const Command_Simple& command, Cluster& cluster, Level& level);
        friend void handleCommand(const Command_Modified& command, Cluster& cluster, Level& level);
        friend void handleCommand(const Command_RepeatWrapper& command, Cluster& cluster, Level& level);

        void buildBoundingAlignedRectangle();

      private:
        friend std::istream& io::operator>>(std::istream& in, model::Cluster& cluster);

        /****** PRIVATE NON CONST FUNCTIONS  ******/
        void setRotation(double angle, const GridXy& pivot);

        /****** DATA MEMBERS  ******/
        double                 m_phaseFraction = 0.0;
        double                 m_angle         = 0.0;
        size_t                 m_index;
        CLUSTER_STATE          m_state         = CLUSTER_STATE::ALIVE;
        PHASE                  m_phase         = PHASE::NONE;
        WorldXy                m_worldOffset   = {0, 0};
        GridXy                 m_rotationPivot = {0, 0};
        CommandVector          m_commandVector;
        std::string            m_name;
        geom::AlignedRectangle m_boundingAlignedRectangle;
    };

} // namespace model

#endif // BLOCKS_CLUSTER_H
