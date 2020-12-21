//
// Created by pc on 21-12-20.
//

#include "ScreenCoordinates.h"

#include "../model/GridCoordinates.h"
#include "../model/WorldCoordinates.h"
#include "Grid.h"
#include "ScreenVector.h"

#include <cmath>

view::ScreenCoordinates::ScreenCoordinates(int x, int y) : m_x(x), m_y(y) {
}

int view::ScreenCoordinates::x() const {
    return m_x;
}

int view::ScreenCoordinates::y() const {
    return m_y;
}

view::ScreenCoordinates view::ScreenCoordinates::fromWorldCoordinates(const model::WorldCoordinates& worldCoordinates,
                                                                      const view::Grid&              grid) {
    return {static_cast<int>(worldCoordinates.x() * grid.scale()) + grid.xOffset(),
            static_cast<int>(worldCoordinates.y() * grid.scale()) + grid.yOffset()};
}

view::ScreenCoordinates view::ScreenCoordinates::fromGridCoordinates(const model::GridCoordinates& gridCoordinates,
                                                                     const view::Grid&             grid) {
    return {grid.xOffset() + gridCoordinates.x() * grid.blockSizeInScreen(),
            grid.yOffset() + gridCoordinates.y() * grid.blockSizeInScreen()};
}

view::ScreenCoordinates& view::ScreenCoordinates::operator+=(const view::ScreenCoordinates& other) {
    m_x += other.m_x;
    m_y += other.m_y;
    return *this;
}
view::ScreenCoordinates view::ScreenCoordinates::fromRotatedWorldCoordinates(const model::WorldCoordinates& worldCoordinates,
                                                                             const model::WorldCoordinates& pivot, double angle,
                                                                             const view::Grid& grid) {
    return view::ScreenCoordinates(0, 0);
}

view::ScreenCoordinates operator+(const view::ScreenCoordinates& lhs, const view::ScreenCoordinates& rhs) {
    view::ScreenCoordinates result{lhs.m_x, lhs.m_y};
    result += rhs;
    return result;
}

view::ScreenCoordinates operator+(const view::ScreenCoordinates& lhs, const view::ScreenVector& rhs) {
    view::ScreenCoordinates result{lhs.m_x + rhs.x(), lhs.m_y + rhs.y()};
    return result;
}
