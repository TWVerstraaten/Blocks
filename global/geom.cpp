//
// Created by pc on 26-12-20.
//

#include "geom.h"

#include "defines.h"

#include <algorithm>
#include <cassert>
#include <vector>

#ifdef _WIN32
#include <cmath>
#endif

namespace geom {

    bool intersect(const model::WorldLineSet& lines1, const model::WorldLineSet& lines2) {
        return std::any_of(D_CIT(lines1), D_FUNC(line1, std::any_of(D_CIT(lines2), D_FUNC(line2, intersect(line1, line2)))));
    }

    model::WorldLineSet getSidesFromGridXY(const model::GridXYSet& blocks) {
        model::WorldLineSet        result;
        std::vector<model::GridXY> cornerPoints;
        for (const auto& dir : {model::GridXY::DIRECTION::UP, model::GridXY::DIRECTION::DOWN}) {
            for (auto it = blocks.begin(); it != blocks.end(); ++it) {
                if (blocks.find(it->neighbor(dir)) == blocks.end()) {
                    const auto start = *it;
                    auto       next  = std::next(it);
                    while (next != blocks.end() && *next - *it == model::GridXY{1, 0} && (blocks.find(next->neighbor(dir)) == blocks.end())) {
                        ++it;
                        ++next;
                    }
                    const model::GridXY point1 = start + model::GridXY{0, dir == model::GridXY::DIRECTION::UP ? 0 : 1};
                    const model::GridXY point2 = model::GridXY{it->x() + 1, start.y() + (dir == model::GridXY::DIRECTION::UP ? 0 : 1)};
                    result.emplace(model::WorldXY(point1), model::WorldXY(point2));
                    cornerPoints.emplace_back(point1);
                    cornerPoints.emplace_back(point2);
                }
            }
        }
        assert(cornerPoints.size() % 2 == 0);
        std::sort(D_IT(cornerPoints), D_FUNC_2(lhs, rhs, lhs.x() == rhs.x() ? lhs.y() < rhs.y() : lhs.x() < rhs.x()));
        for (auto it = cornerPoints.begin(); it != cornerPoints.end(); it += 2) {
            result.emplace(model::WorldLine{*it, *std::next(it)});
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
        const auto s       = lhs.displacementVector();
        const auto r       = rhs.displacementVector();
        const auto rCrossS = cross(r, s);
        if (rCrossS == 0) {
            return false;
        }
        const auto qMinusP = lhs.start() - rhs.start();
        return (containedInClosedInterval(cross(qMinusP, s), 0, rCrossS) && containedInClosedInterval(cross(qMinusP, r), 0, rCrossS));
    }

    int minX(const model::GridXYSet& blocks) {
        assert(not blocks.empty());
        return std::min_element(D_CIT(blocks), D_FUNC_2(lhs, rhs, lhs.x() < rhs.x()))->x();
    }

    int minY(const model::GridXYSet& blocks) {
        assert(not blocks.empty());
        return blocks.begin()->y();
    }

    int maxX(const model::GridXYSet& blocks) {
        assert(not blocks.empty());
        return std::max_element(D_CIT(blocks), D_FUNC_2(lhs, rhs, lhs.x() < rhs.x()))->x();
    }

    int maxY(const model::GridXYSet& blocks) {
        assert(not blocks.empty());
        return blocks.rbegin()->y();
    }

    std::vector<model::Cluster*> neighbors(std::list<model::Cluster>& clusters, model::GridXY point) {
        std::vector<model::Cluster*> result;
        std::for_each(D_IT(clusters), [&](auto& cluster) {
            if (cluster.gridXYIsAdjacent(point)) {
                result.push_back(&cluster);
            }
        });
        return result;
    }

    std::vector<model::Cluster*> neighbors(std::list<model::Cluster>& clusters, const model::Cluster& cluster) {
        std::vector<model::Cluster*> result;
        std::for_each(D_IT(clusters), [&](auto& candidate) {
            if (candidate.isAdjacent(cluster)) {
                result.push_back(&candidate);
            }
        });
        return result;
    }
} // namespace geom