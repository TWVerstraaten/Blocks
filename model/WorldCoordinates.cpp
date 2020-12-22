//
// Created by pc on 21-12-20.
//

#include "WorldCoordinates.h"

#include "GridCoordinates.h"
#include "WorldVector.h"

namespace model {

    const WorldVector WorldCoordinates::halfBlockInWorld = {halfBlockSizeInWorld(), halfBlockSizeInWorld()};

    WorldCoordinates::WorldCoordinates(int x, int y) : m_x(x), m_y(y) {
    }

    int WorldCoordinates::x() const {
        return m_x;
    }

    int WorldCoordinates::y() const {
        return m_y;
    }

    WorldCoordinates WorldCoordinates::fromGridCoordinates(const model::GridCoordinates& gridCoordinates) {
        return {m_blockSizeInWorld * gridCoordinates.x(), m_blockSizeInWorld * gridCoordinates.y()};
    }

    constexpr int WorldCoordinates::halfBlockSizeInWorld() {
        return m_blockSizeInWorld / 2;
    }

    bool operator<(const WorldCoordinates& lhs, const WorldCoordinates& rhs) {
        return lhs.x() == rhs.x() ? lhs.y() < rhs.y() : lhs.x() < rhs.x();
    }

    bool operator==(const WorldCoordinates& lhs, const WorldCoordinates& rhs) {
        return lhs.x() == rhs.x() && lhs.y() == rhs.y();
    }

    model::WorldCoordinates operator-(const model::WorldCoordinates& lhs, const model::WorldCoordinates& rhs) {
        return {lhs.x() - rhs.x(), lhs.y() - rhs.y()};
    }

    model::WorldCoordinates operator+(const model::WorldCoordinates& lhs, const model::WorldCoordinates& rhs) {
        return {lhs.x() + rhs.x(), lhs.y() + rhs.y()};
    }

    model::WorldCoordinates operator+(const model::WorldCoordinates& lhs, const model::WorldVector& rhs) {
        return {lhs.x() + rhs.x(), lhs.y() + rhs.y()};
    }

    model::WorldCoordinates operator-(const model::WorldCoordinates& lhs, const model::WorldVector& rhs) {
        return {lhs.x() - rhs.x(), lhs.y() - rhs.y()};
    }

} // namespace model
