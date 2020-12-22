//
// Created by pc on 22-12-20.
//

#include "Mouse.h"

SDL_Point Mouse::getMouseXY() {
    int xMouse, yMouse;
    SDL_GetMouseState(&xMouse, &yMouse);
    return {xMouse, yMouse};
}
