//
// Created by pc on 21-12-20.
//

#ifndef BLOCKS_WORLDVECTOR_H
#define BLOCKS_WORLDVECTOR_H

namespace model {

    class WorldVector {
      public:
        WorldVector(int x, int y);

        int x() const;
        int y() const;

      private:
        int m_x;
        int m_y;
    };
} // namespace model
#endif // BLOCKS_WORLDVECTOR_H
