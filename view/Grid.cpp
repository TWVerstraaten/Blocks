//
// Created by pc on 15-12-20.
//

#include "Grid.h"

#include <cmath>

namespace view {

    size_t Grid::blockSize() const {
        return m_blockSize;
    }

    int Grid::xAt(double column) const {
        return m_xOffset + column * m_blockSize;
    }

    int Grid::yAt(double row) const {
        return m_yOffset + row * m_blockSize;
    }

    int Grid::firstColumnInView() const {
        return -m_xOffset / static_cast<int>(m_blockSize);
    }

    int Grid::firstRowInView() const {
        return -m_yOffset / static_cast<int>(m_blockSize);
    }

    void Grid::update(size_t width, size_t height) {
    }

    void Grid::setBlockSize(int scaleParameter) {
        m_blockSize = 100 * std::exp(scaleParameter / 10.0);
    }
} // namespace view