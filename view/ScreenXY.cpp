//
// Created by pc on 21-12-20.
//

#include "ScreenXY.h"

#include "../model/GridXY.h"
#include "../model/WorldXY.h"
#include "ViewPort.h"

view::ScreenXY::ScreenXY(int x, int y) : m_x(x), m_y(y) {
}

int view::ScreenXY::x() const {
    return m_x;
}

int view::ScreenXY::y() const {
    return m_y;
}

view::ScreenXY view::ScreenXY::fromWorldXY(const model::WorldXY& worldXY, const view::ViewPort& viewPort) {
    return {static_cast<int>(worldXY.x() * viewPort.scale()) + viewPort.xOffset(),
            static_cast<int>(worldXY.y() * viewPort.scale()) + viewPort.yOffset()};
}

view::ScreenXY view::ScreenXY::fromWorldXYAsVector(const model::WorldXY& worldXY, const view::ViewPort& viewPort) {
    return {static_cast<int>(worldXY.x() * viewPort.scale()), static_cast<int>(worldXY.y() * viewPort.scale())};
}

view::ScreenXY view::ScreenXY::fromGridXY(const model::GridXY& gridXY, const view::ViewPort& viewPort) {
    return fromWorldXY(model::WorldXY::fromGridXY(gridXY), viewPort);
}

view::ScreenXY::ScreenXY(const SDL_Point& point) : m_x(point.x), m_y(point.y) {
}

view::ScreenXY operator+(const view::ScreenXY& lhs, const view::ScreenXY& rhs) {
    return {lhs.x() + rhs.x(), lhs.y() + rhs.y()};
}
