//
// Created by pc on 26-12-20.
//

#include "alg.h"

#include "defines.h"
#include "geom.h"

#include <algorithm>
#include <cassert>
#include <vector>


int alg::minX(const std::set<model::GridXY>& blocks) {
    assert(not blocks.empty());
    return std::min_element(_CIT_(blocks), _FUNC_2_(lhs, rhs, lhs.x() < rhs.x()))->x();
}

int alg::minY(const std::set<model::GridXY>& blocks) {
    assert(not blocks.empty());
    return blocks.begin()->y();
}

int alg::maxX(const std::set<model::GridXY>& blocks) {
    assert(not blocks.empty());
    return std::max_element(_CIT_(blocks), _FUNC_2_(lhs, rhs, lhs.x() < rhs.x()))->x();
}

int alg::maxY(const std::set<model::GridXY>& blocks) {
    assert(not blocks.empty());
    return blocks.rbegin()->y();
}

model::GridXY alg::topLeft(const std::set<model::GridXY>& blocks) {
    return {minX(blocks), minY(blocks)};
}
