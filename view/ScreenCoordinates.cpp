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
    return fromWorldCoordinates(model::WorldCoordinates::fromGridCoordinates(gridCoordinates), grid);
}

view::ScreenCoordinates operator+(const view::ScreenCoordinates& lhs, const view::ScreenCoordinates& rhs) {
    return {lhs.x() + rhs.x(), lhs.y() + rhs.y()};
}

view::ScreenCoordinates operator+(const view::ScreenCoordinates& lhs, const view::ScreenVector& rhs) {
    return {lhs.x() + rhs.x(), lhs.y() + rhs.y()};
}
