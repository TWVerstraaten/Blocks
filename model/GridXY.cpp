//
// Created by pc on 15-12-20.
//

#include "GridXY.h"

#include "../view/ScreenXY.h"
#include "../view/ViewPort.h"

#include <cassert>
#include <cmath>

namespace model {
    GridXY::GridXY(int x, int y) : m_x(x), m_y(y) {
    }

    int GridXY::y() const {
        return m_y;
    }

    int GridXY::x() const {
        return m_x;
    }

    bool operator<(const GridXY& lhs, const GridXY& rhs) {
        return lhs.y() == rhs.y() ? lhs.x() < rhs.x() : lhs.y() < rhs.y();
    }

    int GridXY::manhattanDistance(const GridXY& other) const {
        return std::abs(other.y() - y()) + std::abs(other.x() - x());
    }

    GridXY GridXY::neighbor(model::GridXY::DIRECTION direction) const {
        switch (direction) {
            case model::GridXY::DIRECTION::UP:
                return {x(), y() - 1};
            case model::GridXY::DIRECTION::DOWN:
                return {x(), y() + 1};
            case model::GridXY::DIRECTION::LEFT:
                return {x() - 1, y()};
            case model::GridXY::DIRECTION::RIGHT:
                return {x() + 1, y()};
            default:
                return {x(), y()};
        }
    }

    bool operator==(const GridXY& lhs, const GridXY& rhs) {
        return (lhs.m_y == rhs.m_y) && (lhs.m_x == rhs.m_x);
    }

    GridXY& GridXY::operator+=(const GridXY& gridXY) {
        m_y += gridXY.y();
        m_x += gridXY.x();
        return *this;
    }

    GridXY GridXY::fromWorldXY(const WorldXY& worldXY) {
        return {static_cast<int>(std::floor(static_cast<double>(worldXY.x()) / cst::BLOCK_SIZE_IN_WORLD)),
                static_cast<int>(std::floor(static_cast<double>(worldXY.y()) / cst::BLOCK_SIZE_IN_WORLD))};
    }

    GridXY operator+(const GridXY& lhs, const GridXY& rhs) {
        return {lhs.m_x + rhs.m_x, lhs.m_y + rhs.m_y};
    }

    GridXY operator-(const GridXY& lhs, const GridXY& rhs) {
        return {lhs.m_x - rhs.m_x, lhs.m_y - rhs.m_y};
    }

    GridXY::operator WorldXY() const {
        return WorldXY::fromGridXY(*this);
    }

    bool GridXY::isAdjacent(const GridXY& other) const {
        return manhattanDistance(other) == 1;
    }

    GridXY GridXY::fromScreenXY(const view::ScreenXY& screenXY, const view::ViewPort& viewPort) {
        return GridXY(std::floor((screenXY.x() - viewPort.xOffset()) / static_cast<double>(viewPort.blockSizeInScreen())),
                      std::floor((screenXY.y() - viewPort.yOffset()) / static_cast<double>(viewPort.blockSizeInScreen())));
    }

    bool operator!=(const GridXY& lhs, const GridXY& rhs) {
        return not(lhs == rhs);
    }

} // namespace model