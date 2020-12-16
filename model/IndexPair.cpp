//
// Created by pc on 15-12-20.
//

#include "IndexPair.h"

#include <cassert>
#include <cmath>

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

    size_t IndexPair::manhattanDistance(const IndexPair& other) const {
        return std::abs(other.row() - row()) + std::abs(other.column() - column());
    }

    IndexPair IndexPair::adjacent(enums::DIRECTION direction) const {
        switch (direction) {

            case enums::DIRECTION::UP:
                return IndexPair(row() - 1, column());
            case enums::DIRECTION::DOWN:
                return IndexPair(row() + 1, column());
            case enums::DIRECTION::LEFT:
                return IndexPair(row(), column() - 1);
            case enums::DIRECTION::RIGHT:
                return IndexPair(row(), column() + 1);
            default:
                assert(false);
        }
    }
} // namespace model