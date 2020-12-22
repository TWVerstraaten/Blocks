//
// Created by pc on 21-12-20.
//

#include "ScreenXY.h"

#include "../model/GridXY.h"
#include "../model/WorldXY.h"
#include "Grid.h"
#include "ScreenVector.h"

view::ScreenXY::ScreenXY(int x, int y) : m_x(x), m_y(y) {
}

int view::ScreenXY::x() const {
    return m_x;
}

int view::ScreenXY::y() const {
    return m_y;
}

view::ScreenXY view::ScreenXY::fromWorldXY(const model::WorldXY& worldXY,
                                                                      const view::Grid&              grid) {
    return {static_cast<int>(worldXY.x() * grid.scale()) + grid.xOffset(),
            static_cast<int>(worldXY.y() * grid.scale()) + grid.yOffset()};
}

view::ScreenXY view::ScreenXY::fromGridXY(const model::GridXY& gridXY,
                                                                     const view::Grid&             grid) {
    return fromWorldXY(model::WorldXY::fromGridXY(gridXY), grid);
}

view::ScreenXY operator+(const view::ScreenXY& lhs, const view::ScreenXY& rhs) {
    return {lhs.x() + rhs.x(), lhs.y() + rhs.y()};
}

view::ScreenXY operator+(const view::ScreenXY& lhs, const view::ScreenVector& rhs) {
    return {lhs.x() + rhs.x(), lhs.y() + rhs.y()};
}
