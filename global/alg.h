//
// Created by pc on 26-12-20.
//

#ifndef BLOCKS_ALG_H
#define BLOCKS_ALG_H

#include "../model/GridXY.h"

#include <cassert>
#include <set>

namespace alg {
    int           minX(const std::set<model::GridXY>& blocks);
    int           minY(const std::set<model::GridXY>& blocks);
    int           maxX(const std::set<model::GridXY>& blocks);
    int           maxY(const std::set<model::GridXY>& blocks);
    model::GridXY topLeft(const std::set<model::GridXY>& blocks);
    template <typename Type>
    Type clamp(Type value, Type lower, Type upper) {
        assert(lower <= upper);
        return value < lower ? lower : (value > upper ? upper : value);
    }
} // namespace alg

#endif // BLOCKS_ALG_H
