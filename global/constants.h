//
// Created by pc on 22-12-20.
//

#ifndef BLOCKS_CONSTANTS_H
#define BLOCKS_CONSTANTS_H

#include <SDL.h>

namespace cst {

    static const Uint32 s_timeStepSlow       = 1000;
    static const Uint32 s_timeStepMedium     = 300;
    static const Uint32 s_timeStepFast       = 50;
    static const Uint32 s_actionEditBoxWidth = 150;
    const int           s_blockShrinkInWorld = 9;

    static const size_t s_padding     = 14;
    static const size_t s_titleHeight = 36;

} // namespace cst

#endif // BLOCKS_CONSTANTS_H
