//
// Created by pc on 21-12-20.
//

#ifndef BLOCKS_SCREENXY_H
#define BLOCKS_SCREENXY_H

#include <SDL.h>

namespace model {
    class WorldXY;
    class GridXY;
} // namespace model

namespace view {

    class ViewPort;

    class ScreenXY {
      public:
        ScreenXY(int x, int y);

        static ScreenXY fromWorldXY(const model::WorldXY& worldXY, const ViewPort& viewPort);
        static ScreenXY fromWorldXYAsVector(const model::WorldXY& worldXY, const ViewPort& viewPort);
        static ScreenXY fromGridXY(const model::GridXY& gridXY, const ViewPort& viewPort);

        int x() const;
        int y() const;

      private:
        int m_x;
        int m_y;
    };

} // namespace view

view::ScreenXY operator+(const view::ScreenXY& lhs, const view::ScreenXY& rhs);

#endif // BLOCKS_SCREENXY_H
