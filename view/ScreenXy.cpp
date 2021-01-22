//
// Created by pc on 21-12-20.
//

#include "ScreenXy.h"

#include "../model/GridXy.h"
#include "../model/WorldXy.h"
#include "ViewPort.h"

namespace view {
    ScreenXy::ScreenXy() : m_x(0), m_y(0) {
    }

    ScreenXy::ScreenXy(int x, int y) : m_x(x), m_y(y) {
    }

    int ScreenXy::x() const {
        return m_x;
    }

    int ScreenXy::y() const {
        return m_y;
    }

    ScreenXy ScreenXy::fromWorldXy(const model::WorldXy& worldXy, const ViewPort& viewPort) {
        return {viewPort.worldToScreen(worldXy.x()) + viewPort.xOffset(), viewPort.worldToScreen(worldXy.y()) + viewPort.yOffset()};
    }

    ScreenXy ScreenXy::fromWorldXyAsVector(const model::WorldXy& worldXy, const ViewPort& viewPort) {
        return {viewPort.worldToScreen(worldXy.x()), viewPort.worldToScreen(worldXy.y())};
    }

    ScreenXy ScreenXy::fromGridXy(const model::GridXy& gridXy, const ViewPort& viewPort) {
        return fromWorldXy(model::WorldXy{gridXy}, viewPort);
    }

    ScreenXy::ScreenXy(const QPointF& point) : m_x(static_cast<int>(point.x())), m_y(static_cast<int>(point.y())) {
    }

    ScreenXy::ScreenXy(const QPoint& point) : m_x(point.x()), m_y(point.y()) {
    }

    std::ostream& operator<<(std::ostream& out, const ScreenXy& screenXy) {
        out << "ScreenXy(" << screenXy.m_x << ", " << screenXy.m_y << ")";
        return out;
    }

} // namespace view

view::ScreenXy operator-(const view::ScreenXy& lhs, const view::ScreenXy& rhs) {
    return {lhs.x() - rhs.x(), lhs.y() - rhs.y()};
}

view::ScreenXy operator+(const view::ScreenXy& lhs, const view::ScreenXy& rhs) {
    return {lhs.x() + rhs.x(), lhs.y() + rhs.y()};
}
