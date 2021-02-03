//
// Created by pc on 21-12-20.
//

#ifndef BLOCKS_SCREENXY_H
#define BLOCKS_SCREENXY_H

#include <QPoint>
#include <QPointF>
#include <ostream>

namespace model {
    class WorldXy;
    class GridXy;
} // namespace model

namespace view {

    class ViewPort;

    class ScreenXy : public QPoint {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        ScreenXy();
        ScreenXy(int x, int y);
        explicit ScreenXy(const QPointF& point);
        explicit ScreenXy(const QPoint& point);

        /****** PUBLIC STATIC FUNCTIONS  ******/
        static ScreenXy fromWorldXy(const model::WorldXy& worldXy, const ViewPort& viewPort);
        static ScreenXy fromWorldXyAsVector(const model::WorldXy& worldXy, const ViewPort& viewPort);
        static ScreenXy fromGridXy(const model::GridXy& gridXy, const ViewPort& viewPort);

        friend std::ostream& operator<<(std::ostream& out, const ScreenXy& screenXy);
    };

} // namespace view

view::ScreenXy operator+(const view::ScreenXy& lhs, const view::ScreenXy& rhs);
view::ScreenXy operator-(const view::ScreenXy& lhs, const view::ScreenXy& rhs);

#endif // BLOCKS_SCREENXY_H
