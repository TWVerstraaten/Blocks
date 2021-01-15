//
// Created by pc on 21-12-20.
//

#ifndef BLOCKS_SCREENXY_H
#define BLOCKS_SCREENXY_H

#include <QPoint>
#include <QPointF>
#include <SDL.h>
#include <ostream>

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
        ScreenXY(const QPointF& point);
        ScreenXY(const QPoint& point);

        /****** PUBLIC STATIC FUNCTIONS  ******/
        static ScreenXY fromWorldXY(const model::WorldXY& worldXY, const ViewPort& viewPort);
        static ScreenXY fromWorldXYAsVector(const model::WorldXY& worldXY, const ViewPort& viewPort);
        static ScreenXY fromGridXY(const model::GridXY& gridXY, const ViewPort& viewPort);

        /****** CONST GETTERS  ******/
        [[nodiscard]] int x() const;
        [[nodiscard]] int y() const;

        /****** OPERATORS  ******/
        operator SDL_Point() const;

        friend std::ostream& operator<<(std::ostream& out, const ScreenXY& screenXy);

      private:
        /****** DATA MEMBERS  ******/
        int m_x;
        int m_y;
    };

} // namespace view

view::ScreenXY operator+(const view::ScreenXY& lhs, const view::ScreenXY& rhs);
view::ScreenXY operator-(const view::ScreenXY& lhs, const view::ScreenXY& rhs);

#endif // BLOCKS_SCREENXY_H
