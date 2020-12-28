//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_MODEL_H
#define BLOCKS_MODEL_H

#include "../Actions/AddClusterAction.h"
#include "../global/cst.h"
#include "Cluster.h"
#include "Level.h"

#include <list>
#include <memory>

class SDL_Renderer;

namespace model {

    class Model {

      public:
        Model();

        Model& operator=(const Model& other);

        void                            preStep();
        void                            interactClustersWithDynamicBlocks();
        void                            interactClustersWithInstantBlocks();
        void                            update(double dPhase);
        void                            init();
        void                            clear();
        void                            clearEmptyClusters();
        void                            startPhase();
        std::unique_ptr<action::Action> addCluster(const GridXY& gridXY);
        std::unique_ptr<action::Action> linkBlocks(const GridXY& base, const GridXY& extension);
        std::unique_ptr<action::Action> clearBlock(const GridXY& gridXY);
        void                            finishInteractions();
        const Level&                    level() const;
        const std::list<Cluster>&       clusters() const;
        std::list<Cluster>&             clusters();
        std::list<Cluster>::iterator    clusterWithIndex(size_t index);

      private:
        void intersectWithLevel();
        void intersectClusters();
        void splitDisconnectedClusters();
        void updateInternal(double dPhase);
        bool containsEmptyClusters() const;

        bool               m_needsPreStep  = false;
        double             m_phaseFraction = 0.0;
        Level              m_level;
        std::list<Cluster> m_clusters;
    };

} // namespace model

#endif // BLOCKS_MODEL_H
