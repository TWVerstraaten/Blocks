//
// Created by pc on 16-12-20.
//

#include "Aux.h"

namespace view {
    namespace aux {

        SDL_Rect pad(const SDL_Rect& rect, size_t padding) {
            return {rect.x - static_cast<int>(padding), rect.y - static_cast<int>(padding),
                    static_cast<int>(rect.w + 2 * padding), static_cast<int>(rect.h + 2 * padding)};
        }

    } // namespace aux
} // namespace view
