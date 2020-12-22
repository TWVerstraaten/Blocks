//
// Created by pc on 21-12-20.
//

#ifndef BLOCKS_SCREENXY_H
#define BLOCKS_SCREENXY_H

#include <SDL2/SDL.h>

namespace model {
    class WorldXY;
    class GridXY;
} // namespace model

namespace view {

    class Grid;
    class ScreenVector;

    class ScreenXY {
      public:
        ScreenXY(int x, int y);

        static ScreenXY fromWorldCoordinates(const model::WorldXY& worldCoordinates, const Grid& grid);
        static ScreenXY fromGridCoordinates(const model::GridXY& gridCoordinates, const Grid& grid);

        int x() const;
        int y() const;

      private:
        int m_x;
        int m_y;
    };

} // namespace view

view::ScreenXY operator+(const view::ScreenXY& lhs, const view::ScreenXY& rhs);
view::ScreenXY operator+(const view::ScreenXY& lhs, const view::ScreenVector& rhs);

#endif // BLOCKS_SCREENXY_H
