//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_INDEXPAIR_H
#define BLOCKS_INDEXPAIR_H

#include <cstddef>

namespace model {

    class IndexPair {

      public:
        explicit IndexPair(int row = 0, int column = 0);
        int  row() const;
        int  column() const;
        void set(int row, int column);

        friend bool operator<(const IndexPair& lhs, const IndexPair& rhs);

      private:
        int m_row;
        int m_column;
    };
} // namespace model

#endif // BLOCKS_INDEXPAIR_H
