//
// Created by pc on 15-12-20.
//

#include "ViewPort.h"

#include "../misc/defines.h"

#include <cmath>

namespace view {

    int ViewPort::blockSizeInScreen() const {
        return static_cast<int>(app::BLOCK_SIZE_IN_WORLD * m_scale);
    }

    void ViewPort::zoom(int dZoom, const view::ScreenXy& point) {
        const auto worldPointUnderMouse   = model::WorldXy::fromScreenXy(point, *this);
        m_zoom                            = std::clamp(m_zoom + dZoom, -2000, 3000);
        m_scale                           = std::exp(m_zoom / 1000.0);
        m_distanceBetweenBlocksInScreenXy = worldToScreen(2 * app::BLOCK_SHRINK_IN_WORLD);
        const auto translation            = worldPointUnderMouse - model::WorldXy::fromScreenXy(point, *this);
        translate(worldToScreen(-translation.x()), worldToScreen(-translation.y()));
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

    int ViewPort::worldToScreen(int worldLength) const {
        return static_cast<int>(m_scale * worldLength);
    }

    int ViewPort::blockSeparationInScreenXy() const {
        return m_distanceBetweenBlocksInScreenXy;
    }

    int ViewPort::screenToWorld(int worldLength) const {
        return static_cast<int>(worldLength / m_scale);
    }
} // namespace view