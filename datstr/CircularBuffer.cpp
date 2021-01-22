//
// Created by teunv on 1/21/2021.
//

#include "CircularBuffer.h"

#include "../misc/defines.h"

#include <algorithm>

namespace datstr {

    CircularBuffer::CircularBuffer(size_t n) : m_values(std::vector<size_t>(n, 0)), m_currentIndex(0), m_size(n), m_sum(0) {
    }

    void CircularBuffer::add(size_t value) {
        m_sum -= m_values.at(m_currentIndex);
        m_sum += value;
        m_values[m_currentIndex] = value;
        ++m_currentIndex;
        m_currentIndex %= m_size;
    }

    size_t CircularBuffer::size() const {
        return m_size;
    }

    size_t CircularBuffer::sum() const {
        return m_sum;
    }

    double CircularBuffer::average() const {
        return m_sum / static_cast<double>(m_size);
    }

    size_t CircularBuffer::max() const {
        return *std::max_element(D_IT(m_values));
    }

    size_t CircularBuffer::min() const {
        return *std::min_element(D_IT(m_values));
    }
} // namespace datstr