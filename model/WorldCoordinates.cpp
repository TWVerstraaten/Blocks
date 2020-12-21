//
// Created by pc on 21-12-20.
//

#include "WorldCoordinates.h"

model::WorldCoordinates::WorldCoordinates(int x, int y) : m_x(x), m_y(y) {
}

int model::WorldCoordinates::x() const {
    return m_x;
}

int model::WorldCoordinates::y() const {
    return m_y;
}
