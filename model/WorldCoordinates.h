//
// Created by pc on 21-12-20.
//

#ifndef BLOCKS_WORLDCOORDINATES_H
#define BLOCKS_WORLDCOORDINATES_H

#include "GridCoordinates.h"

#include <cstddef>

namespace view {
    class Grid;
}

namespace model {

    class WorldCoordinates {
      public:
        WorldCoordinates(int x, int y);

        int x() const;
        int y() const;

        static const int m_blockSizeInWorld = 100;

        static model::WorldCoordinates fromGridCoordinates(const model::GridCoordinates& gridCoordinates);

      private:
        int m_x;
        int m_y;
    };
} // namespace model

#endif // BLOCKS_WORLDCOORDINATES_H
