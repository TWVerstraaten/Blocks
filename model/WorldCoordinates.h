//
// Created by pc on 21-12-20.
//

#ifndef BLOCKS_WORLDCOORDINATES_H
#define BLOCKS_WORLDCOORDINATES_H

namespace model {

    class WorldCoordinates {
      public:
        WorldCoordinates(int x, int y);

        int x() const;
        int y() const;

      private:
        int m_x;
        int m_y;
    };
} // namespace model

#endif // BLOCKS_WORLDCOORDINATES_H
