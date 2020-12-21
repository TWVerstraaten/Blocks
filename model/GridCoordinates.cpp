//
// Created by pc on 15-12-20.
//

#include "GridCoordinates.h"

#include <cassert>
#include <cmath>

namespace model {
    GridCoordinates::GridCoordinates(int x, int y) : m_x(x), m_y(y) {
    }

    int GridCoordinates::y() const {
        return m_y;
    }

    int GridCoordinates::x() const {
        return m_x;
    }

    bool operator<(const GridCoordinates& lhs, const GridCoordinates& rhs) {
        return lhs.y() == rhs.y() ? lhs.x() < rhs.x() : lhs.y() < rhs.y();
    }

    size_t GridCoordinates::manhattanDistance(const GridCoordinates& other) const {
        return std::abs(other.y() - y()) + std::abs(other.x() - x());
    }

    GridCoordinates GridCoordinates::adjacent(enums::DIRECTION direction) const {
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

    bool operator==(const GridCoordinates& lhs, const GridCoordinates& rhs) {
        return lhs.m_y == rhs.m_y && lhs.m_x == rhs.m_x;
    }

    GridCoordinates& GridCoordinates::operator+=(const GridCoordinates& indexPair) {
        m_y += indexPair.y();
        m_x += indexPair.x();
        return *this;
    }

} // namespace model