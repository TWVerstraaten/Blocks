//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_GRIDXy_H
#define BLOCKS_GRIDXy_H

#include <iostream>

namespace view {
    class ScreenXy;
    class ViewPort;
} // namespace view

namespace model {

    class WorldXy;

    class GridXy {

      public:
        /****** PUBLIC ENUMS / TYPEDEFS  ******/
        enum class DIRECTION { UP, DOWN, LEFT, RIGHT };

        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        GridXy(int x, int y);

        /****** PUBLIC STATIC FUNCTIONS  ******/
        static GridXy fromWorldXy(const model::WorldXy& worldXy);
        static GridXy fromScreenXy(const view::ScreenXy& screenXy, const view::ViewPort& viewPort);

        /****** CONST GETTERS  ******/
        [[nodiscard]] int x() const;
        [[nodiscard]] int y() const;

        /****** CONST FUNCTIONS  ******/
        [[nodiscard]] bool   isAdjacent(const GridXy& other) const;
        [[nodiscard]] int    manhattanDistance(const GridXy& other) const;
        [[nodiscard]] GridXy neighbor(DIRECTION direction) const;

        /****** OPERATORS  ******/
        explicit operator WorldXy() const;
        GridXy&  operator+=(const GridXy& gridXy);

        /****** FRIENDS  ******/
        friend std::ostream& operator<<(std::ostream& out, const GridXy& gridXy);
        friend GridXy        operator+(const GridXy& lhs, const GridXy& rhs);
        friend GridXy        operator-(const GridXy& lhs, const GridXy& rhs);
        friend bool          operator<(const GridXy& lhs, const GridXy& rhs);
        friend bool          operator==(const GridXy& lhs, const GridXy& rhs);
        friend bool          operator!=(const GridXy& lhs, const GridXy& rhs);

      private:
        /****** DATA MEMBERS  ******/
        int m_x;
        int m_y;
    };
} // namespace model

#endif // BLOCKS_GRIDXy_H
