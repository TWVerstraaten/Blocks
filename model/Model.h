//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_MODEL_H
#define BLOCKS_MODEL_H

#include "../action/AddClusterAction.h"
#include "../global/cst.h"
#include "Cluster.h"
#include "Level.h"

#include <list>
#include <memory>

namespace model {

    class Model {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        Model();
        Model& operator=(const Model& other);

        /****** CONST GETTERS  ******/
        const Level&              level() const;
        const std::list<Cluster>& clusters() const;

        /****** NON CONST FUNCTIONS  ******/
        void                            preStep();
        void                            init();
        void                            clear();
        void                            clearEmptyClusters();
        void                            startPhase();
        void                            finishInteractions();
        void                            update(double dPhase);
        std::list<Cluster>&             clusters();
        std::list<Cluster>::iterator    clusterWithIndex(size_t index);
        std::unique_ptr<action::Action> addCluster(const GridXY& gridXY);
        std::unique_ptr<action::Action> linkBlocks(const GridXY& base, const GridXY& extension);
        std::unique_ptr<action::Action> clearBlock(const GridXY& gridXY);

      private:
        /****** PRIVATE CONST FUNCTIONS  ******/
        bool containsEmptyClusters() const;

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
