//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_VIEWPORT_H
#define BLOCKS_VIEWPORT_H

#include "../global/cst.h"

namespace view {

    class ViewPort {
      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        ViewPort() = default;

        /****** CONST GETTERS  ******/
        int blockSizeInScreen() const;
        int xOffset() const;
        int yOffset() const;

        /****** CONST FUNCTIONS  ******/
        int worldToScreenLength(int worldLength) const;
        int distanceBetweenBlocksInScreenXY() const;

        /****** NON CONST FUNCTIONS  ******/
        void zoom(int dZoom);
        void translate(int dx, int dy);

      private:
        /****** DATA MEMBERS  ******/
        double m_scale                           = 1.0;
        int    m_zoom                            = 0;
        int    m_xOffset                         = 0;
        int    m_yOffset                         = 0;
        int    m_distanceBetweenBlocksInScreenXY = 2 * cst::BLOCK_SHRINK_IN_WORLD;
    };
} // namespace view
#endif // BLOCKS_VIEWPORT_H
