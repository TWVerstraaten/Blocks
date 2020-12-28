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

            /****** PUBLIC ENUMS / TYPEDEFS  ******/
            enum class MODE { SINGLE, DOUBLE };
            struct Data {
                size_t m_stringIndex = std::numeric_limits<size_t>::max();
                size_t m_charIndex   = std::numeric_limits<size_t>::max();
            };

            /****** PUBLIC STATIC FUNCTIONS  ******/
            static bool isReversed(const Data& first, const Data& last);

            /****** CONST GETTERS  ******/
            bool empty() const;

            /****** CONST FUNCTIONS  ******/
            bool isAtStart() const;

            /****** NON CONST FUNCTIONS  ******/
            void reset();
            void fix();

            /****** DATA MEMBERS  ******/
            MODE m_mode = MODE::SINGLE;
            Data m_first;
            Data m_last;
        };

    } // namespace widget
} // namespace view

#endif // BLOCKS_SELECTIONDATA_H
