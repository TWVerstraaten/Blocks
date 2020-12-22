//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_AUX_H
#define BLOCKS_AUX_H

#include "../model/WorldCoordinates.h"

#include <SDL2/SDL.h>

namespace aux {

    SDL_Rect pad(const SDL_Rect& rect, int padding);

    model::WorldCoordinates rotateClockWise(const model::WorldCoordinates& point, double angleInDegrees);
    model::WorldCoordinates rotateClockWiseAboutPivot(const model::WorldCoordinates& point,
                                                      const model::WorldCoordinates& pivot,
                                                      double                         angleInDegrees);

    bool pointInBlock(const model::WorldCoordinates& point, const model::GridCoordinates& gridCoordinates);

    //    bool                    pointInAlignedRectangle(const SDL_Rect& rect, const Point& point);
    //    bool                    pointInRectangle(const SDL_Rect& rect, double angle, const Point& pivot, const Point& point);
    //    Point                   center(const SDL_Rect& rect);
    //
} // namespace aux

#endif // BLOCKS_AUX_H
