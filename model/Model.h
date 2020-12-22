//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_MODEL_H
#define BLOCKS_MODEL_H

#include "../view/widget/ActionEditBox.h"
#include "Cluster.h"
#include "Level.h"

#include <vector>

class SDL_Renderer;

namespace model {

    class Model {

      public:
        Model();
        Model(const Model& other);

        Model& operator=(const Model& other);

        void                               interactClustersWithDynamicBlocks();
        void                               interactClustersWithInstantBlocks();
        void                               interactClustersWithLevel();
        void                               update(double fractionOfPhase);
        void                               init();
        void                               clear();
        void                               clearEmptyClusters();
        void                               setClusters(const std::vector<model::Cluster>& clusters);
        const Level&                       level() const;
        const std::vector<model::Cluster>& clusters() const;
        std::vector<model::Cluster>&       clusters();

      private:
        Level                m_level;
        std::vector<Cluster> m_clusters;
    };

} // namespace model

#endif // BLOCKS_MODEL_H
