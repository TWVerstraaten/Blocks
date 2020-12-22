//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_GRIDCOORDINATES_H
#define BLOCKS_GRIDCOORDINATES_H

#include "Enums.h"

#include <iostream>

namespace model {

    class WorldCoordinates;

    class GridCoordinates {

      public:
        GridCoordinates();

        GridCoordinates(int x, int y);

        int                    x() const;
        int                    y() const;
        size_t                 manhattanDistance(const GridCoordinates& other) const;
        GridCoordinates        adjacent(enums::DIRECTION direction) const;
        static GridCoordinates fromWorldCoordinates(const model::WorldCoordinates& worldCoordinates);

                               operator WorldCoordinates() const;
        GridCoordinates&       operator+=(const GridCoordinates& indexPair);
        friend GridCoordinates operator+(const GridCoordinates& lhs, const GridCoordinates& rhs);
        friend bool            operator<(const GridCoordinates& lhs, const GridCoordinates& rhs);
        friend bool            operator==(const GridCoordinates& lhs, const GridCoordinates& rhs);
        friend std::ostream&   operator<<(std::ostream& out, const GridCoordinates& gridCoordinates) {
            out << "GridCoordinates(" << gridCoordinates.m_x << ", " << gridCoordinates.m_y << ")";
            return out;
        }

      private:
        int m_x;
        int m_y;
    };
} // namespace model

#endif // BLOCKS_GRIDCOORDINATES_H
