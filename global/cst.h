//
// Created by pc on 22-12-20.
//

#ifndef BLOCKS_CST_H
#define BLOCKS_CST_H

#include <SDL.h>

namespace cst {
    static const int BLOCK_SHRINK_IN_WORLD = 4;

    static const Uint32 TIME_STEP_SLOW   = 1000;
    static const Uint32 TIME_STEP_MEDIUM = 300;
    static const Uint32 TIME_STEP_FAST   = 50;

    static const Uint32 LINE_EDIT_WIDTH        = 150;
    static const size_t LINE_EDIT_PADDING      = 4;
    static const size_t LINE_EDIT_TITLE_HEIGHT = 44;

    static const double MAX_D_PHASE = 0.1;

    namespace color {

        static const SDL_Color WHITE                           = {255, 255, 255, 255};
        static const SDL_Color RED                             = {255, 0, 0, 255};
        static const SDL_Color BLACK                           = {0, 0, 0, 255};
        static const SDL_Color TEXT_ERROR                      = {200, 50, 50, 255};
        static const SDL_Color GREEN                           = {0, 255, 0, 255};
        static const SDL_Color BLUE                            = {0, 0, 255, 255};
        static const SDL_Color GRID_LINE_COLOR                 = {200, 200, 200, 255};
        static const SDL_Color BACKGROUND                      = {12, 43, 103, 255};
        static const SDL_Color BACKGROUND_PLAYABLE             = {35, 123, 193, 255};
        static const SDL_Color BACKGROUND_START                = {119, 181, 230, 255};
        static const SDL_Color ACTION_EDIT_BACKGROUND_INACTIVE = {120, 120, 120, 255};
        static const SDL_Color ACTION_EDIT_HIGHLIGHT_DEAD      = {150, 150, 150, 255};
        static const SDL_Color ACTION_EDIT_BACKGROUND          = {180, 180, 228, 255};
        static const SDL_Color ACTION_EDIT_HIGHLIGHT_HARD      = {255, 200, 250, 255};
        static const SDL_Color ACTION_EDIT_HIGHLIGHT_SOFT      = {220, 200, 250, 255};
    } // namespace color

} // namespace cst

#endif // BLOCKS_CST_H
