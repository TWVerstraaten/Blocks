//
// Created by teunv on 1/8/2021.
//

#ifndef BLOCKS_APPLICATION_CONSTANTS_H
#define BLOCKS_APPLICATION_CONSTANTS_H

#include "../model/WorldXy.h"

namespace app {
    static const size_t         TIME_STEP_SLOW           = 500;
    static const size_t         TIME_STEP_MEDIUM         = 190;
    static const size_t         TIME_STEP_FAST           = 20;
    static const double         MAX_D_PHASE              = 0.1;
    static const int            BLOCK_SHRINK_IN_WORLD    = 3;
    static const int            BLOCK_SIZE_IN_WORLD      = 40;
    static const int            HALF_BLOCK_SIZE_IN_WORLD = BLOCK_SIZE_IN_WORLD / 2;
    static const model::WorldXy HALF_BLOCK_IN_WORLD      = {HALF_BLOCK_SIZE_IN_WORLD, HALF_BLOCK_SIZE_IN_WORLD};
} // namespace app

#endif // BLOCKS_APPLICATION_CONSTANTS_H
