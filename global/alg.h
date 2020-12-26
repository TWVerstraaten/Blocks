//
// Created by pc on 26-12-20.
//

#ifndef BLOCKS_ALG_H
#define BLOCKS_ALG_H

#include "../model/GridXY.h"
#include "../model/Line.h"

#include <algorithm>
#include <set>
#include <vector>

namespace alg {

    bool intersect(const std::set<model::Line<model::WorldXY>>& lines1, const std::set<model::Line<model::WorldXY>>& lines2);

    template <typename XY>
    std::set<model::Line<XY>> getSidesInGridXY(std::set<model::GridXY> blocks) {
        std::set<model::Line<XY>> result;
        std::vector<model::GridXY>     cornerPoints;
        int                            yOffset = 0;
        for (const auto& dir : {enums::DIRECTION::UP, enums::DIRECTION::DOWN}) {
            auto it = blocks.begin();
            while (it != blocks.end()) {
                if (blocks.find(it->neighbor(dir)) == blocks.end()) {
                    const model::GridXY start = *it;
                    int                 idx   = it->x();
                    do {
                        ++it;
                        ++idx;
                    } while (it != blocks.end() && it->x() == idx && it->y() == start.y() &&
                             (blocks.find(it->neighbor(dir)) == blocks.end()));
                    result.emplace(XY(start) + model::GridXY{0, yOffset}, XY(model::GridXY{idx, start.y()} + model::GridXY{0, yOffset}));
                    cornerPoints.emplace_back(start + model::GridXY{0, yOffset});
                    cornerPoints.emplace_back(model::GridXY{idx, start.y()} + model::GridXY{0, yOffset});
                } else {
                    ++it;
                }
            }
            ++yOffset;
        }
        std::sort(cornerPoints.begin(), cornerPoints.end(), [](const model::GridXY& lhs, const model::GridXY& rhs) {
            return lhs.x() == rhs.x() ? lhs.y() < rhs.y() : lhs.x() < rhs.x();
        });
        while (not cornerPoints.empty()) {
            result.emplace(model::Line<XY>{*(cornerPoints.rbegin() + 1), *cornerPoints.rbegin()});
            cornerPoints.erase(cornerPoints.end() - 2, cornerPoints.end());
        }
        return result;
    }

} // namespace alg

#endif // BLOCKS_ALG_H
