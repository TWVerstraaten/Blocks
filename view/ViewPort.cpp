//
// Created by pc on 15-12-20.
//

#include "ViewPort.h"

#include "../global/fns.h"

#include <cmath>

namespace view {

    int ViewPort::blockSizeInScreen() const {
        return static_cast<int>(cst::BLOCK_SIZE_IN_WORLD * m_scale);
    }

    void ViewPort::zoom(int dZoom) {
        m_zoom                            = fns::clamp(m_zoom + dZoom, -10, 20);
        m_scale                           = std::exp(m_zoom / 10.0);
        m_distanceBetweenBlocksInScreenXY = worldToScreenLength(2 * cst::BLOCK_SHRINK_IN_WORLD);
    }

    void ViewPort::translate(int dx, int dy) {
        m_xOffset += dx;
        m_yOffset += dy;
    }

    int ViewPort::xOffset() const {
        return m_xOffset;
    }

    int ViewPort::yOffset() const {
        return m_yOffset;
    }

    int ViewPort::worldToScreenLength(int worldLength) const {
        return static_cast<int>(m_scale * worldLength);
    }

    int ViewPort::blockSeparationInScreenXY() const {
        return m_distanceBetweenBlocksInScreenXY;
    }
} // namespace view