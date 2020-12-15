//
// Created by pc on 15-12-20.
//

#include "IndexPair.h"

IndexPair::IndexPair(int row, int column) : m_row(row), m_column(column) {
}

int IndexPair::row() const {
    return m_row;
}
int IndexPair::column() const {
    return m_column;
}
