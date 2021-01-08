//
// Created by pc on 25-12-20.
//

#ifndef BLOCKS_WORLDLINE_H
#define BLOCKS_WORLDLINE_H

#include "WorldXY.h"

namespace model {

    class WorldLine {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        WorldLine(const WorldXY& start, const WorldXY& end);

        /****** CONST GETTERS  ******/
        [[nodiscard]] const WorldXY& start() const;
        [[nodiscard]] const WorldXY& end() const;
        [[nodiscard]] WorldXY        displacementVector() const;

        /****** FRIENDS  ******/
        friend bool operator<(const WorldLine& w1, const WorldLine& w2);

      private:
        /****** DATA MEMBERS  ******/
        WorldXY m_start;
        WorldXY m_end;
    };
} // namespace model

#endif // BLOCKS_WORLDLINE_H
