//
// Created by pc on 22-01-21.
//

#include "AlignedRectangle.h"

#include "../app/Application_constants.h"
#include "../model/Cluster.h"
#include "defines.h"
#include "geom.h"

#include <cassert>

namespace geom {

    AlignedRectangle::AlignedRectangle(int mXMin, int mXMax, int mYMin, int mYMax) : m_xMin(mXMin), m_xMax(mXMax), m_yMin(mYMin), m_yMax(mYMax) {
        assert(m_xMin <= m_xMax);
        assert(m_yMin <= m_yMax);
    }

    AlignedRectangle AlignedRectangle::boundingAlignedRectangle(const model::Cluster& cluster) {
        const auto& gridXySet = cluster.gridXyVector();
        int         minX      = app::BLOCK_SIZE_IN_WORLD * geom::minX(gridXySet);
        int         maxX      = app::BLOCK_SIZE_IN_WORLD * (geom::maxX(gridXySet) + 1);
        int         minY      = app::BLOCK_SIZE_IN_WORLD * geom::minY(gridXySet);
        int         maxY      = app::BLOCK_SIZE_IN_WORLD * (geom::maxY(gridXySet) + 1);

        switch (cluster.phase()) {
            case model::PHASE::NONE:
                return {minX, maxX, minY, maxY};
            case model::PHASE::TRANSLATING: {
                const auto& worldOffset = cluster.dynamicWorldOffset();
                return {minX + worldOffset.x(), maxX + worldOffset.x(), minY + worldOffset.y(), maxY + worldOffset.y()};
            }
            case model::PHASE::ROTATING:
                const auto&          f  = cluster.phaseTransformation();
                const model::WorldXy p1 = f(model::WorldXy{minX, minY});
                const model::WorldXy p2 = f(model::WorldXy{maxX, minY});
                const model::WorldXy p3 = f(model::WorldXy{minX, maxY});
                const model::WorldXy p4 = f(model::WorldXy{maxX, maxY});

                return {std::min(p1.x(), std::min(p2.x(), std::min(p3.x(), p4.x()))),
                        std::max(p1.x(), std::max(p2.x(), std::max(p3.x(), p4.x()))),
                        std::min(p1.y(), std::min(p2.y(), std::min(p3.y(), p4.y()))),
                        std::max(p1.y(), std::max(p2.y(), std::max(p3.y(), p4.y())))};
        }

        return AlignedRectangle(0, 0, 0, 0);
    }

    model::WorldLine AlignedRectangle::line(size_t index) const {
        assert(index < 4);

        switch (index) {
            case 0:
                return model::WorldLine{model::WorldXy{m_xMin, m_yMin}, model::WorldXy{m_xMin, m_yMax}};
            case 1:
                return model::WorldLine{model::WorldXy{m_xMin, m_yMin}, model::WorldXy{m_xMax, m_yMin}};
            case 2:
                return model::WorldLine{model::WorldXy{m_xMax, m_yMin}, model::WorldXy{m_xMax, m_yMax}};
            case 3:
                return model::WorldLine{model::WorldXy{m_xMin, m_yMax}, model::WorldXy{m_xMax, m_yMax}};
            default:
                assert(false);
                return model::WorldLine{model::WorldXy{0, 0}, model::WorldXy{0, 0}};
        }
        assert(false);
        return model::WorldLine{model::WorldXy{0, 0}, model::WorldXy{0, 0}};
    }

    bool AlignedRectangle::intersects(const AlignedRectangle& other) const {
        return (other.m_xMin < m_xMax) && (m_xMin < other.m_xMax) && (other.m_yMin < m_yMax) && (m_yMin < other.m_yMax);
    }

    int AlignedRectangle::width() const {
        return m_xMax - m_xMin;
    }

    int AlignedRectangle::height() const {
        return m_yMax - m_yMin;
    }

    int AlignedRectangle::maxDimension() const {
        return std::max(width(), height());
    }

} // namespace geom
