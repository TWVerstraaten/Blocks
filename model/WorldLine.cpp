//
// Created by teunv on 1/8/2021.
//

#include "WorldLine.h"

namespace model {

    WorldLine::WorldLine(const WorldXy& start, const WorldXy& end) : m_start(start), m_end(end) {
    }

    const WorldXy& WorldLine::start() const {
        return m_start;
    }

    const WorldXy& WorldLine::end() const {
        return m_end;
    }

    WorldXy WorldLine::displacementVector() const {
        return m_end - m_start;
    }

    bool operator<(const WorldLine& w1, const WorldLine& w2) {
        return w1.start() == w2.start() ? w1.end() < w2.end() : w1.start() < w2.start();
    }
} // namespace model
