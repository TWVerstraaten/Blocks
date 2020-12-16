//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_MODEL_H
#define BLOCKS_MODEL_H

#include "../view/ActionEditBox.h"
#include "Cluster.h"
#include "Level.h"

#include <vector>

class SDL_Renderer;

namespace model {

    class Model {

      public:
        Model();

        void                        moveClusters();
        void                        interactClustersWithDynamicBlocks();
        void                        interactClustersWithInstantBlocks();
        void                        update(double fractionOfPhase);
        const Level&                level() const;
        const std::vector<Cluster>& clusters() const;

      private:
        Level                m_level;
        std::vector<Cluster> m_clusters;
    };

} // namespace model

#endif // BLOCKS_MODEL_H
