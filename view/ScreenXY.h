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
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        ScreenXY();
        ScreenXY(int x, int y);
        ScreenXY(const SDL_Point& point);

        /****** PUBLIC STATIC FUNCTIONS  ******/
        static ScreenXY fromWorldXY(const model::WorldXY& worldXY, const ViewPort& viewPort);
        static ScreenXY fromWorldXYAsVector(const model::WorldXY& worldXY, const ViewPort& viewPort);
        static ScreenXY fromGridXY(const model::GridXY& gridXY, const ViewPort& viewPort);

        /****** CONST GETTERS  ******/
        int x() const;
        int y() const;

        /****** OPERATORS  ******/
        operator SDL_Point() const {
            return {m_x, m_y};
        }

      private:
        /****** DATA MEMBERS  ******/
        int m_x;
        int m_y;
    };

} // namespace view

view::ScreenXY operator+(const view::ScreenXY& lhs, const view::ScreenXY& rhs);

#endif // BLOCKS_SCREENXY_H
