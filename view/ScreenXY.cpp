//
// Created by pc on 21-12-20.
//

#include "ScreenXY.h"

#include "../model/GridXY.h"
#include "../model/WorldXY.h"
#include "ViewPort.h"

namespace view {
    ScreenXY::ScreenXY() : m_x(0), m_y(0) {
    }

    ScreenXY::ScreenXY(int x, int y) : m_x(x), m_y(y) {
    }

    int ScreenXY::x() const {
        return m_x;
    }

    int ScreenXY::y() const {
        return m_y;
    }

    ScreenXY ScreenXY::fromWorldXY(const model::WorldXY& worldXY, const ViewPort& viewPort) {
        return {viewPort.worldToScreen(worldXY.x()) + viewPort.xOffset(), viewPort.worldToScreen(worldXY.y()) + viewPort.yOffset()};
    }

    ScreenXY ScreenXY::fromWorldXYAsVector(const model::WorldXY& worldXY, const ViewPort& viewPort) {
        return {viewPort.worldToScreen(worldXY.x()), viewPort.worldToScreen(worldXY.y())};
    }

    ScreenXY ScreenXY::fromGridXY(const model::GridXY& gridXY, const ViewPort& viewPort) {
        return fromWorldXY(gridXY, viewPort);
    }

    ScreenXY::ScreenXY(const SDL_Point& point) : m_x(point.x), m_y(point.y) {
    }

    ScreenXY::operator SDL_Point() const {
        return {m_x, m_y};
    }
    ScreenXY::ScreenXY(const QPointF& point) : m_x(point.x()), m_y(point.y()) {
    }

    ScreenXY::ScreenXY(const QPoint& point) : m_x(point.x()), m_y(point.y()) {
    }

    std::ostream& operator<<(std::ostream& out, const ScreenXY& screenXy) {
        out << "ScreenXY(" << screenXy.m_x << ", " << screenXy.m_y << ")";
        return out;
    }

} // namespace view

view::ScreenXY operator-(const view::ScreenXY& lhs, const view::ScreenXY& rhs) {
    return {lhs.x() - rhs.x(), lhs.y() - rhs.y()};
}

view::ScreenXY operator+(const view::ScreenXY& lhs, const view::ScreenXY& rhs) {
    return {lhs.x() + rhs.x(), lhs.y() + rhs.y()};
}
