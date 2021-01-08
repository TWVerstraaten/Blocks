//
// Created by pc on 22-12-20.
//

#ifndef BLOCKS_CST_H
#define BLOCKS_CST_H

#include "../model/WorldXY.h"

#include <SDL.h>

namespace cst {

    static const Uint32 BLOCK_SELECT_WIDGET_HEIGHT = 150;
    static const Uint32 BLOCK_SELECT_WIDGET_WIDTH  = 230;

    static const int            INITIAL_SCREEN_WIDTH      = 1000;
    static const int            INITIAL_SCREEN_HEIGHT     = 800;
    static const int            BLOCK_SHRINK_IN_WORLD     = 3;
    static const int            BLOCK_SIZE_IN_WORLD       = 40;
    static const int            HALF_BLOCK_SIZE_IN_WORLD  = BLOCK_SIZE_IN_WORLD / 2;
    static const model::WorldXY HALF_BLOCK_IN_WORLD       = {HALF_BLOCK_SIZE_IN_WORLD, HALF_BLOCK_SIZE_IN_WORLD};
    static const Uint32         TIME_STEP_SLOW            = 1000;
    static const Uint32         TIME_STEP_MEDIUM          = 300;
    static const Uint32         TIME_STEP_FAST            = 20;
    static const Uint32         LINE_EDIT_WIDTH           = 150;
    static const size_t         LINE_EDIT_PADDING         = 4;
    static const size_t         LINE_EDIT_TITLE_HEIGHT    = 44;
    static const Uint32         MAIN_FONT_SIZE            = 15;
    static const Uint32         SMALL_FONT_SIZE           = 12;
    static const int            COMMAND_SCROLL_AREA_WIDTH = 200;
    static const int            LINE_EDIT_X_OFFSET        = 25;

    static const double MAX_D_PHASE = 0.1;

} // namespace cst

#endif // BLOCKS_CST_H
