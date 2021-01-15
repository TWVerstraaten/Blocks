//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_GRIDXY_H
#define BLOCKS_GRIDXY_H

#include <iostream>

namespace view {
    class ScreenXY;
    class ViewPort;
} // namespace view

namespace model {

    class WorldXY;

    class GridXY {

      public:
        /****** PUBLIC ENUMS / TYPEDEFS  ******/
        enum class DIRECTION { UP, DOWN, LEFT, RIGHT };

        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        GridXY(const GridXY& other) = default;
        GridXY(GridXY&& other)      = default;
        GridXY(int x, int y);
        GridXY& operator=(const GridXY& rhs) = default;
        GridXY& operator=(GridXY&& rhs) = default;

        /****** PUBLIC STATIC FUNCTIONS  ******/
        static GridXY fromWorldXY(const model::WorldXY& worldXY);
        static GridXY fromScreenXY(const view::ScreenXY& screenXY, const view::ViewPort& viewPort);

        /****** CONST GETTERS  ******/
        [[nodiscard]] int x() const;
        [[nodiscard]] int y() const;

        /****** CONST FUNCTIONS  ******/
        [[nodiscard]] bool   isAdjacent(const GridXY& other) const;
        [[nodiscard]] int    manhattanDistance(const GridXY& other) const;
        [[nodiscard]] GridXY neighbor(DIRECTION direction) const;

        /****** OPERATORS  ******/
        explicit operator WorldXY() const;
        GridXY&  operator+=(const GridXY& gridXY);

        /****** FRIENDS  ******/
        friend std::ostream& operator<<(std::ostream& out, const GridXY& gridXy);
        friend GridXY operator+(const GridXY& lhs, const GridXY& rhs);
        friend GridXY operator-(const GridXY& lhs, const GridXY& rhs);
        friend bool   operator<(const GridXY& lhs, const GridXY& rhs);
        friend bool   operator==(const GridXY& lhs, const GridXY& rhs);
        friend bool   operator!=(const GridXY& lhs, const GridXY& rhs);

      private:
        /****** DATA MEMBERS  ******/
        int m_x;
        int m_y;
    };
} // namespace model

#endif // BLOCKS_GRIDXY_H
