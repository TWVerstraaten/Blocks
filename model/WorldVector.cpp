//
// Created by pc on 21-12-20.
//

#include "WorldVector.h"

namespace model {

    WorldVector::WorldVector(int x, int y) : m_x(x), m_y(y) {
    }

    int WorldVector::x() const {
        return m_x;
    }

    int WorldVector::y() const {
        return m_y;
    }
} // namespace model
