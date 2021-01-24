//
// Created by pc on 23-01-21.
//

#ifndef BLOCKS_MAININTERFACE_H
#define BLOCKS_MAININTERFACE_H

#include "../model/Level_enums.h"
#include "model/Cluster.h"
#include "model/GridXy.h"

#include <map>

namespace model {
    class Model;
}

namespace view {
    class CommandScroll;
}

namespace cont {

    class MainInterface {

      public:
        static void startInteractPhase(model::Model& model, view::CommandScroll& commandScrollArea);
        static void startMovePhase(model::Model& model, view::CommandScroll& commandScrollArea);

      private:
        static void handleKillBlocks(const std::map<model::GridXy, model::INSTANT_BLOCK_TYPE>& instantBlocks, std::vector<model::Cluster>& clusters);
        static void handleDynamicBlocks(const std::map<model::GridXy, model::DYNAMIC_BLOCK_TYPE>& dynamicBlocks,
                                        std::vector<model::Cluster>&                              clusters);
    };

} // namespace cont

#endif // BLOCKS_MAININTERFACE_H
