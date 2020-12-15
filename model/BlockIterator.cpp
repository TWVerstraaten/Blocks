//
// Created by pc on 15-12-20.
//

#include "BlockIterator.h"

#include "Matrix.h"

model::BlockIterator::BlockIterator(const model::Matrix& matrix) : m_matrix(matrix) {
    if (not m_matrix.at(0, 0)) {
        next();
    }
}

bool model::BlockIterator::isDone() const {
    return m_done;
}

const IndexPair& model::BlockIterator::get() const {
    return m_indexPair;
}

void model::BlockIterator::next() {
    if (m_done) {
        return;
    }
    for (int j = m_indexPair.column() + 1; j != static_cast<int>(m_matrix.columnCount()); ++j) {
        if (m_matrix.at(m_indexPair.row(), j)) {
            m_indexPair = IndexPair{m_indexPair.row(), j};
            return;
        }
    }

    for (int i = m_indexPair.row() + 1; i != static_cast<int>(m_matrix.rowCount()); ++i) {
        for (int j = 0; j != static_cast<int>(m_matrix.columnCount()); ++j) {
            if (m_matrix.at(i, j)) {
                m_indexPair = IndexPair{i, j};
                return;
            }
        }
    }
    m_done = true;
}

void model::BlockIterator::forceDone() {
    m_done = true;
}
