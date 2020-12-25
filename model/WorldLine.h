//
// Created by pc on 25-12-20.
//

#ifndef BLOCKS_WORLDLINE_H
#define BLOCKS_WORLDLINE_H

#include "WorldXY.h"
namespace model {
    class WorldLine {

      public:
        WorldLine(const WorldXY& start, const WorldXY& end);

        const WorldXY& start() const;
        const WorldXY& end() const;

        friend bool operator<(const WorldLine& w1, const WorldLine& w2) {
            return w1.start() == w2.start() ? w1.end() < w2.end() : w1.start() < w2.start();
        }

      private:
        WorldXY m_start;
        WorldXY m_end;
    };
} // namespace model

#endif // BLOCKS_WORLDLINE_H
