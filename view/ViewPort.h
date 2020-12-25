//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_VIEWPORT_H
#define BLOCKS_VIEWPORT_H

#include "../global/cst.h"

#include <cstddef>

namespace view {

    class ViewPort {
      public:
        ViewPort() = default;

        double scale() const;
        int    blockSizeInScreen() const;
        int    firstColumnInView() const;
        int    firstRowInView() const;
        int    xOffset() const;
        int    yOffset() const;
        int    worldToScreenLength(int worldLength) const;
        void   setScale(int scaleParameter);
        void   translate(int dx, int dy);
        int    distanceBetweenBlocksInScreenXY() const;

      private:
        double m_scale                           = 1.0;
        int    m_xOffset                         = 0;
        int    m_yOffset                         = 0;
        int    m_distanceBetweenBlocksInScreenXY = 2 * cst::BLOCK_SHRINK_IN_WORLD;
    };
} // namespace view
#endif // BLOCKS_VIEWPORT_H
