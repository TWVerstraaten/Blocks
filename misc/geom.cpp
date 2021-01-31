//
// Created by pc on 26-12-20.
//

#include "geom.h"

#include "AlignedRectangle.h"
#include "defines.h"

#include <QDebug>
#include <cassert>
#include <cmath>

namespace geom {

    using namespace model;

    bool intersect(const WorldLineVector& lines1, const WorldLineVector& lines2) {
        return std::any_of(D_CIT(lines1), D_FUNC(line1, std::any_of(D_CIT(lines2), D_FUNC(line2, intersect(line1, line2)))));
    }

    int square(int a) {
        return a * a;
    }

    bool intersect(const Cluster& cluster1, const Cluster& cluster2, int shrinkInWorld) {
        const auto boundingAlignedRectangle1 = cluster1.boundingAlignedRectangle();
        const auto boundingAlignedRectangle2 = cluster2.boundingAlignedRectangle();

        const auto point1 = *cluster1.gridXyVector().begin();
        const auto point2 = *cluster2.gridXyVector().begin();

        if (square(point1.x() - point2.x()) + square(point1.y() - point2.y()) >
            square(boundingAlignedRectangle1.maxDimension() + boundingAlignedRectangle2.maxDimension())) {
            return false;
        }

        if (not boundingAlignedRectangle1.intersects(boundingAlignedRectangle2)) {
            return false;
        }
        return geom::intersect(cluster1.sides(shrinkInWorld), cluster2.sides(shrinkInWorld));
    }

    WorldLineVector getSidesFromGridXy(const GridXyVector& blocks) {
        assert(std::is_sorted(D_IT(blocks)));
        WorldLineVector     result;
        std::vector<GridXy> cornerPoints;
        for (const auto& dir : {GridXy::DIRECTION::UP, GridXy::DIRECTION::DOWN}) {
            for (auto it = blocks.begin(); it != blocks.end(); ++it) {
                if (std::find(D_CIT(blocks), it->neighbor(dir)) == blocks.end()) {
                    const auto start = *it;
                    auto       next  = std::next(it);
                    while (next != blocks.end() && *next - *it == GridXy{1, 0} && (std::find(D_CIT(blocks), next->neighbor(dir)) == blocks.end())) {
                        ++it;
                        ++next;
                    }
                    const GridXy point1 = start + GridXy{0, dir == GridXy::DIRECTION::UP ? 0 : 1};
                    const GridXy point2 = GridXy{it->x() + 1, start.y() + (dir == GridXy::DIRECTION::UP ? 0 : 1)};
                    result.emplace_back(WorldXy(point1), WorldXy(point2));
                    cornerPoints.emplace_back(point1);
                    cornerPoints.emplace_back(point2);
                }
            }
        }
        assert(cornerPoints.size() % 2 == 0);
        std::sort(D_IT(cornerPoints), D_FUNC_2(lhs, rhs, lhs.x() == rhs.x() ? lhs.y() < rhs.y() : lhs.x() < rhs.x()));
        for (auto it = cornerPoints.begin(); it != cornerPoints.end(); it += 2) {
            result.emplace_back(WorldLine{*it, *std::next(it)});
        }
        return result;
    }

    WorldXy rotateClockWise(const WorldXy& point, double angleInDegrees) {
        if (angleInDegrees == 0.0) {
            return point;
        }
        static const double DEGREES_TO_RADIANS = 2.0 * M_PI / 360.0;
        const double        ca                 = std::cos(-angleInDegrees * DEGREES_TO_RADIANS);
        const double        sa                 = std::sin(-angleInDegrees * DEGREES_TO_RADIANS);
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

    int minX(const GridXyVector& blocks) {
        assert(not blocks.empty());
        return std::min_element(D_CIT(blocks), D_FUNC_2(lhs, rhs, lhs.x() < rhs.x()))->x();
    }

    int minY(const GridXyVector& blocks) {
        assert(not blocks.empty());
        return std::min_element(D_CIT(blocks), D_FUNC_2(lhs, rhs, lhs.y() < rhs.y()))->y();
    }

    int maxX(const GridXyVector& blocks) {
        assert(not blocks.empty());
        return std::max_element(D_CIT(blocks), D_FUNC_2(lhs, rhs, lhs.x() < rhs.x()))->x();
    }

    int maxY(const GridXyVector& blocks) {
        assert(not blocks.empty());
        return std::max_element(D_CIT(blocks), D_FUNC_2(lhs, rhs, lhs.y() < rhs.y()))->y();
    }

    std::vector<Cluster*> neighbors(std::vector<Cluster>& clusters, GridXy point) {
        std::vector<Cluster*> result;
        std::for_each(D_IT(clusters), [&](auto& cluster) {
            if (cluster.gridXyIsAdjacent(point)) {
                result.push_back(&cluster);
            }
        });
        return result;
    }

    std::vector<Cluster*> neighbors(std::vector<Cluster>& clusters, const Cluster& cluster) {
        std::vector<Cluster*> result;
        std::for_each(D_IT(clusters), [&](auto& candidate) {
            if (candidate.isAdjacent(cluster)) {
                result.push_back(&candidate);
            }
        });
        return result;
    }

    void splitDisconnectedClusters(std::vector<Cluster>& clusters) {
        std::vector<Cluster> newClusters;
        for (auto& cluster : clusters) {
            if (not cluster.isConnected()) {
                const auto& components = cluster.collectAllButFirstComponent();
                std::copy(D_IT(components), std::back_inserter(newClusters));
                assert(cluster.isConnected());
            }
        }
        std::copy(D_IT(newClusters), std::back_inserter(clusters));
        for (const auto& cluster : clusters) {
            assert(cluster.isConnected());
        }
    }
} // namespace geom