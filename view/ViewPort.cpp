//
// Created by pc on 15-12-20.
//

#include "ViewPort.h"

#include "../model/WorldXY.h"

#include <cmath>

namespace view {

    int ViewPort::blockSizeInScreen() const {
        return static_cast<int>(model::WorldXY::m_blockSizeInWorld * m_scale);
    }

    int ViewPort::firstColumnInView() const {
        return -m_xOffset / blockSizeInScreen();
    }

    int ViewPort::firstRowInView() const {
        return -m_yOffset / blockSizeInScreen();
    }

    void ViewPort::setScale(int scaleParameter) {
        m_scale = std::exp(scaleParameter / 10.0);
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

    double ViewPort::scale() const {
        return m_scale;
    }

    int ViewPort::worldToScreenLength(int worldLength) const {
        return static_cast<int>(m_scale * worldLength);
    }
} // namespace view