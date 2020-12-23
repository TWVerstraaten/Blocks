//
// Created by pc on 21-12-20.
//

#include "ScreenVector.h"

#include "../model/WorldVector.h"
#include "ViewPort.h"

view::ScreenVector::ScreenVector(int x, int y) : m_x(x), m_y(y) {
}

int view::ScreenVector::x() const {
    return m_x;
}

int view::ScreenVector::y() const {
    return m_y;
}

view::ScreenVector view::ScreenVector::fromWorldVector(const model::WorldVector& worldVector, const view::ViewPort& viewPort) {
    return {static_cast<int>(worldVector.x() * viewPort.scale()), static_cast<int>(worldVector.y() * viewPort.scale())};
}
