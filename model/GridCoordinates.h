//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_GRIDCOORDINATES_H
#define BLOCKS_GRIDCOORDINATES_H

#include "Enums.h"

#include <cstddef>

namespace model {

    class GridCoordinates {

      public:
        GridCoordinates() : m_x(0), m_y(0) {
        }

        GridCoordinates(int x, int y);

        int             x() const;
        int             y() const;
        size_t          manhattanDistance(const GridCoordinates& other) const;
        GridCoordinates adjacent(enums::DIRECTION direction) const;

        GridCoordinates& operator+=(const GridCoordinates& indexPair);

        friend bool operator<(const GridCoordinates& lhs, const GridCoordinates& rhs);
        friend bool operator==(const GridCoordinates& lhs, const GridCoordinates& rhs);

      private:
        int m_x;
        int m_y;
    };
} // namespace model

#endif // BLOCKS_GRIDCOORDINATES_H
