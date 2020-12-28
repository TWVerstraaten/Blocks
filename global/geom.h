//
// Created by pc on 26-12-20.
//

#ifndef BLOCKS_GEOM_H
#define BLOCKS_GEOM_H

#include "../model/Line.h"
#include "../model/WorldXY.h"

#include <SDL.h>

namespace geom {
    int            cross(const model::WorldXY& lhs, const model::WorldXY& rhs);
    bool           intersect(const model::Line<model::WorldXY>& lhs, const model::Line<model::WorldXY>& rhs);
    SDL_Rect       pad(const SDL_Rect& rect, int padding);
    model::WorldXY rotateClockWise(const model::WorldXY& point, double angleInDegrees);
    model::WorldXY rotateAboutPivot(const model::WorldXY& point, const model::WorldXY& pivot, double angleInDegrees);

} // namespace geom

#endif // BLOCKS_GEOM_H
