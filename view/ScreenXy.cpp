//
// Created by pc on 21-12-20.
//

#include "ScreenXy.h"

#include "../model/GridXy.h"
#include "../model/WorldXy.h"
#include "ViewPort.h"

namespace view {
    ScreenXy::ScreenXy() : QPoint() {
    }

    ScreenXy::ScreenXy(int x, int y) : QPoint(x, y) {
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

    ScreenXy::ScreenXy(const QPointF& point) : QPoint(point.x(), point.y()) {
    }

    ScreenXy::ScreenXy(const QPoint& point) : QPoint(point) {
    }

    std::ostream& operator<<(std::ostream& out, const ScreenXy& screenXy) {
        out << "ScreenXy(" << screenXy.x() << ", " << screenXy.x() << ")";
        return out;
    }

} // namespace view

view::ScreenXy operator-(const view::ScreenXy& lhs, const view::ScreenXy& rhs) {
    return {lhs.x() - rhs.x(), lhs.y() - rhs.y()};
}

view::ScreenXy operator+(const view::ScreenXy& lhs, const view::ScreenXy& rhs) {
    return {lhs.x() + rhs.x(), lhs.y() + rhs.y()};
}
