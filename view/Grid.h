//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_GRID_H
#define BLOCKS_GRID_H

#include <cstddef>

namespace view {

    class Grid {
      public:
        Grid() = default;

        double scale() const;
        int    pixelsPerBlock() const;
        int    xAt(double column) const;
        int    yAt(double row) const;
        int    firstColumnInView() const;
        int    firstRowInView() const;
        int    xOffset() const;
        int    yOffset() const;
        int    worldToScreenLength(int worldLength) const;
        void   setScale(int scaleParameter);
        void   translate(int dx, int dy);

      private:
        double m_scale   = 1.0;
        int    m_xOffset = 0;
        int    m_yOffset = 0;
    };
} // namespace view
#endif // BLOCKS_GRID_H
