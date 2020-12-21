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
}
view::ScreenCoordinates operator+(const view::ScreenCoordinates& lhs, const view::ScreenCoordinates& rhs);

namespace view {

    class Grid;

    class ScreenCoordinates {
      public:
        ScreenCoordinates(int x, int y);

        static ScreenCoordinates fromWorldCoordinates(const model::WorldCoordinates& worldCoordinates, const Grid& grid);
        static ScreenCoordinates fromGridCoordinates(const model::GridCoordinates& gridCoordinates, const Grid& grid);

        ScreenCoordinates& operator+=(const ScreenCoordinates& other);
        friend ScreenCoordinates(::operator+)(const ScreenCoordinates& lhs, const ScreenCoordinates& rhs);

        int x() const;
        int y() const;

      private:
        int m_x;
        int m_y;
    };

} // namespace view

#endif // BLOCKS_SCREENCOORDINATES_H
