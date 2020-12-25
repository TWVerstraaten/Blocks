//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_GRIDXY_H
#define BLOCKS_GRIDXY_H

#include "Enums.h"

#include <iostream>

namespace view {
    class ScreenXY;
    class ViewPort;
} // namespace view

namespace model {

    class WorldXY;

    class GridXY {

      public:
        GridXY(const GridXY& other) = default;
        GridXY(GridXY&& other)      = default;
        GridXY(int x, int y);

        GridXY& operator=(const GridXY& rhs) = default;
        GridXY& operator=(GridXY&& rhs) = default;

        int           x() const;
        int           y() const;
        int           manhattanDistance(const GridXY& other) const;
        GridXY        neighbor(enums::DIRECTION direction) const;
        static GridXY fromWorldXY(const model::WorldXY& worldXY);
        static GridXY fromScreenXY(const view::ScreenXY& screenXY, const view::ViewPort& viewPort);
        bool          isAdjacent(const GridXY& other) const;

        explicit      operator WorldXY() const;
        GridXY&       operator+=(const GridXY& gridXY);
        friend GridXY operator+(const GridXY& lhs, const GridXY& rhs);
        friend bool   operator<(const GridXY& lhs, const GridXY& rhs);
        friend bool   operator==(const GridXY& lhs, const GridXY& rhs);
        friend bool   operator!=(const GridXY& lhs, const GridXY& rhs);

      private:
        int m_x;
        int m_y;
    };
} // namespace model

#endif // BLOCKS_GRIDXY_H
