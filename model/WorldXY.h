//
// Created by pc on 21-12-20.
//

#ifndef BLOCKS_WORLDXY_H
#define BLOCKS_WORLDXY_H

#include "WorldXY.h"

#include <cstddef>

namespace view {
    class ViewPort;
}

namespace model {
    class GridXY;

    class WorldXY {
      public:
        WorldXY(int x, int y) noexcept;
        WorldXY(const model::GridXY& gridXY);

        int x() const;
        int y() const;

        friend bool operator<(const WorldXY& lhs, const WorldXY& rhs);
        friend bool operator==(const WorldXY& lhs, const WorldXY& rhs);

        friend WorldXY operator-(const WorldXY& lhs, const WorldXY& rhs);
        friend WorldXY operator+(const WorldXY& lhs, const WorldXY& rhs);

      private:
        friend class GridXY;
        static WorldXY fromGridXY(const model::GridXY& gridXY);

        int m_x = 0;
        int m_y = 0;
    };
} // namespace model

#endif // BLOCKS_WORLDXY_H
