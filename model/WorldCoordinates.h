//
// Created by pc on 21-12-20.
//

#ifndef BLOCKS_WORLDCOORDINATES_H
#define BLOCKS_WORLDCOORDINATES_H

#include "GridCoordinates.h"
#include "WorldVector.h"

#include <cstddef>

namespace view {
    class Grid;
}

namespace model {
    class WorldVector;

    class WorldCoordinates {
      public:
        WorldCoordinates(int x, int y);

        int x() const;
        int y() const;

        static const int         m_blockSizeInWorld = 100;
        static constexpr int     halfBlockSizeInWorld();
        static const WorldVector halfBlockInWorld;

        static WorldCoordinates fromGridCoordinates(const model::GridCoordinates& gridCoordinates);

        friend bool operator<(const WorldCoordinates& lhs, const WorldCoordinates& rhs);
        friend bool operator==(const WorldCoordinates& lhs, const WorldCoordinates& rhs);

        friend WorldCoordinates operator-(const WorldCoordinates& lhs, const WorldCoordinates& rhs);
        friend WorldCoordinates operator+(const WorldCoordinates& lhs, const WorldCoordinates& rhs);
        friend WorldCoordinates operator+(const WorldCoordinates& lhs, const WorldVector& rhs);
        friend WorldCoordinates operator-(const WorldCoordinates& lhs, const WorldVector& rhs);
        friend std::ostream&    operator<<(std::ostream& out, const WorldCoordinates& worldCoordinates) {
            out << "GridCoordinates(" << worldCoordinates.m_x << ", " << worldCoordinates.m_y << ")";
            return out;
        }

      private:
        int m_x;
        int m_y;
    };
} // namespace model

#endif // BLOCKS_WORLDCOORDINATES_H
