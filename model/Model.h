//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_MODEL_H
#define BLOCKS_MODEL_H

#include "../view/widget/ActionEditBox.h"
#include "Cluster.h"
#include "Level.h"

#include <list>

class SDL_Renderer;

namespace model {

    class Model {

      public:
        Model();

        void                             interactClustersWithDynamicBlocks();
        void                             interactClustersWithInstantBlocks();
        void                             interactClustersWithLevel();
        void                             update(double fractionOfPhase);
        void                             init();
        void                             clear();
        const Level&                     level() const;
        const std::list<model::Cluster>& clusters() const;

      private:
        Level              m_level;
        std::list<Cluster> m_clusters;
    };

} // namespace model

#endif // BLOCKS_MODEL_H
