//
// Created by pc on 21-12-20.
//

#include "WorldXy.h"

#include "../app/Application_constants.h"
#include "../view/ViewPort.h"
#include "GridXy.h"

namespace model {

    WorldXy::WorldXy(int x, int y) noexcept : m_x(x), m_y(y) {
    }

    int WorldXy::x() const {
        return m_x;
    }

    int WorldXy::y() const {
        return m_y;
    }

    WorldXy WorldXy::fromGridXy(const model::GridXy& gridXy) {
        return {app::BLOCK_SIZE_IN_WORLD * gridXy.x(), app::BLOCK_SIZE_IN_WORLD * gridXy.y()};
    }

    bool operator<(const WorldXy& lhs, const WorldXy& rhs) {
        return lhs.y() == rhs.y() ? lhs.x() < rhs.x() : lhs.y() < rhs.y();
    }

    bool operator==(const WorldXy& lhs, const WorldXy& rhs) {
        return lhs.x() == rhs.x() && lhs.y() == rhs.y();
    }

    model::WorldXy operator-(const model::WorldXy& lhs, const model::WorldXy& rhs) {
        return {lhs.x() - rhs.x(), lhs.y() - rhs.y()};
    }

    model::WorldXy operator+(const model::WorldXy& lhs, const model::WorldXy& rhs) {
        return {lhs.x() + rhs.x(), lhs.y() + rhs.y()};
    }

    model::WorldXy operator/(const model::WorldXy& lhs, double a) {
        return {static_cast<int>(lhs.x() / a), static_cast<int>(lhs.y() / a)};
    }

    WorldXy::WorldXy(const GridXy& gridXy) {
        *this = fromGridXy(gridXy);
    }

    WorldXy& WorldXy::operator+=(const WorldXy& other) {
        m_x += other.m_x;
        m_y += other.m_y;
        return *this;
    }

    WorldXy& WorldXy::operator/=(int divisor) {
        m_x /= divisor;
        m_y /= divisor;
        return *this;
    }

    WorldXy WorldXy::fromScreenXy(const view::ScreenXy& screenXy, const view::ViewPort& viewPort) {
        return {viewPort.screenToWorld(screenXy.x() - viewPort.xOffset()), viewPort.screenToWorld(screenXy.y() - viewPort.yOffset())};
    }

} // namespace model
