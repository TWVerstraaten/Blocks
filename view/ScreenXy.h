//
// Created by pc on 21-12-20.
//

#ifndef BLOCKS_SCREENXy_H
#define BLOCKS_SCREENXy_H

#include <QPoint>
#include <QPointF>
#include <ostream>

namespace model {
    class WorldXy;
    class GridXy;
} // namespace model

namespace view {

    class ViewPort;

    class ScreenXy {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        ScreenXy();
        ScreenXy(int x, int y);
        ScreenXy(const QPointF& point);
        ScreenXy(const QPoint& point);

        /****** PUBLIC STATIC FUNCTIONS  ******/
        static ScreenXy fromWorldXy(const model::WorldXy& worldXy, const ViewPort& viewPort);
        static ScreenXy fromWorldXyAsVector(const model::WorldXy& worldXy, const ViewPort& viewPort);
        static ScreenXy fromGridXy(const model::GridXy& gridXy, const ViewPort& viewPort);

        /****** CONST GETTERS  ******/
        [[nodiscard]] int x() const;
        [[nodiscard]] int y() const;

        friend std::ostream& operator<<(std::ostream& out, const ScreenXy& screenXy);

      private:
        /****** DATA MEMBERS  ******/
        int m_x;
        int m_y;
    };

} // namespace view

view::ScreenXy operator+(const view::ScreenXy& lhs, const view::ScreenXy& rhs);
view::ScreenXy operator-(const view::ScreenXy& lhs, const view::ScreenXy& rhs);

#endif // BLOCKS_SCREENXy_H
