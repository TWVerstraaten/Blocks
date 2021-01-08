//
// Created by pc on 26-12-20.
//

#include "alg.h"

#include "defines.h"
#include "geom.h"

#include <algorithm>
#include <cassert>
#include <vector>

bool alg::intersect(const std::set<model::Line<model::WorldXY>>& lines1, const std::set<model::Line<model::WorldXY>>& lines2) {
    for (const auto& line1 : lines1) {
        for (auto line2 : lines2) {
            if (geom::intersect(line1, line2)) {
                return true;
            }
        }
    }
    return false;
}

std::set<model::Line<model::WorldXY>> alg::getSidesFromGridXY(std::set<model::GridXY> blocks) {
    std::set<model::Line<model::WorldXY>> result;
    std::vector<model::GridXY>            cornerPoints;
    int                                   yOffset = 0;
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
    std::sort(_IT_(cornerPoints), _FUNC_2_(lhs, rhs, lhs.x() == rhs.x() ? lhs.y() < rhs.y() : lhs.x() < rhs.x()));
    while (not cornerPoints.empty()) {
        result.emplace(model::Line<model::WorldXY>{*(cornerPoints.rbegin() + 1), *cornerPoints.rbegin()});
        cornerPoints.erase(cornerPoints.end() - 2, cornerPoints.end());
    }
    return result;
}

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
