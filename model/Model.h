//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_MODEL_H
#define BLOCKS_MODEL_H

#include "../Global.h"
#include "Cluster.h"
#include "Level.h"

#include <vector>

class SDL_Renderer;

namespace model {

    class Model {

      public:
        Model();
        //        explicit Model(const std::string& path);

        Model(const Model& other);

        Model& operator=(const Model& other) = default;

        void                               preStep();
        void                               interactClustersWithDynamicBlocks();
        void                               interactClustersWithInstantBlocks();
        void                               interactClustersWithLevel();
        void                               update(double fractionOfPhase);
        void                               init();
        void                               clear();
        void                               clearEmptyClusters();
        const Level&                       level() const;
        const std::vector<model::Cluster>& clusters() const;
        std::vector<model::Cluster>&       clusters();

      private:

        void splitDisconnectedClusters();

        Level                m_level;
        std::vector<Cluster> m_clusters;
    };

} // namespace model

#endif // BLOCKS_MODEL_H
