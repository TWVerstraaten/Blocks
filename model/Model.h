//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_MODEL_H
#define BLOCKS_MODEL_H

#include "Cluster.h"
#include "Level.h"

#include <vector>

class SDL_Renderer;

namespace model {

    class Model {

      public:
        Model();

        void                        doStep();
        const std::vector<Cluster>& clusters() const;
        const Level&                level() const;

      private:
        std::vector<Cluster> m_clusters;
        Level                m_level;
    };

} // namespace model

#endif // BLOCKS_MODEL_H
