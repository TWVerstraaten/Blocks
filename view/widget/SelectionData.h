//
// Created by pc on 17-12-20.
//

#ifndef BLOCKS_SELECTIONDATA_H
#define BLOCKS_SELECTIONDATA_H

#include <cstddef>
#include <limits>

namespace view {
    namespace widget {
        struct SelectionData {
            void reset();

            void fix();

            bool empty() const;

            struct Data {
                size_t m_stringIndex = std::numeric_limits<size_t>::max();
                size_t m_charIndex   = std::numeric_limits<size_t>::max();
            };

            Data m_first;
            Data m_last;

            static bool isReversed(const Data& first, const Data& last);
            bool        isAtStart() const;

            enum class MODE { SINGLE, DOUBLE };
            MODE m_mode = MODE::SINGLE;
        };

    } // namespace widget
} // namespace view

#endif // BLOCKS_SELECTIONDATA_H
