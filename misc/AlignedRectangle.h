//
// Created by pc on 22-01-21.
//

#ifndef BLOCKS_ALIGNEDRECTANGLE_H
#define BLOCKS_ALIGNEDRECTANGLE_H

#include "../model/WorldLine.h"

namespace model {
    class Cluster;
}

namespace geom {

    class AlignedRectangle {

      public:
        AlignedRectangle() = default;
        AlignedRectangle(int mXMin, int mXMax, int mYMin, int mYMax);

        static AlignedRectangle boundingAlignedRectangle(const model::Cluster& cluster);

        [[nodiscard]] bool             intersects(const AlignedRectangle& other) const;
        [[nodiscard]] model::WorldLine line(size_t index) const;
        [[nodiscard]] int              width() const;
        [[nodiscard]] int              height() const;
        [[nodiscard]] int              maxDimension() const;

      private:
        int m_xMin;
        int m_xMax;
        int m_yMin;
        int m_yMax;
        int m_maxDimension;
    };

} // namespace geom

#endif // BLOCKS_ALIGNEDRECTANGLE_H
