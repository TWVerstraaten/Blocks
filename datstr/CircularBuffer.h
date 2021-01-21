//
// Created by teunv on 1/21/2021.
//

#ifndef BLOCKS_CIRCULARBUFFER_H
#define BLOCKS_CIRCULARBUFFER_H

#include <vector>

namespace datstr {

    class CircularBuffer {

      public:
        explicit CircularBuffer(size_t n);

        void add(size_t value);

        [[nodiscard]] size_t size() const;
        [[nodiscard]] size_t sum() const;
        [[nodiscard]] size_t max() const;
        [[nodiscard]] size_t min() const;
        [[nodiscard]] double average() const;

      private:
        std::vector<size_t> m_values;
        size_t              m_currentIndex;
        size_t              m_size;
        size_t              m_sum;
    };

} // namespace datstr

#endif // BLOCKS_CIRCULARBUFFER_H
