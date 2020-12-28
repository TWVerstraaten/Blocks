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

        /****** PRIVATE ENUMS / TYPEDEFS  ******/
        friend class GridXY;

      public:
        /****** PUBLIC ENUMS / TYPEDEFS  ******/
        WorldXY(int x, int y) noexcept;
        WorldXY(const model::GridXY& gridXY);

        /****** CONST GETTERS  ******/
        int x() const;
        int y() const;

        /****** OPERATORS  ******/
        WorldXY& operator+=(const WorldXY& other);
        WorldXY& operator/=(int divisor);

        /****** FRIENDS  ******/
        friend bool    operator<(const WorldXY& lhs, const WorldXY& rhs);
        friend bool    operator==(const WorldXY& lhs, const WorldXY& rhs);
        friend WorldXY operator-(const WorldXY& lhs, const WorldXY& rhs);
        friend WorldXY operator+(const WorldXY& lhs, const WorldXY& rhs);

      private:
        /****** PRIVATE STATIC FUNCTIONS  ******/
        static WorldXY fromGridXY(const model::GridXY& gridXY);

        /****** DATA MEMBERS  ******/
        int m_x = 0;
        int m_y = 0;
    };
} // namespace model

#endif // BLOCKS_WORLDXY_H
