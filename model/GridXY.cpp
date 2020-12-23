//
// Created by pc on 15-12-20.
//

#include "GridXY.h"

#include "WorldXY.h"

#include <cassert>
#include <cmath>

namespace model {
    GridXY::GridXY() : m_x(0), m_y(0) {
    }

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

    GridXY GridXY::adjacent(enums::DIRECTION direction) const {
        assert(direction != enums::DIRECTION::NONE);
        switch (direction) {
            case enums::DIRECTION::UP:
                return {x(), y() - 1};
            case enums::DIRECTION::DOWN:
                return {x(), y() + 1};
            case enums::DIRECTION::LEFT:
                return {x() - 1, y()};
            case enums::DIRECTION::RIGHT:
                return {x() + 1, y()};
            default:
                return {x(), y()};
        }
    }

    bool operator==(const GridXY& lhs, const GridXY& rhs) {
        return lhs.m_y == rhs.m_y && lhs.m_x == rhs.m_x;
    }

    GridXY& GridXY::operator+=(const GridXY& gridXY) {
        m_y += gridXY.y();
        m_x += gridXY.x();
        return *this;
    }

    GridXY GridXY::fromWorldXY(const WorldXY& worldXY) {
        return {static_cast<int>(std::floor(static_cast<double>(worldXY.x()) / model::WorldXY::m_blockSizeInWorld)),
                static_cast<int>(std::floor(static_cast<double>(worldXY.y()) / model::WorldXY::m_blockSizeInWorld))};
    }

    GridXY operator+(const GridXY& lhs, const GridXY& rhs) {
        return {lhs.m_x + rhs.m_x, lhs.m_y + rhs.m_y};
    }

    GridXY::operator WorldXY() const {
        return WorldXY::fromGridXY(*this);
    }

    bool GridXY::isAdjacent(const GridXY& other) const {
        return manhattanDistance(other) == 1;
    }

} // namespace model