//
// Created by pc on 16-12-20.
//

#include "Aux.h"

namespace view {
    namespace aux {

        SDL_Rect pad(const SDL_Rect& rect, int padding) {
            return {rect.x - padding, rect.y - padding, rect.w + 2 * padding, rect.h + 2 * padding};
        }

    } // namespace aux
} // namespace view
