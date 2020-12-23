//
// Created by pc on 21-12-20.
//

#ifndef BLOCKS_SCREENVECTOR_H
#define BLOCKS_SCREENVECTOR_H

namespace model {
    class WorldVector;
}

namespace view {

    class ViewPort;

    class ScreenVector {
      public:
        ScreenVector(int x, int y);

        static ScreenVector fromWorldVector(const model::WorldVector& worldVector, const ViewPort& viewPort);

        int x() const;
        int y() const;

      private:
        int m_x;
        int m_y;
    };

} // namespace view

#endif // BLOCKS_SCREENVECTOR_H
