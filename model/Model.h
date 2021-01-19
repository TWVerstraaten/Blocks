//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_MODEL_H
#define BLOCKS_MODEL_H

#include "Cluster.h"
#include "Level.h"

#include <list>

namespace model {

    class Model {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        Model();
        Model(const Model& other) = default;
        Model(Model&& other)      = default;
        Model& operator           =(const Model& other);

        /****** CONST GETTERS  ******/
        [[nodiscard]] const Level&              level() const;
        [[nodiscard]] const std::list<Cluster>& clusters() const;

        /****** CONST FUNCTIONS  ******/
        [[nodiscard]] bool noLiveOrStoppedClusterOnBlock(const GridXy& gridXy) const;

        /****** NON CONST FUNCTIONS  ******/
        void                         init();
        void                         clear();
        void                         startPhase();
        void                         update(double dPhase);
        void                         clearEmpty();
        Level&                       level();
        std::list<Cluster>&          clusters();
        std::list<Cluster>::iterator clusterWithIndex(size_t index);
        std::list<Cluster>::iterator clusterContaining(const GridXy& point);

      private:
        /****** PRIVATE NON CONST FUNCTIONS  ******/
        void intersectWithLevel();
        void intersectClusters();
        void updateInternal(double dPhase);

        /****** DATA MEMBERS  ******/
        double             m_phaseFraction = 0.0;
        Level              m_level;
        std::list<Cluster> m_clusters;
    };

} // namespace model

#endif // BLOCKS_MODEL_H
