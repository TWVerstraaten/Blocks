//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_AUX_H
#define BLOCKS_AUX_H

#include "../model/WorldXY.h"

#include <SDL2/SDL.h>
#include <string>

namespace aux {

    SDL_Rect pad(const SDL_Rect& rect, int padding);

    model::WorldXY rotateClockWise(const model::WorldXY& point, double angleInDegrees);
    model::WorldXY rotateAboutPivot(const model::WorldXY& point, const model::WorldXY& pivot, double angleInDegrees);

    bool pointInBlock(const model::WorldXY& point, const model::GridXY& gridXY);
    int  clamp(int value, int lower, int upper);

    std::string trimWhiteSpace(const std::string& string);

} // namespace aux

#endif // BLOCKS_AUX_H
