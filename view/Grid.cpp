//
// Created by pc on 15-12-20.
//

#include "Grid.h"

#include <algorithm>
#include <cmath>

namespace view {

    int Grid::pixelsPerBlock() const {
        return 100 * m_scale;
    }

    int Grid::xAt(double column) const {
        return m_xOffset + column * pixelsPerBlock();
    }

    int Grid::yAt(double row) const {
        return m_yOffset + row * pixelsPerBlock();
    }

    int Grid::firstColumnInView() const {
        return -m_xOffset / pixelsPerBlock();
    }

    int Grid::firstRowInView() const {
        return -m_yOffset / pixelsPerBlock();
    }

    void Grid::setScale(int scaleParameter) {
        m_scale = std::exp(scaleParameter / 10.0);
    }

    void Grid::translate(int dx, int dy) {
        m_xOffset += dx;
        m_yOffset += dy;
    }
    int Grid::xOffset() const {
        return m_xOffset;
    }
    int Grid::yOffset() const {
        return m_yOffset;
    }

    double Grid::scale() const {
        return m_scale;
    }

    int Grid::worldToScreenLength(int worldLength) const {
        return m_scale * worldLength;
    }
} // namespace view