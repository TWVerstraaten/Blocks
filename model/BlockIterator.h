//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_BLOCKITERATOR_H
#define BLOCKS_BLOCKITERATOR_H

#include "IndexPair.h"

#include <tuple>

namespace model {
    class Matrix;

    class BlockIterator {
      public:
        explicit BlockIterator(const Matrix& matrix);

        const IndexPair& get() const;
        bool             isDone() const;
        void             next();
        void             forceDone();

      private:
        bool          m_done = false;
        IndexPair     m_indexPair;
        const Matrix& m_matrix;
    };
} // namespace model
#endif // BLOCKS_BLOCKITERATOR_H
