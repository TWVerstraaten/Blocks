//
// Created by pc on 21-12-20.
//

#ifndef BLOCKS_WORLDXY_H
#define BLOCKS_WORLDXY_H

#include "WorldVector.h"

#include <cstddef>

namespace view {
    class ViewPort;
}

namespace model {
    class GridXY;

    class WorldXY {
      public:
        WorldXY(int x, int y);

        int x() const;
        int y() const;

        static const int         m_blockSizeInWorld = 50;
        static constexpr int     halfBlockSizeInWorld();
        static const WorldVector halfBlockInWorld;

        static WorldXY fromGridXY(const model::GridXY& gridXY);

        friend bool operator<(const WorldXY& lhs, const WorldXY& rhs);
        friend bool operator==(const WorldXY& lhs, const WorldXY& rhs);

        friend WorldXY operator-(const WorldXY& lhs, const WorldXY& rhs);
        friend WorldXY operator+(const WorldXY& lhs, const WorldXY& rhs);
        friend WorldXY operator+(const WorldXY& lhs, const WorldVector& rhs);
        friend WorldXY operator-(const WorldXY& lhs, const WorldVector& rhs);

      private:
        int m_x;
        int m_y;
    };
} // namespace model

#endif // BLOCKS_WORLDXY_H
