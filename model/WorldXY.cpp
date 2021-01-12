//
// Created by pc on 21-12-20.
//

#include "WorldXY.h"

#include "../app/Application_constants.h"
#include "GridXY.h"

namespace model {

    WorldXY::WorldXY(int x, int y) noexcept : m_x(x), m_y(y) {
    }

    int WorldXY::x() const {
        return m_x;
    }

    int WorldXY::y() const {
        return m_y;
    }

    WorldXY WorldXY::fromGridXY(const model::GridXY& gridXY) {
        return {app::BLOCK_SIZE_IN_WORLD * gridXY.x(), app::BLOCK_SIZE_IN_WORLD * gridXY.y()};
    }

    bool operator<(const WorldXY& lhs, const WorldXY& rhs) {
        return lhs.y() == rhs.y() ? lhs.x() < rhs.x() : lhs.y() < rhs.y();
    }

    bool operator==(const WorldXY& lhs, const WorldXY& rhs) {
        return lhs.x() == rhs.x() && lhs.y() == rhs.y();
    }

    model::WorldXY operator-(const model::WorldXY& lhs, const model::WorldXY& rhs) {
        return {lhs.x() - rhs.x(), lhs.y() - rhs.y()};
    }

    model::WorldXY operator+(const model::WorldXY& lhs, const model::WorldXY& rhs) {
        return {lhs.x() + rhs.x(), lhs.y() + rhs.y()};
    }

    model::WorldXY operator/(const model::WorldXY& lhs, double a) {
        return {static_cast<int>(lhs.x() / a), static_cast<int>(lhs.y() / a)};
    }

    WorldXY::WorldXY(const GridXY& gridXY) {
        *this = fromGridXY(gridXY);
    }

    WorldXY& WorldXY::operator+=(const WorldXY& other) {
        m_x += other.m_x;
        m_y += other.m_y;
        return *this;
    }

    WorldXY& WorldXY::operator/=(int divisor) {
        m_x /= divisor;
        m_y /= divisor;
        return *this;
    }

    WorldXY::operator SDL_Point() const {
        return {m_x, m_y};
    }

} // namespace model
