//
// Created by teunv on 1/8/2021.
//

#ifndef BLOCKS_MODEL_TYPEDEFS_H
#define BLOCKS_MODEL_TYPEDEFS_H

#include "GridXy.h"
#include "WorldLine.h"

#include <set>
#include <vector>

namespace model {
    typedef std::vector<model::WorldLine> WorldLineVector;
    typedef std::set<model::GridXy>       GridXySet;
    typedef std::set<model::WorldXy>      WorldXySet;
} // namespace model

#endif // BLOCKS_MODEL_TYPEDEFS_H
