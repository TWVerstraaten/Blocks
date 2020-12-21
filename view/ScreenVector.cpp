//
// Created by pc on 21-12-20.
//

#include "ScreenVector.h"

#include "../model/WorldVector.h"
#include "Grid.h"

view::ScreenVector::ScreenVector(int x, int y) : m_x(x), m_y(y) {
}

int view::ScreenVector::x() const {
    return m_x;
}

int view::ScreenVector::y() const {
    return m_y;
}

view::ScreenVector view::ScreenVector::fromWorldVector(const model::WorldVector& worldVector, const view::Grid& grid) {
    return {worldVector.x(), worldVector.y()};
}
