//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_GLOBAL_H
#define BLOCKS_GLOBAL_H

#include "../model/WorldXY.h"

#include <SDL2/SDL.h>
#include <string>

namespace global {

    SDL_Rect pad(const SDL_Rect& rect, int padding);

    model::WorldXY rotateClockWise(const model::WorldXY& point, double angleInDegrees);
    model::WorldXY rotateAboutPivot(const model::WorldXY& point, const model::WorldXY& pivot, double angleInDegrees);

    bool pointInBlock(const model::WorldXY& point, const model::GridXY& gridXY);
    int  clamp(int value, int lower, int upper);

    std::string trimWhiteSpace(const std::string& string);

} // namespace global

#endif // BLOCKS_GLOBAL_H
