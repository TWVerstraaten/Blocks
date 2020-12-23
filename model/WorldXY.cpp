//
// Created by pc on 21-12-20.
//

#include "WorldXY.h"

#include "GridXY.h"

namespace model {

    const WorldXY WorldXY::halfBlockInWorld = {halfBlockSizeInWorld(), halfBlockSizeInWorld()};

    WorldXY::WorldXY(int x, int y) : m_x(x), m_y(y) {
    }

    int WorldXY::x() const {
        return m_x;
    }

    int WorldXY::y() const {
        return m_y;
    }

    WorldXY WorldXY::fromGridXY(const model::GridXY& gridXY) {
        return {m_blockSizeInWorld * gridXY.x(), m_blockSizeInWorld * gridXY.y()};
    }

    constexpr int WorldXY::halfBlockSizeInWorld() {
        return m_blockSizeInWorld / 2;
    }

    bool operator<(const WorldXY& lhs, const WorldXY& rhs) {
        return lhs.x() == rhs.x() ? lhs.y() < rhs.y() : lhs.x() < rhs.x();
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

} // namespace model
