//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_MATRIX_H
#define BLOCKS_MATRIX_H

#include "BlockIterator.h"

#include <utility>
#include <vector>

namespace model {
    class Matrix {

        typedef std::vector<bool> Row;

      public:
        explicit Matrix(std::vector<Row>&& mat);

        void                    set(size_t row, size_t column, bool value);
        bool                    at(size_t row, size_t column) const;
        bool                    empty() const;
        size_t                  rowCount() const;
        size_t                  columnCount() const;
        IndexPair               fitToSize();
        BlockIterator           begin() const;
        Matrix                  rotateClockWise() const;
        Matrix                  rotateCounterClockWise() const;
        const std::vector<Row>& rows() const;

      private:
        std::vector<Row> m_rows;
        size_t           m_rowCount;
        size_t           m_columnCount;
    };
} // namespace model
#endif // BLOCKS_MATRIX_H
