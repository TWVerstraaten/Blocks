//
// Created by pc on 16-12-20.
//

#include "Aux.h"

#include "../model/WorldCoordinates.h"

#include <iostream>

namespace aux {

    SDL_Rect pad(const SDL_Rect& rect, int padding) {
        return {rect.x - padding, rect.y - padding, rect.w + 2 * padding, rect.h + 2 * padding};
    }

    model::WorldCoordinates rotateClockWise(const model::WorldCoordinates& point, double angleInDegrees) {
        const double ca = std::cos(-angleInDegrees * 2.0 * M_PI / 360.0);
        const double sa = std::sin(-angleInDegrees * 2.0 * M_PI / 360.0);

        return {static_cast<int>(ca * point.x() - sa * point.y()), static_cast<int>(sa * point.x() + ca * point.y())};
    }

    model::WorldCoordinates rotateClockWiseAboutPivot(const model::WorldCoordinates& point, const model::WorldCoordinates& pivot,
                                                      double angleInDegrees) {
        return rotateClockWise(point - pivot, angleInDegrees) + pivot;
    }

    bool pointInBlock(const model::WorldCoordinates& point, const model::GridCoordinates& gridCoordinates) {
        return gridCoordinates == model::GridCoordinates::fromWorldCoordinates(point);
    }

    //    bool pointInAlignedRectangle(const SDL_Rect& rect, const Point& point) {
    //        const SDL_Point p = point;
    //        return SDL_PointInRect(&p, &rect);
    //    }
    //
    //    bool pointInRectangle(const SDL_Rect& rect, double angle, const Point& pivot, const Point& point) {
    //        const auto rotatedPoint = rotateClockWiseAboutPivot(point, pivot, angle);
    //        return pointInAlignedRectangle(rect, rotatedPoint);
    //    }
    //
    //    Point center(const SDL_Rect& rect) {
    //        return Point(rect.x + rect.w / 2, rect.y + rect.h / 2);
    //    }

} // namespace aux
