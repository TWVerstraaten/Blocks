//
// Created by pc on 25-12-20.
//

#ifndef BLOCKS_WORLDLINE_H
#define BLOCKS_WORLDLINE_H

#include "WorldXy.h"

namespace model {

    class WorldLine {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        WorldLine(const WorldXy& start, const WorldXy& end);

        /****** CONST GETTERS  ******/
        [[nodiscard]] const WorldXy& start() const;
        [[nodiscard]] const WorldXy& end() const;
        [[nodiscard]] WorldXy        displacementVector() const;

        /****** FRIENDS  ******/
        friend bool operator<(const WorldLine& w1, const WorldLine& w2);

      private:
        /****** DATA MEMBERS  ******/
        WorldXy m_start;
        WorldXy m_end;
    };
} // namespace model

#endif // BLOCKS_WORLDLINE_H
