//
// Created by teunv on 1/8/2021.
//

#ifndef BLOCKS_MODEL_TYPEDEFS_H
#define BLOCKS_MODEL_TYPEDEFS_H

#include "GridXY.h"
#include "WorldLine.h"

#include <set>

namespace model {
    typedef std::set<model::WorldLine> WorldLineSet;
    typedef std::set<model::GridXY>    GridXYSet;
    typedef std::set<model::WorldXY>   WorldXYSet;
} // namespace model

#endif // BLOCKS_MODEL_TYPEDEFS_H
