//
// Created by pc on 25-12-20.
//

#include "WorldLine.h"

model::WorldLine::WorldLine(const model::WorldXY& start, const model::WorldXY& end) : m_start(start), m_end(end) {
}

const model::WorldXY& model::WorldLine::start() const {
    return m_start;
}

const model::WorldXY& model::WorldLine::end() const {
    return m_end;
}
