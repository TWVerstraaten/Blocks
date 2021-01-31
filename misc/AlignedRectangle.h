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
        AlignedRectangle(int xMin, int xMax, int yMin, int yMax);

        static AlignedRectangle boundingAlignedRectangle(const model::Cluster& cluster);

        [[nodiscard]] bool             intersects(const AlignedRectangle& other) const;
        [[nodiscard]] int              maxDimension() const;
        [[nodiscard]] model::WorldLine sideAt(size_t index) const;

      private:
        [[nodiscard]] int width() const;
        [[nodiscard]] int height() const;

        int m_xMin;
        int m_xMax;
        int m_yMin;
        int m_yMax;
        int m_maxDimension;
    };

} // namespace geom

#endif // BLOCKS_ALIGNEDRECTANGLE_H
