//
// Created by pc on 26-12-20.
//

#ifndef BLOCKS_GEOM_H
#define BLOCKS_GEOM_H

#include "../model/Model_typedefs.h"
#include "../model/WorldLine.h"
#include "../model/WorldXY.h"

#include <SDL.h>
#include <set>

namespace geom {
    bool                intersect(const model::WorldLineSet& lines1, const model::WorldLineSet& lines2);
    bool                intersect(const model::WorldLine& lhs, const model::WorldLine& rhs);
    int                 minX(const model::GridXYSet& blocks);
    int                 minY(const model::GridXYSet& blocks);
    int                 maxX(const model::GridXYSet& blocks);
    int                 maxY(const model::GridXYSet& blocks);
    int                 cross(const model::WorldXY& lhs, const model::WorldXY& rhs);
    SDL_Rect            pad(const SDL_Rect& rect, int padding);
    model::WorldXY      rotateClockWise(const model::WorldXY& point, double angleInDegrees);
    model::WorldXY      rotateAboutPivot(const model::WorldXY& point, const model::WorldXY& pivot, double angleInDegrees);
    model::WorldLineSet getSidesFromGridXY(const model::GridXYSet& blocks);
} // namespace geom

#endif // BLOCKS_GEOM_H
