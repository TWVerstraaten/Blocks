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
        Model& operator=(const Model& other);

        /****** CONST GETTERS  ******/
        [[nodiscard]] const Level&              level() const;
        [[nodiscard]] const std::list<Cluster>& clusters() const;

        /****** CONST FUNCTIONS  ******/
        [[nodiscard]] bool noClusterOnBlock(const GridXY& gridXY) const;

        /****** NON CONST FUNCTIONS  ******/
        void                         preStep();
        void                         init();
        void                         clear();
        void                         startPhase();
        void                         finishInteractions();
        void                         update(double dPhase);
        Level&                       level();
        std::list<Cluster>&          clusters();
        std::list<Cluster>::iterator clusterWithIndex(size_t index);
        std::list<Cluster>::iterator clusterContaining(const GridXY& point);

      private:
        /****** PRIVATE NON CONST FUNCTIONS  ******/
        void intersectWithLevel();
        void intersectClusters();
        void updateInternal(double dPhase);

        /****** DATA MEMBERS  ******/
        bool               m_needsPreStep  = false;
        double             m_phaseFraction = 0.0;
        Level              m_level;
        std::list<Cluster> m_clusters;
    };

} // namespace model

#endif // BLOCKS_MODEL_H
