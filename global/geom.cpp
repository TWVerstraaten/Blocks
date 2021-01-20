//
// Created by pc on 26-12-20.
//

#include "geom.h"

#include "defines.h"

#include <cassert>

#ifdef _WIN32
#include <cmath>
#endif

namespace geom {

    using namespace model;

    bool intersect(const WorldLineSet& lines1, const WorldLineSet& lines2) {
        return std::any_of(D_CIT(lines1), D_FUNC(line1, std::any_of(D_CIT(lines2), D_FUNC(line2, intersect(line1, line2)))));
    }

    WorldLineSet getSidesFromGridXy(const GridXySet& blocks) {
        WorldLineSet        result;
        std::vector<GridXy> cornerPoints;
        for (const auto& dir : {GridXy::DIRECTION::UP, GridXy::DIRECTION::DOWN}) {
            for (auto it = blocks.begin(); it != blocks.end(); ++it) {
                if (blocks.find(it->neighbor(dir)) == blocks.end()) {
                    const auto start = *it;
                    auto       next  = std::next(it);
                    while (next != blocks.end() && *next - *it == GridXy{1, 0} && (blocks.find(next->neighbor(dir)) == blocks.end())) {
                        ++it;
                        ++next;
                    }
                    const GridXy point1 = start + GridXy{0, dir == GridXy::DIRECTION::UP ? 0 : 1};
                    const GridXy point2 = GridXy{it->x() + 1, start.y() + (dir == GridXy::DIRECTION::UP ? 0 : 1)};
                    result.emplace(WorldXy(point1), WorldXy(point2));
                    cornerPoints.emplace_back(point1);
                    cornerPoints.emplace_back(point2);
                }
            }
        }
        assert(cornerPoints.size() % 2 == 0);
        std::sort(D_IT(cornerPoints), D_FUNC_2(lhs, rhs, lhs.x() == rhs.x() ? lhs.y() < rhs.y() : lhs.x() < rhs.x()));
        for (auto it = cornerPoints.begin(); it != cornerPoints.end(); it += 2) {
            result.emplace(WorldLine{*it, *std::next(it)});
        }
        return result;
    }

    WorldXy rotateClockWise(const WorldXy& point, double angleInDegrees) {
        if (angleInDegrees == 0.0) {
            return point;
        }
        const double ca = std::cos(-angleInDegrees * 2.0 * M_PI / 360.0);
        const double sa = std::sin(-angleInDegrees * 2.0 * M_PI / 360.0);
        return {static_cast<int>(ca * point.x() - sa * point.y()), static_cast<int>(sa * point.x() + ca * point.y())};
    }

    WorldXy rotateAboutPivot(const WorldXy& point, const WorldXy& pivot, double angleInDegrees) {
        return rotateClockWise(point - pivot, angleInDegrees) + pivot;
    }

    int cross(const WorldXy& lhs, const WorldXy& rhs) {
        return lhs.x() * rhs.y() - lhs.y() * rhs.x();
    }

    template <typename Type>
    static bool containedInClosedInterval(Type value, Type lower, Type upper) {
        if (lower > upper) {
            return containedInClosedInterval(value, upper, lower);
        }
        return value <= upper && value >= lower;
    }

    bool intersect(const WorldLine& lhs, const WorldLine& rhs) {
        const auto s       = lhs.displacementVector();
        const auto r       = rhs.displacementVector();
        const auto rCrossS = cross(r, s);
        if (rCrossS == 0) {
            return false;
        }
        const auto qMinusP = lhs.start() - rhs.start();
        return (containedInClosedInterval(cross(qMinusP, s), 0, rCrossS) && containedInClosedInterval(cross(qMinusP, r), 0, rCrossS));
    }

    int minX(const GridXySet& blocks) {
        assert(not blocks.empty());
        return std::min_element(D_CIT(blocks), D_FUNC_2(lhs, rhs, lhs.x() < rhs.x()))->x();
    }

    int minY(const GridXySet& blocks) {
        assert(not blocks.empty());
        return blocks.begin()->y();
    }

    int maxX(const GridXySet& blocks) {
        assert(not blocks.empty());
        return std::max_element(D_CIT(blocks), D_FUNC_2(lhs, rhs, lhs.x() < rhs.x()))->x();
    }

    int maxY(const GridXySet& blocks) {
        assert(not blocks.empty());
        return blocks.rbegin()->y();
    }

    std::vector<Cluster*> neighbors(std::list<Cluster>& clusters, GridXy point) {
        std::vector<Cluster*> result;
        std::for_each(D_IT(clusters), [&](auto& cluster) {
            if (cluster.gridXyIsAdjacent(point)) {
                result.push_back(&cluster);
            }
        });
        return result;
    }

    std::vector<Cluster*> neighbors(std::list<Cluster>& clusters, const Cluster& cluster) {
        std::vector<Cluster*> result;
        std::for_each(D_IT(clusters), [&](auto& candidate) {
            if (candidate.isAdjacent(cluster)) {
                result.push_back(&candidate);
            }
        });
        return result;
    }
} // namespace geom