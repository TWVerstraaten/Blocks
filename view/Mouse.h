//
// Created by pc on 22-12-20.
//

#ifndef BLOCKS_MOUSE_H
#define BLOCKS_MOUSE_H

#include <SDL2/SDL.h>

class Mouse {

  public:
    static SDL_Point getMouseCoordinates();
};

#endif // BLOCKS_MOUSE_H
