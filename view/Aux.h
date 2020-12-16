//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_AUX_H
#define BLOCKS_AUX_H

#include <SDL2/SDL.h>

namespace view {
    namespace aux {

        SDL_Rect pad(const SDL_Rect& rect, size_t padding);

    } // namespace aux
} // namespace view

#endif // BLOCKS_AUX_H
