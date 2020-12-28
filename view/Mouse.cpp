//
// Created by pc on 22-12-20.
//

#include "Mouse.h"

namespace view {
    ScreenXY Mouse::MouseXY() {
        int xMouse, yMouse;
        SDL_GetMouseState(&xMouse, &yMouse);
        return {xMouse, yMouse};
    }
} // namespace view