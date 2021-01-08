//
// Created by pc on 26-12-20.
//

#include "geom.h"

#include "../model/GridXY.h"
#include "defines.h"

#include <vector>

#ifdef _WIN32
#include <algorithm>
#include <cassert>
#include <cmath>
#endif

namespace geom {

    bool intersect(const model::WorldLineSet& lines1, const model::WorldLineSet& lines2) {
        for (const auto& line1 : lines1) {
            for (const auto& line2 : lines2) {
                if (geom::intersect(line1, line2)) {
                    return true;
                }
            }
        }
        return false;
    }

    model::WorldLineSet getSidesFromGridXY(const model::GridXYSet& blocks) {
        model::WorldLineSet        result;
        std::vector<model::GridXY> cornerPoints;
        int                        yOffset = 0;
        for (const auto& dir : {model::GridXY::DIRECTION::UP, model::GridXY::DIRECTION::DOWN}) {
            auto it = blocks.begin();
            while (it != blocks.end()) {
                if (blocks.find(it->neighbor(dir)) != blocks.end()) {
                    ++it;
                } else {
                    const model::GridXY start = *it;
                    int                 idx   = it->x();
                    do {
                        ++it;
                        ++idx;
                    } while (it != blocks.end() && it->x() == idx && it->y() == start.y() && (blocks.find(it->neighbor(dir)) == blocks.end()));
                    result.emplace(model::WorldXY(start) + model::GridXY{0, yOffset},
                                   model::WorldXY(model::GridXY{idx, start.y()} + model::GridXY{0, yOffset}));
                    cornerPoints.emplace_back(start + model::GridXY{0, yOffset});
                    cornerPoints.emplace_back(model::GridXY{idx, start.y()} + model::GridXY{0, yOffset});
                }
            }
            ++yOffset;
        }
        std::sort(__IT(cornerPoints), __FUNC_2(lhs, rhs, lhs.x() == rhs.x() ? lhs.y() < rhs.y() : lhs.x() < rhs.x()));
        while (not cornerPoints.empty()) {
            result.emplace(model::WorldLine{*(cornerPoints.rbegin() + 1), *cornerPoints.rbegin()});
            cornerPoints.erase(cornerPoints.end() - 2, cornerPoints.end());
        }
        return result;
    }

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

    bool intersect(const model::WorldLine& lhs, const model::WorldLine& rhs) {
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

    int minX(const model::GridXYSet& blocks) {
        assert(not blocks.empty());
        return std::min_element(__CIT(blocks), __FUNC_2(lhs, rhs, lhs.x() < rhs.x()))->x();
    }

    int minY(const model::GridXYSet& blocks) {
        assert(not blocks.empty());
        return blocks.begin()->y();
    }

    int maxX(const model::GridXYSet& blocks) {
        assert(not blocks.empty());
        return std::max_element(__CIT(blocks), __FUNC_2(lhs, rhs, lhs.x() < rhs.x()))->x();
    }

    int maxY(const model::GridXYSet& blocks) {
        assert(not blocks.empty());
        return blocks.rbegin()->y();
    }

} // namespace geom