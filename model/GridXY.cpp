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

    size_t GridXY::manhattanDistance(const GridXY& other) const {
        return static_cast<size_t>(std::abs(other.y() - y()) + std::abs(other.x() - x()));
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

    GridXY& GridXY::operator+=(const GridXY& gridCoordinates) {
        m_y += gridCoordinates.y();
        m_x += gridCoordinates.x();
        return *this;
    }

    GridXY GridXY::fromWorldCoordinates(const WorldXY& worldCoordinates) {
        return {static_cast<int>(std::floor(static_cast<double>(worldCoordinates.x()) / model::WorldXY::m_blockSizeInWorld)),
                static_cast<int>(std::floor(static_cast<double>(worldCoordinates.y()) / model::WorldXY::m_blockSizeInWorld))};
    }

    GridXY operator+(const GridXY& lhs, const GridXY& rhs) {
        return {lhs.m_x + rhs.m_x, lhs.m_y + rhs.m_y};
    }

    GridXY::operator WorldXY() const {
        return WorldXY::fromGridCoordinates(*this);
    }

} // namespace model