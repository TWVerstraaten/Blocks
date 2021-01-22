//
// Created by pc on 26-12-20.
//

#ifndef BLOCKS_GEOM_H
#define BLOCKS_GEOM_H

#include "../model/Cluster.h"
#include "../model/Model_typedefs.h"
#include "../model/WorldLine.h"
#include "../model/WorldXy.h"

#include <set>

namespace geom {
    bool                         intersect(const model::WorldLineVector& lines1, const model::WorldLineVector& lines2);
    bool                         intersect(const model::Cluster& cluster1, const model::Cluster& cluster2, int shrinkInWorld);
    bool                         intersect(const model::WorldLine& lhs, const model::WorldLine& rhs);
    int                          minX(const model::GridXyVector& blocks);
    int                          minY(const model::GridXyVector& blocks);
    int                          maxX(const model::GridXyVector& blocks);
    int                          maxY(const model::GridXyVector& blocks);
    int                          cross(const model::WorldXy& lhs, const model::WorldXy& rhs);
    model::WorldXy               rotateClockWise(const model::WorldXy& point, double angleInDegrees);
    model::WorldXy               rotateAboutPivot(const model::WorldXy& point, const model::WorldXy& pivot, double angleInDegrees);
    model::WorldLineVector       getSidesFromGridXy(const model::GridXyVector& blocks);
    std::vector<model::Cluster*> neighbors(std::list<model::Cluster>& clusters, model::GridXy point);
    std::vector<model::Cluster*> neighbors(std::list<model::Cluster>& clusters, const model::Cluster& cluster);
} // namespace geom

#endif // BLOCKS_GEOM_H
