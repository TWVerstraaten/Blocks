//
// Created by pc on 15-12-20.
//

#include "Matrix.h"

#include <algorithm>
#include <cassert>
#include <iostream>

namespace model {
    Matrix::Matrix(std::vector<Row>&& mat) : m_rows(std::move(mat)), m_rowCount(m_rows.size()) {
        assert(m_rowCount != 0);
        m_columnCount = m_rows[0].size();
        assert(m_columnCount != 0);
        for (size_t i = 0; i != m_rowCount; ++i) {
            assert(m_rows[i].size() == m_columnCount);
        }
    }

    bool Matrix::at(size_t row, size_t column) const {
        assert(row < m_rowCount);
        assert(column < m_columnCount);
        return m_rows[row][column];
    }

    size_t Matrix::rowCount() const {
        return m_rowCount;
    }

    size_t Matrix::columnCount() const {
        return m_columnCount;
    }

    BlockIterator Matrix::begin() const {
        return BlockIterator(*this);
    }

    Matrix Matrix::rotateClockWise() const {
        std::vector<Row> newRows(m_columnCount, Row(m_rowCount, false));
        Matrix           result(std::move(newRows));

        for (size_t row = 0; row != m_rowCount; ++row) {
            for (size_t column = 0; column != m_columnCount; ++column) {
                result.set(column, m_rowCount - row - 1, at(row, column));
            }
        }

        return result;
    }

    Matrix Matrix::rotateCounterClockWise() const {
        std::vector<Row> newRows(m_columnCount, Row(m_rowCount, false));
        Matrix           result(std::move(newRows));

        for (size_t row = 0; row != m_rowCount; ++row) {
            for (size_t column = 0; column != m_columnCount; ++column) {
                result.set(m_columnCount - column - 1, row, at(row, column));
            }
        }

        return result;
    }

    void Matrix::set(size_t row, size_t column, bool value) {
        m_rows[row][column] = value;
    }

    const std::vector<Matrix::Row>& Matrix::rows() const {
        return m_rows;
    }

    static bool rowIsEmpty(const std::vector<bool>& row) {
        return std::all_of(row.begin(), row.end(), [](bool b) { return !b; });
    }

    static bool columnIsEmpty(const std::vector<std::vector<bool>>& matrix, size_t column) {
        return std::all_of(matrix.begin(), matrix.end(),
                           [column](const std::vector<bool>& row) { return !row.at(column); });
    }

    IndexPair Matrix::fitToSize() {
        int removedTopRows = 0;
        while (not m_rows.empty() && rowIsEmpty(m_rows.front())) {
            ++removedTopRows;
            m_rows.erase(m_rows.begin());
        }
        while (not m_rows.empty() && rowIsEmpty(m_rows.back())) {
            m_rows.erase(m_rows.end() - 1);
        }
        m_rowCount = m_rows.size();

        int removedLeftColumns = 0;
        while (columnIsEmpty(m_rows, 0)) {
            ++removedLeftColumns;
            for (size_t i = 0; i != m_rowCount; ++i) {
                m_rows[i].erase(m_rows[i].begin());
            }
        }
        m_columnCount = m_rows.front().size();
        while (columnIsEmpty(m_rows, m_columnCount - 1)) {
            --m_columnCount;
            for (size_t i = 0; i != m_rowCount; ++i) {
                m_rows[i].erase(m_rows[i].end() - 1);
            }
        }

        return IndexPair{removedTopRows, removedLeftColumns};
    }

    bool Matrix::empty() const {
        return std::all_of(m_rows.begin(), m_rows.end(), [](const Row& row) { return rowIsEmpty(row); });
    }
} // namespace model