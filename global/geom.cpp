//
// Created by pc on 26-12-20.
//

#include "geom.h"

#include <cassert>
#include <iostream>

namespace geom {
    SDL_Rect pad(const SDL_Rect& rect, int padding) {
        return {rect.x - padding, rect.y - padding, rect.w + 2 * padding, rect.h + 2 * padding};
    }

    model::WorldXY rotateClockWise(const model::WorldXY& point, double angleInDegrees) {
        if (angleInDegrees == 0.0) {
            return point;
        }

        const double ca = std::cos(-angleInDegrees * 2.0 * M_PI / 360.0);
        const double sa = std::sin(-angleInDegrees * 2.0 * M_PI / 360.0);

        return {static_cast<int>(ca * point.x() - sa * point.y()), static_cast<int>(sa * point.x() + ca * point.y())};
    }

    model::WorldXY rotateAboutPivot(const model::WorldXY& point, const model::WorldXY& pivot, double angleInDegrees) {
        return rotateClockWise(point - pivot, angleInDegrees) + pivot;
    }

    int cross(const model::WorldXY& lhs, const model::WorldXY& rhs) {
        return lhs.x() * rhs.y() - lhs.y() * rhs.x();
    }

    template <typename Type>
    static bool containedInClosedInterval(Type value, Type lower, Type upper) {
        if (lower > upper) {
            return containedInClosedInterval(value, upper, lower);
        }
        return value <= upper && value >= lower;
    }

    bool intersect(const model::Line<model::WorldXY>& lhs, const model::Line<model::WorldXY>& rhs) {
        const auto qMinusP = lhs.start() - rhs.start();
        const auto s       = lhs.displacementVector();
        const auto r       = rhs.displacementVector();
        const auto rCrossS = cross(r, s);
        if (rCrossS == 0) {
            return false;
        }

        if (containedInClosedInterval(cross(qMinusP, s), 0, rCrossS) && containedInClosedInterval(cross(qMinusP, r), 0, rCrossS)) {
            return true;
        } else {
            return false;
        }
    }
} // namespace geom