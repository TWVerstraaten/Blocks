//
// Created by pc on 15-12-20.
//

#include "IndexPair.h"

namespace model {

    IndexPair::IndexPair(int row, int column) : m_row(row), m_column(column) {
    }

    int IndexPair::row() const {
        return m_row;
    }
    int IndexPair::column() const {
        return m_column;
    }

    bool operator<(const IndexPair& lhs, const IndexPair& rhs) {
        return lhs.row() == rhs.row() ? lhs.column() < rhs.column() : lhs.row() < rhs.row();
    }

    void IndexPair::set(int row, int column) {
        m_row    = row;
        m_column = column;
    }
} // namespace model