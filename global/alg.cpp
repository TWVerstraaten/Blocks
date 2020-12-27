//
// Created by pc on 26-12-20.
//

#include "alg.h"

#include "geom.h"

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
