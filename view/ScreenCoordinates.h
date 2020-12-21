//
// Created by pc on 21-12-20.
//

#ifndef BLOCKS_SCREENCOORDINATES_H
#define BLOCKS_SCREENCOORDINATES_H

namespace model {
    class WorldCoordinates;
    class GridCoordinates;
} // namespace model

namespace view {
    class ScreenCoordinates;
    class ScreenVector;
} // namespace view
view::ScreenCoordinates operator+(const view::ScreenCoordinates& lhs, const view::ScreenCoordinates& rhs);
view::ScreenCoordinates operator+(const view::ScreenCoordinates& lhs, const view::ScreenVector& rhs);

namespace view {

    class Grid;

    class ScreenCoordinates {
      public:
        ScreenCoordinates(int x, int y);

        static ScreenCoordinates fromWorldCoordinates(const model::WorldCoordinates& worldCoordinates, const Grid& grid);
        static ScreenCoordinates fromRotatedWorldCoordinates(const model::WorldCoordinates& worldCoordinates,
                                                             const model::WorldCoordinates& pivot, double angle, const Grid& grid);
        static ScreenCoordinates fromGridCoordinates(const model::GridCoordinates& gridCoordinates, const Grid& grid);

        ScreenCoordinates& operator+=(const ScreenCoordinates& other);
        friend ScreenCoordinates(::operator+)(const ScreenCoordinates& lhs, const ScreenCoordinates& rhs);
        friend ScreenCoordinates(::operator+)(const ScreenCoordinates& lhs, const view::ScreenVector& rhs);

        int x() const;
        int y() const;

      private:
        int m_x;
        int m_y;
    };

} // namespace view

#endif // BLOCKS_SCREENCOORDINATES_H
