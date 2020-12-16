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

        void setBlockSize(int scaleParameter);
        void update(size_t width, size_t height);

        size_t blockSize() const;
        int    xAt(double column) const;
        int    yAt(double row) const;
        int    firstColumnInView() const;
        int    firstRowInView() const;

        void translate(int dx, int dy);

      private:
        size_t m_blockSize = 100;
        int    m_xOffset   = 0;
        int    m_yOffset   = 0;
    };
} // namespace view
#endif // BLOCKS_GRID_H
