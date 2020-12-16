//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_INDEXPAIR_H
#define BLOCKS_INDEXPAIR_H

#include "Enums.h"

#include <cstddef>

namespace model {

    class IndexPair {

      public:
        IndexPair(int row = 0, int column = 0);

        void      set(int row, int column);
        int       row() const;
        int       column() const;
        size_t    manhattanDistance(const IndexPair& other) const;
        IndexPair adjacent(enums::DIRECTION direction) const;

        friend bool operator<(const IndexPair& lhs, const IndexPair& rhs);

      private:
        int m_row;
        int m_column;
    };
} // namespace model

#endif // BLOCKS_INDEXPAIR_H
