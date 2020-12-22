//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_GRIDXY_H
#define BLOCKS_GRIDXY_H

#include "Enums.h"

#include <iostream>

namespace model {

    class WorldXY;

    class GridXY {

      public:
        GridXY();

        GridXY(int x, int y);

        int           x() const;
        int           y() const;
        size_t        manhattanDistance(const GridXY& other) const;
        GridXY        adjacent(enums::DIRECTION direction) const;
        static GridXY fromWorldCoordinates(const model::WorldXY& worldCoordinates);

        explicit      operator WorldXY() const;
        GridXY&       operator+=(const GridXY& gridCoordinates);
        friend GridXY operator+(const GridXY& lhs, const GridXY& rhs);
        friend bool   operator<(const GridXY& lhs, const GridXY& rhs);
        friend bool   operator==(const GridXY& lhs, const GridXY& rhs);

      private:
        int m_x;
        int m_y;
    };
} // namespace model

#endif // BLOCKS_GRIDXY_H
