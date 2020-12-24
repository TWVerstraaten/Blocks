//
// Created by pc on 16-12-20.
//

#include "Global.h"

#include "../model/GridXY.h"

#include <cassert>
#include <cmath>

namespace global {

    SDL_Rect pad(const SDL_Rect& rect, int padding) {
        return {rect.x - padding, rect.y - padding, rect.w + 2 * padding, rect.h + 2 * padding};
    }

    model::WorldXY rotateClockWise(const model::WorldXY& point, double angleInDegrees) {
        const double ca = std::cos(-angleInDegrees * 2.0 * M_PI / 360.0);
        const double sa = std::sin(-angleInDegrees * 2.0 * M_PI / 360.0);

        return {static_cast<int>(ca * point.x() - sa * point.y()), static_cast<int>(sa * point.x() + ca * point.y())};
    }

    model::WorldXY rotateAboutPivot(const model::WorldXY& point, const model::WorldXY& pivot, double angleInDegrees) {
        return rotateClockWise(point - pivot, angleInDegrees) + pivot;
    }

    bool pointInBlock(const model::WorldXY& point, const model::GridXY& gridXY) {
        return gridXY == model::GridXY::fromWorldXY(point);
    }

    int clamp(int value, int lower, int upper) {
        assert(lower <= upper);
        return value < lower ? lower : (value > upper ? upper : value);
    }

    std::string trimWhiteSpace(const std::string& string) {
        const auto startIndex = string.find_first_not_of(" \t");
        if (startIndex == std::string::npos) {
            return "";
        } else {
            const auto endIndex = string.find_last_not_of(" \t");
            return string.substr(startIndex, endIndex - startIndex + 1);
        }
    }

} // namespace global
