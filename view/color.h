//
// Created by pc on 28-12-20.
//

#ifndef BLOCKS_COLOR_H
#define BLOCKS_COLOR_H

#include <SDL.h>

namespace view::color {
    static const SDL_Color WHITE               = {255, 255, 255, 255};
    static const SDL_Color GREY                = {120, 120, 120, 255};
    static const SDL_Color RED                 = {255, 0, 0, 255};
    static const SDL_Color BLACK               = {0, 0, 0, 255};
    static const SDL_Color TEXT_ERROR          = {200, 50, 50, 255};
    static const SDL_Color BACKGROUND          = {8, 33, 73, 255};
    static const SDL_Color BACKGROUND_PLAYABLE = {35, 123, 193, 255};
    static const SDL_Color BACKGROUND_START    = {119, 181, 230, 255};
    static const SDL_Color BACKGROUND_SPLICE   = {230, 181, 119, 255};
    //    static const SDL_Color BACKGROUND_START             = {216, 191, 216, 255};
    static const SDL_Color EDIT_BOX_BACKGROUND_INACTIVE = {120, 120, 120, 255};
    static const SDL_Color EDIT_BOX_HIGHLIGHT_DEAD      = {150, 150, 150, 255};
    static const SDL_Color EDIT_BOX_BACKGROUND          = {210, 210, 255, 255};
    static const SDL_Color EDIT_BOX_HIGHLIGHT_HARD      = {255, 200, 250, 255};
    static const SDL_Color EDIT_BOX_HIGHLIGHT_SOFT      = {220, 200, 250, 255};
    static const SDL_Color SCROLL_AREA_BACKGROUND       = {140, 140, 180, 255};
} // namespace view::color

#endif // BLOCKS_COLOR_H
