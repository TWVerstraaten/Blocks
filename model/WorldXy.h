//
// Created by pc on 21-12-20.
//

#ifndef BLOCKS_WORLDXY_H
#define BLOCKS_WORLDXY_H

#include <cstddef>

namespace view {
    class ViewPort;
    class ScreenXy;
} // namespace view

namespace model {

    class GridXy;

    class WorldXy {

        /****** PRIVATE ENUMS / TYPEDEFS  ******/
        friend class GridXy;

      public:
        /****** PUBLIC ENUMS / TYPEDEFS  ******/
        WorldXy(int x, int y) noexcept;
        explicit WorldXy(const model::GridXy& gridXy);

        /****** CONST GETTERS  ******/
        [[nodiscard]] int x() const;
        [[nodiscard]] int y() const;

        /****** OPERATORS  ******/
        WorldXy& operator+=(const WorldXy& other);
        WorldXy& operator/=(int divisor);

        static WorldXy fromScreenXy(const view::ScreenXy& screenXy, const view::ViewPort& viewPort);

        /****** FRIENDS  ******/
        friend bool    operator<(const WorldXy& lhs, const WorldXy& rhs);
        friend bool    operator==(const WorldXy& lhs, const WorldXy& rhs);
        friend WorldXy operator-(const WorldXy& lhs, const WorldXy& rhs);
        friend WorldXy operator+(const WorldXy& lhs, const WorldXy& rhs);
        friend WorldXy operator/(const WorldXy& lhs, double a);

      private:
        /****** PRIVATE STATIC FUNCTIONS  ******/
        static WorldXy fromGridXy(const model::GridXy& gridXy);

        /****** DATA MEMBERS  ******/
        int m_x = 0;
        int m_y = 0;
    };
} // namespace model

#endif // BLOCKS_WORLDXY_H
