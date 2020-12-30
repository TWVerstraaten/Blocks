//
// Created by pc on 26-12-20.
//

#ifndef BLOCKS_ALG_H
#define BLOCKS_ALG_H

#include "../model/GridXY.h"
#include "../model/Line.h"

#include <set>

namespace alg {

    bool intersect(const std::set<model::Line<model::WorldXY>>& lines1, const std::set<model::Line<model::WorldXY>>& lines2);
    std::set<model::Line<model::WorldXY>> getSidesFromGridXY(std::set<model::GridXY> blocks);

} // namespace alg

#endif // BLOCKS_ALG_H
