//
// Created by pc on 25-12-20.
//

#ifndef BLOCKS_LINE_H
#define BLOCKS_LINE_H

#include "WorldXY.h"

namespace model {

    template <typename XY>
    class Line {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        Line(const XY& start, const XY& end) : m_start(start), m_end(end) {
        }

        /****** CONST GETTERS  ******/
        const XY& start() const {
            return m_start;
        }
        const XY& end() const {
            return m_end;
        }
        XY displacementVector() const {
            return m_end - m_start;
        }

        /****** FRIENDS  ******/
        friend bool operator<(const Line<XY>& w1, const Line<XY>& w2) {
            return w1.start() == w2.start() ? w1.end() < w2.end() : w1.start() < w2.start();
        }

      private:
        /****** DATA MEMBERS  ******/
        XY m_start;
        XY m_end;
    };
} // namespace model

#endif // BLOCKS_LINE_H
