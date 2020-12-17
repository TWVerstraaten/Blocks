//
// Created by pc on 17-12-20.
//

#include "SelectionData.h"

void view::widget::SelectionData::reset() {
    m_first.m_stringIndex = std::numeric_limits<size_t>::max();
    m_first.m_charIndex   = std::numeric_limits<size_t>::max();
    m_last.m_stringIndex  = std::numeric_limits<size_t>::max();
    m_last.m_charIndex    = std::numeric_limits<size_t>::max();
    m_mode                = MODE::SINGLE;
}

void view::widget::SelectionData::fix() {
    if (m_first.m_stringIndex == m_last.m_stringIndex && m_first.m_charIndex == m_last.m_charIndex) {
        m_mode = MODE::SINGLE;
    }
}

bool view::widget::SelectionData::empty() const {
    return m_first.m_stringIndex == std::numeric_limits<size_t>::max();
}

bool view::widget::SelectionData::isReversed() const {
    return (m_first.m_stringIndex > m_last.m_stringIndex) ||
           (m_first.m_stringIndex == m_last.m_stringIndex && m_first.m_charIndex > m_last.m_charIndex);
}

bool view::widget::SelectionData::isAtStart() const {
    return m_first.m_stringIndex == 0 && m_first.m_charIndex == 0;
}

bool view::widget::SelectionData::isReversed(const view::widget::SelectionData::Data& first,
                                             const view::widget::SelectionData::Data& last) {
    return (first.m_stringIndex > last.m_stringIndex) ||
           (first.m_stringIndex == last.m_stringIndex && first.m_charIndex > last.m_charIndex);
}
