//
// Created by pc on 15-12-20.
//

#include "GridXy.h"

#include "../view/ScreenXy.h"
#include "../view/ViewPort.h"

#ifdef _WIN32
#include <cmath>
#endif

namespace model {
    GridXy::GridXy(int x, int y) : m_x(x), m_y(y) {
    }

    int GridXy::y() const {
        return m_y;
    }

    int GridXy::x() const {
        return m_x;
    }

    bool operator<(const GridXy& lhs, const GridXy& rhs) {
        return lhs.y() == rhs.y() ? lhs.x() < rhs.x() : lhs.y() < rhs.y();
    }

    int GridXy::manhattanDistance(const GridXy& other) const {
        return std::abs(other.y() - y()) + std::abs(other.x() - x());
    }

    GridXy GridXy::neighbor(model::GridXy::DIRECTION direction) const {
        switch (direction) {
            case model::GridXy::DIRECTION::UP:
                return {x(), y() - 1};
            case model::GridXy::DIRECTION::DOWN:
                return {x(), y() + 1};
            case model::GridXy::DIRECTION::LEFT:
                return {x() - 1, y()};
            case model::GridXy::DIRECTION::RIGHT:
                return {x() + 1, y()};
        }
        assert(false);
        return {x(), y()};
    }

    bool operator==(const GridXy& lhs, const GridXy& rhs) {
        return (lhs.m_y == rhs.m_y) && (lhs.m_x == rhs.m_x);
    }

    GridXy& GridXy::operator+=(const GridXy& gridXy) {
        m_y += gridXy.y();
        m_x += gridXy.x();
        return *this;
    }

    GridXy GridXy::fromWorldXy(const WorldXy& worldXy) {
        return {static_cast<int>(std::floor(static_cast<double>(worldXy.x()) / app::BLOCK_SIZE_IN_WORLD)),
                static_cast<int>(std::floor(static_cast<double>(worldXy.y()) / app::BLOCK_SIZE_IN_WORLD))};
    }

    GridXy operator+(const GridXy& lhs, const GridXy& rhs) {
        return {lhs.m_x + rhs.m_x, lhs.m_y + rhs.m_y};
    }

    GridXy operator-(const GridXy& lhs, const GridXy& rhs) {
        return {lhs.m_x - rhs.m_x, lhs.m_y - rhs.m_y};
    }

    GridXy::operator WorldXy() const {
        return WorldXy(*this);
    }

    bool GridXy::isAdjacent(const GridXy& other) const {
        return manhattanDistance(other) == 1;
    }

    GridXy GridXy::fromScreenXy(const view::ScreenXy& screenXy, const view::ViewPort& viewPort) {
        return GridXy(static_cast<int>(std::floor((screenXy.x() - viewPort.xOffset()) / static_cast<double>(viewPort.blockSizeInScreen()))),
                      static_cast<int>(std::floor((screenXy.y() - viewPort.yOffset()) / static_cast<double>(viewPort.blockSizeInScreen()))));
    }

    bool operator!=(const GridXy& lhs, const GridXy& rhs) {
        return not(lhs == rhs);
    }

    std::ostream& operator<<(std::ostream& out, const GridXy& gridXy) {
        out << "GridXy(" << gridXy.m_x << ", " << gridXy.m_y << ")";
        return out;
    }

} // namespace model