//
// Created by pc on 21-12-20.
//

#ifndef BLOCKS_COLOR_H
#define BLOCKS_COLOR_H

#include <SDL2/SDL.h>

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
    static const SDL_Color ACTION_EDIT_BACKGROUND_INACTIVE = {120, 120, 120, 255};
    static const SDL_Color ACTION_EDIT_HIGHLIGHT_DEAD      = {150, 150, 150, 255};
    static const SDL_Color ACTION_EDIT_BACKGROUND          = {180, 180, 228, 255};
    static const SDL_Color ACTION_EDIT_HIGHLIGHT_HARD      = {255, 200, 250, 255};
    static const SDL_Color ACTION_EDIT_HIGHLIGHT_SOFT      = {220, 200, 250, 255};

} // namespace color

#endif // BLOCKS_COLOR_H
