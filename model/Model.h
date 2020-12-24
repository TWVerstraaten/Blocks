//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_MODEL_H
#define BLOCKS_MODEL_H

#include "../global/Conf.h"
#include "Cluster.h"
#include "Level.h"

#include <memory>
#include <vector>

class SDL_Renderer;

namespace model {

    class Model {

      public:
        Model();

        Model& operator=(const Model& other) {
            m_level    = other.m_level;
            m_clusters = other.m_clusters;
            return *this;
        };

        void                        preStep();
        void                        interactClustersWithDynamicBlocks();
        void                        interactClustersWithInstantBlocks();
        void                        interactClustersWithLevel();
        void                        update(double fractionOfPhase);
        void                        init();
        void                        clear();
        void                        clearEmptyClusters();
        void                        addBlock(const GridXY& gridXY);
        void                        linkBlocks(const GridXY& base, const GridXY& extension);
        const Level&                level() const;
        const std::vector<Cluster>& clusters() const;
        std::vector<Cluster>&       clusters();

        void clearBlock(const GridXY gridXY);

      private:
        void splitDisconnectedClusters();

        Level                m_level;
        std::vector<Cluster> m_clusters;
    };

} // namespace model

#endif // BLOCKS_MODEL_H
