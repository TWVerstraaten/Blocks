//
// Created by pc on 21-12-20.
//

#ifndef BLOCKS_SCREENCOORDINATES_H
#define BLOCKS_SCREENCOORDINATES_H

#include "ScreenVector.h"

#include <SDL2/SDL.h>

namespace model {
    class WorldCoordinates;
    class GridCoordinates;
} // namespace model

namespace view {

    class Grid;

    class ScreenCoordinates {
      public:
        ScreenCoordinates(int x, int y);

        static ScreenCoordinates fromWorldCoordinates(const model::WorldCoordinates& worldCoordinates, const Grid& grid);
        static ScreenCoordinates fromGridCoordinates(const model::GridCoordinates& gridCoordinates, const Grid& grid);

        int x() const;
        int y() const;

      private:
        int m_x;
        int m_y;
    };

} // namespace view

view::ScreenCoordinates operator+(const view::ScreenCoordinates& lhs, const view::ScreenCoordinates& rhs);
view::ScreenCoordinates operator+(const view::ScreenCoordinates& lhs, const view::ScreenVector& rhs);

#endif // BLOCKS_SCREENCOORDINATES_H
