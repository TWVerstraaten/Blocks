//
// Created by pc on 16-12-20.
//

#include "LineEditBox.h"

#include "../../global/cst.h"
#include "../../global/fns.h"
#include "../../global/geom.h"
#include "../Assets.h"
#include "../Mouse.h"
#include "../Rectangle.h"

#include <algorithm>
#include <cassert>
#include <sstream>
#include <utility>

static std::string toUpper(const std::string& text) {
    std::string upperCaseString;
    std::transform(text.cbegin(), text.cend(), std::back_inserter(upperCaseString), ::toupper);
    return upperCaseString;
}

namespace view {
    namespace widget {
        LineEditBox::LineEditBox(int x, int y, Uint32 w, Uint32 h, const Assets* assetHandler, std::string title)
            : RectWidget({x, y, static_cast<int>(w), static_cast<int>(h)}), m_assets(assetHandler), m_title(std::move(title)) {
        }

        void LineEditBox::update(SDL_Renderer* renderer) {
            if (not m_titleTexture) {
                m_titleTexture =
                    Texture::createFromText(m_title, cst::color::BLACK, renderer, m_assets->font(Assets::FONT_ENUM::MAIN)->font());
            }

            m_textures.clear();
            m_yOffsets.clear();
        }

        void LineEditBox::render(SDL_Renderer* renderer) {
            if (m_needsUpdate) {
                update(renderer);
            }
            Rectangle::render(geom::pad(m_rect, cst::LINE_EDIT_PADDING),
                              m_active ? cst::color::EDIT_BOX_BACKGROUND : cst::color::EDIT_BOX_BACKGROUND_INACTIVE,
                              renderer);

            Assets::renderTexture(m_titleTexture.get(), {m_rect.x, m_rect.y, m_titleTexture->width(), m_titleTexture->height()}, renderer);
            renderHighlightIfSelected(renderer);
            renderStrings(renderer);
        }

        void LineEditBox::renderHighlightIfSelected(SDL_Renderer* renderer) {
            if (not m_selectionData.empty()) {
                renderSelection(renderer);
            }
        }

        void LineEditBox::renderStrings(SDL_Renderer* renderer) {
            int yOffset = cst::LINE_EDIT_TITLE_HEIGHT;
            for (const auto& texture : m_textures) {
                Assets::renderTexture(texture.get(), {m_rect.x, m_rect.y + yOffset, texture->width(), texture->height()}, renderer);
                yOffset += texture->height();
            }
        }

        void LineEditBox::keyEvent(const SDL_Event& event) {
            if (m_selectionData.m_first.m_stringIndex == std::numeric_limits<size_t>::max()) {
                return;
            } else {
                assert(m_selectionData.m_first.m_stringIndex < m_strings.size());
                switch (event.type) {
                    case SDL_KEYDOWN:
                        handleKeyDown(event);
                        break;
                    case SDL_TEXTINPUT:
                        insertText(std::string(event.text.text));
                        break;
                    default:
                        break;
                }
            }
        }

        void LineEditBox::leftClickEvent(const SDL_Event& event) {
            assert(event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP);
            m_selectionData.reset();
            const auto mousePoint = Mouse::getMouseXY();
            assert(SDL_PointInRect(&mousePoint, &m_rect));
            getSelectionFromMousePoint(m_selectionData.m_first, mousePoint);
            m_selectionData.m_mode = SelectionData::MODE::SINGLE;
            m_blinkTimeOffset      = SDL_GetTicks();
        }

        void LineEditBox::loseFocus() {
            RectWidget::loseFocus();
            m_selectionData.reset();
        }

        void LineEditBox::insertEmptyBeforeLine(size_t lineNumber) {
            m_strings.insert(m_strings.begin() + lineNumber, "");
            m_selectionData.m_mode                = SelectionData::MODE::SINGLE;
            m_selectionData.m_first.m_charIndex   = 0;
            m_selectionData.m_first.m_stringIndex = lineNumber;
            m_needsUpdate                         = true;
        }

        void LineEditBox::handleKeyDown(const SDL_Event& event) {
            assert(event.type == SDL_KEYDOWN);
            if (SDL_GetModState() & KMOD_CTRL) {
                handleKeyDownControlPressed(event);
            }
            switch (event.key.keysym.sym) {
                case SDLK_BACKSPACE:
                    doBackSpace();
                    break;
                case SDLK_DELETE:
                    doDelete();
                    break;
                case SDLK_RETURN:
                    doReturn(SDL_GetModState() & KMOD_SHIFT);
                    break;
                case SDLK_UP:
                    moveUp();
                    break;
                case SDLK_DOWN:
                    moveDown();
                    break;
                case SDLK_LEFT:
                    moveLeft(SDL_GetModState() & KMOD_SHIFT);
                    break;
                case SDLK_RIGHT:
                    moveRight(SDL_GetModState() & KMOD_SHIFT);
                    break;
                default:
                    break;
            }
            m_blinkTimeOffset = SDL_GetTicks();
        }

        void LineEditBox::handleKeyDownControlPressed(const SDL_Event& event) {
            switch (event.key.keysym.sym) {
                case SDLK_c:
                    copyIfSelectionNotEmpty();
                    break;
                case SDLK_v:
                    insertText(SDL_GetClipboardText());
                    break;
                case SDLK_x:
                    copyIfSelectionNotEmpty();
                    deleteRange(m_selectionData.m_first, m_selectionData.m_last);
                    break;
                default:
                    break;
            }
        }

        void LineEditBox::getSelectionFromMousePoint(SelectionData::Data& data, const SDL_Point& mousePoint) const {
            data.m_stringIndex = 0;
            if (mousePoint.y >= m_rect.y) {
                while (data.m_stringIndex < m_yOffsets.size() && mousePoint.y > m_yOffsets.at(data.m_stringIndex) + m_rect.y) {
                    ++data.m_stringIndex;
                }
                if (data.m_stringIndex > 0) {
                    --data.m_stringIndex;
                }
                if (data.m_stringIndex >= m_strings.size()) {
                    assert(data.m_stringIndex == m_strings.size());
                    --data.m_stringIndex;
                }
            }

            const auto& selected = m_strings.at(data.m_stringIndex);
            data.m_charIndex     = selected.length();
            while (widthOfString(selected.substr(0, data.m_charIndex)) > mousePoint.x - m_rect.x) {
                --data.m_charIndex;
            }
        }

        void LineEditBox::mouseDragEvent(const SDL_Event& event) {

            const auto mousePoint = Mouse::getMouseXY();

            getSelectionFromMousePoint(m_selectionData.m_last,
                                       SDL_Point{fns::clamp(mousePoint.x, m_rect.x, m_rect.x + m_rect.w),
                                                 fns::clamp(mousePoint.y, m_rect.y, m_rect.y + m_rect.h)});
            m_selectionData.m_mode = SelectionData::MODE::DOUBLE;
            m_selectionData.fix();
            m_blinkTimeOffset = SDL_GetTicks();
        }

        void LineEditBox::drawDashAt(const SelectionData::Data& data, SDL_Renderer* renderer) const {
            if (not(((SDL_GetTicks() - m_blinkTimeOffset) / 400) % 2)) {
                Rectangle::render(m_rect.x + widthOfString(m_strings.at(data.m_stringIndex).substr(0, data.m_charIndex)),
                                  m_rect.y + m_yOffsets.at(data.m_stringIndex),
                                  3,
                                  m_yOffsets.at(data.m_stringIndex + 1) - m_yOffsets.at(data.m_stringIndex),
                                  {0, 0, 0, 255},
                                  renderer);
            }
        }

        void LineEditBox::renderSelection(SDL_Renderer* renderer) {
            switch (m_selectionData.m_mode) {
                case SelectionData::MODE::SINGLE:
                    highlightString(m_selectionData.m_first.m_stringIndex, renderer, HIGHLIGHT_MODE::SOFT);
                    break;
                case SelectionData::MODE::DOUBLE:
                    highlightRange(m_selectionData.m_first, m_selectionData.m_last, renderer, HIGHLIGHT_MODE::HARD);
                    break;
            }
            if (hasFocus()) {
                drawDashAt(m_selectionData.m_mode == SelectionData::MODE::DOUBLE ? m_selectionData.m_last : m_selectionData.m_first,
                           renderer);
            }
        }

        void LineEditBox::highlightString(size_t stringIndex, SDL_Renderer* renderer, HIGHLIGHT_MODE mode) const {
            assert(stringIndex < m_strings.size());
            Rectangle::render(m_rect.x,
                              m_rect.y + m_yOffsets.at(stringIndex),
                              m_rect.w,
                              m_yOffsets.at(stringIndex + 1) - m_yOffsets.at(stringIndex),
                              getHighlightColor(mode),
                              renderer);
        }

        void LineEditBox::highlightStringPartial(
            size_t stringIndex, size_t firstCharIndex, size_t lastCharIndex, SDL_Renderer* renderer, HIGHLIGHT_MODE mode) const {
            assert(stringIndex < m_strings.size());
            assert(lastCharIndex >= firstCharIndex);
            if (lastCharIndex == firstCharIndex) {
                return;
            }
            const auto leftOffset  = widthOfString(m_strings.at(stringIndex).substr(0, firstCharIndex));
            const auto rightOffset = lastCharIndex == std::string ::npos ? -m_rect.w + m_textures.at(stringIndex)->width()
                                                                         : widthOfString(m_strings.at(stringIndex).substr(lastCharIndex));
            Rectangle::render(m_rect.x + leftOffset,
                              m_rect.y + m_yOffsets.at(stringIndex),
                              m_textures.at(stringIndex)->width() - (leftOffset + rightOffset),
                              m_yOffsets.at(stringIndex + 1) - m_yOffsets.at(stringIndex),
                              getHighlightColor(mode),
                              renderer);
        }

        void LineEditBox::highlightRange(const SelectionData::Data& first,
                                         const SelectionData::Data& last,
                                         SDL_Renderer*              renderer,
                                         HIGHLIGHT_MODE             mode) const {
            if (SelectionData::isReversed(first, last)) {
                highlightRange(last, first, renderer, mode);
            } else {
                assert(first.m_stringIndex <= last.m_stringIndex);
                if (first.m_stringIndex == last.m_stringIndex) {
                    assert(first.m_charIndex <= last.m_charIndex);
                    highlightStringPartial(first.m_stringIndex, first.m_charIndex, last.m_charIndex, renderer, mode);
                } else {
                    highlightStringPartial(first.m_stringIndex, first.m_charIndex, std::string::npos, renderer, mode);
                    highlightStringPartial(last.m_stringIndex, 0, last.m_charIndex, renderer, mode);
                    if (first.m_stringIndex + 1 < last.m_stringIndex) {
                        highlightStrings(first.m_stringIndex + 1, last.m_stringIndex, renderer, mode);
                    }
                }
            }
        }

        void LineEditBox::highlightStrings(size_t         firstStringIndex,
                                           size_t         lastStringIndex,
                                           SDL_Renderer*  renderer,
                                           HIGHLIGHT_MODE mode) const {
            assert(firstStringIndex < m_strings.size());
            assert(lastStringIndex < m_strings.size());
            assert(firstStringIndex < lastStringIndex);
            Rectangle::render(m_rect.x,
                              m_rect.y + m_yOffsets.at(firstStringIndex),
                              m_rect.w,
                              m_yOffsets.at(lastStringIndex) - m_yOffsets.at(firstStringIndex),
                              getHighlightColor(mode),
                              renderer);
        }

        int LineEditBox::widthOfString(const std::string& string) const {
            return m_assets->font(Assets::FONT_ENUM::MAIN)->widthOfString(string);
        }

        void LineEditBox::doBackSpace() {
            assert(not m_selectionData.empty());
            switch (m_selectionData.m_mode) {
                case SelectionData::MODE::SINGLE:
                    assert(m_selectionData.m_first.m_stringIndex < m_strings.size());
                    if (m_selectionData.isAtStart()) {
                        return;
                    }
                    m_needsUpdate = true;
                    {
                        auto& stringIndex = m_selectionData.m_first.m_stringIndex;
                        auto& charIndex   = m_selectionData.m_first.m_charIndex;
                        if (charIndex > 0) {
                            auto& str = m_strings[stringIndex];
                            str.erase(str.begin() + charIndex - 1);
                            --charIndex;
                        } else {
                            --stringIndex;
                            charIndex = m_strings.at(stringIndex).length();
                            concatAt(stringIndex);
                        }
                    }

                    break;
                case SelectionData::MODE::DOUBLE:
                    deleteRange(m_selectionData.m_first, m_selectionData.m_last);
                    m_needsUpdate = true;
                    break;
            }
        }

        void LineEditBox::deleteRange(const SelectionData::Data& first, const SelectionData::Data& last) {
            if (SelectionData::isReversed(first, last)) {
                deleteRange(last, first);
                return;
            }
            m_needsUpdate = true;
            assert(first.m_stringIndex <= last.m_stringIndex);
            if (first.m_stringIndex == last.m_stringIndex) {
                assert(first.m_charIndex < last.m_charIndex);
                auto& str = m_strings[first.m_stringIndex];
                str.erase(str.begin() + first.m_charIndex, str.begin() + last.m_charIndex);
                m_selectionData.m_mode                = SelectionData::MODE::SINGLE;
                m_selectionData.m_first.m_stringIndex = first.m_stringIndex;
                m_selectionData.m_first.m_charIndex   = first.m_charIndex;
            } else {
                m_strings.insert(m_strings.begin() + first.m_stringIndex,
                                 m_strings[first.m_stringIndex].substr(0, first.m_charIndex) +
                                     m_strings.at(last.m_stringIndex).substr(last.m_charIndex));
                m_strings.erase(m_strings.begin() + first.m_stringIndex + 1, m_strings.begin() + last.m_stringIndex + 2);
                m_selectionData.m_mode                = SelectionData::MODE::SINGLE;
                m_selectionData.m_first.m_stringIndex = first.m_stringIndex;
                m_selectionData.m_first.m_charIndex   = first.m_charIndex;
            }
        }

        void LineEditBox::doReturn(bool shiftPressed) {
            if (shiftPressed && m_selectionData.m_mode == SelectionData::MODE::SINGLE) {
                insertEmptyBeforeLine(m_selectionData.m_first.m_stringIndex);
                return;
            }
            switch (m_selectionData.m_mode) {
                case SelectionData::MODE::SINGLE:
                    splitAt(m_selectionData.m_first);
                    break;
                case SelectionData::MODE::DOUBLE:
                    deleteRange(m_selectionData.m_first, m_selectionData.m_last);
                    splitAt(m_selectionData.m_first);
                    break;
            }
        }

        void LineEditBox::splitAt(SelectionData::Data& data) {
            const auto stringIndex = data.m_stringIndex;
            const auto charIndex   = data.m_charIndex;
            m_strings.insert(m_strings.begin() + stringIndex + 1, "");
            m_strings[stringIndex + 1] = m_strings[stringIndex].substr(charIndex);
            m_strings[stringIndex]     = m_strings[stringIndex].substr(0, charIndex);
            ++data.m_stringIndex;
            data.m_charIndex       = 0;
            m_selectionData.m_mode = SelectionData::MODE::SINGLE;
            m_needsUpdate          = true;
        }

        void LineEditBox::moveUp() {
            swapIfRangeIsSelected();
            m_selectionData.m_first.m_stringIndex -= m_selectionData.m_first.m_stringIndex == 0 ? 0 : 1;
            potentiallyDecrementFirstCharIndex();
            m_selectionData.m_mode = SelectionData::MODE::SINGLE;
        }

        void LineEditBox::moveDown() {
            swapIfRangeIsSelected();
            if (m_selectionData.m_first.m_stringIndex + 1 == m_strings.size()) {
                m_strings.emplace_back("");
                m_needsUpdate = true;
            }
            m_selectionData.m_first = moveSelectionOneDown(m_selectionData.m_first);
            m_selectionData.m_mode  = SelectionData::MODE::SINGLE;
        }

        void LineEditBox::moveRight(bool shiftPressed) {
            swapIfRangeIsSelected();
            if (shiftPressed) {
                m_selectionData.m_first.m_charIndex = m_strings.at(m_selectionData.m_first.m_stringIndex).length();
            } else {
                if (m_selectionData.m_first.m_charIndex != m_strings.at(m_selectionData.m_first.m_stringIndex).length()) {
                    ++m_selectionData.m_first.m_charIndex;
                } else {
                    if (m_selectionData.m_first.m_stringIndex + 1 != m_strings.size()) {
                        ++m_selectionData.m_first.m_stringIndex;
                        m_selectionData.m_first.m_charIndex = 0;
                    }
                }
            }
            m_selectionData.m_mode = SelectionData::MODE::SINGLE;
        }

        void LineEditBox::moveLeft(bool shiftPressed) {
            swapIfRangeIsSelected();
            if (shiftPressed) {
                m_selectionData.m_first.m_charIndex = 0;
            } else if (not m_selectionData.isAtStart()) {
                if (m_selectionData.m_first.m_charIndex != 0) {
                    --m_selectionData.m_first.m_charIndex;
                } else {
                    --m_selectionData.m_first.m_stringIndex;
                    m_selectionData.m_first.m_charIndex = m_strings.at(m_selectionData.m_first.m_stringIndex).length();
                }
            }
            m_selectionData.m_mode = SelectionData::MODE::SINGLE;
        }

        void LineEditBox::swapIfRangeIsSelected() {
            if (m_selectionData.m_mode == SelectionData::MODE::DOUBLE) {
                std::swap(m_selectionData.m_first, m_selectionData.m_last);
            }
        }

        void LineEditBox::doDelete() {
            assert(not m_selectionData.empty());
            switch (m_selectionData.m_mode) {
                case SelectionData::MODE::SINGLE:
                    assert(m_selectionData.m_first.m_stringIndex < m_strings.size());
                    {
                        if (m_selectionData.m_first.m_charIndex != m_strings.at(m_selectionData.m_first.m_stringIndex).length()) {
                            auto& str = m_strings[m_selectionData.m_first.m_stringIndex];
                            str.erase(str.begin() + m_selectionData.m_first.m_charIndex);
                        } else {
                            if (m_selectionData.m_first.m_stringIndex + 1 != m_strings.size()) {
                                concatAt(m_selectionData.m_first.m_stringIndex);
                            }
                        }
                    }
                    m_needsUpdate = true;
                    break;
                case SelectionData::MODE::DOUBLE:
                    deleteRange(m_selectionData.m_first, m_selectionData.m_last);
                    m_needsUpdate = true;
                    break;
            }
        }

        std::string LineEditBox::selectionToString(const SelectionData::Data& first, const SelectionData::Data& last) const {
            assert(m_selectionData.m_mode != SelectionData::MODE::SINGLE);
            if (SelectionData::isReversed(first, last)) {
                return selectionToString(last, first);
            }
            std::stringstream buffer;

            if (first.m_stringIndex == last.m_stringIndex) {
                assert(first.m_charIndex < last.m_charIndex);
                auto& str = m_strings[first.m_stringIndex];
                buffer << std::string(str.begin() + first.m_charIndex, str.begin() + last.m_charIndex);
            } else {
                auto& str = m_strings[first.m_stringIndex];
                buffer << std::string(str.begin() + first.m_charIndex, str.end());
                for (size_t i = first.m_stringIndex + 1; i != last.m_stringIndex; ++i) {
                    buffer << "\n";
                    buffer << m_strings.at(i);
                }
                buffer << "\n";
                buffer << std::string(m_strings.at(last.m_stringIndex).begin(),
                                      m_strings.at(last.m_stringIndex).begin() + last.m_charIndex);
            }
            return buffer.str();
        }

        void LineEditBox::copyIfSelectionNotEmpty() {
            if (m_selectionData.m_mode == SelectionData::MODE::SINGLE) {
                return;
            }
            SDL_SetClipboardText(selectionToString(m_selectionData.m_first, m_selectionData.m_last).c_str());
        }

        void LineEditBox::insertText(const std::string& text) {
            if (m_selectionData.m_mode == SelectionData::MODE::DOUBLE) {
                deleteRange(m_selectionData.m_first, m_selectionData.m_last);
            }
            assert(m_selectionData.m_mode == SelectionData::MODE::SINGLE);
            if (text.length() == 0) {
                return;
            }
            m_needsUpdate = true;
            m_strings[m_selectionData.m_first.m_stringIndex].insert(m_selectionData.m_first.m_charIndex, toUpper(text));
            const auto lastNewLine = m_strings[m_selectionData.m_first.m_stringIndex].find_last_of('\n');
            if (lastNewLine == std::string::npos) {
                m_selectionData.m_first.m_charIndex += text.length();
            } else {
                m_selectionData.m_first.m_charIndex += text.length() - lastNewLine - 1;
                std::string str = std::move(m_strings[m_selectionData.m_first.m_stringIndex]);
                m_strings.erase(m_strings.begin() + m_selectionData.m_first.m_stringIndex);

                std::stringstream ss(str);
                std::string       result;
                while (std::getline(ss, result, '\n')) {
                    m_strings.insert(m_strings.begin() + m_selectionData.m_first.m_stringIndex, result);
                    ++m_selectionData.m_first.m_stringIndex;
                }
                --m_selectionData.m_first.m_stringIndex;
            }
        }

        void LineEditBox::concatAt(size_t index) {
            m_strings[index] += m_strings[index + 1];
            m_strings.erase(m_strings.begin() + index + 1);
        }

        void LineEditBox::potentiallyDecrementFirstCharIndex() {
            m_selectionData.m_first.m_charIndex =
                std::min(m_selectionData.m_first.m_charIndex, m_strings.at(m_selectionData.m_first.m_stringIndex).length());
        }

        void LineEditBox::setHighLightedLine(size_t index, bool skipEmpty) {
            if (not skipEmpty) {
                m_selectionData.m_first.m_stringIndex = index;
            } else {
                NOTE_ONCE("Remove")
                for (const auto& it : m_strings) {
                    if (fns::trimWhiteSpace(it).empty()) {
                        assert(it.empty());
                    }
                }

                auto it = std::find_if(m_strings.begin(), m_strings.end(), [](const std::string& str) { return not str.empty(); });
                if (it == m_strings.end()) {
                    assert(index == 0);
                    m_selectionData.m_first.m_stringIndex = 0;
                } else {
                    while (index != 0) {
                        it = std::find_if(std::next(it), m_strings.end(), [](const auto& str) { return not str.empty(); });
                        --index;
                    }
                    m_selectionData.m_first.m_stringIndex = std::distance(m_strings.begin(), it);
                }
            }

            m_selectionData.m_first.m_charIndex = 0;
            m_selectionData.m_mode              = SelectionData::MODE::SINGLE;
        }

        SelectionData::Data LineEditBox::moveSelectionOneDown(const SelectionData::Data& data) const {
            assert(data.m_stringIndex + 1 < m_strings.size());
            SelectionData::Data result;
            result.m_stringIndex = data.m_stringIndex + 1ul;
            result.m_charIndex   = std::min(data.m_charIndex, m_strings.at(result.m_stringIndex).length());
            return result;
        }

        int LineEditBox::width() const {
            return m_rect.w;
        }

        int LineEditBox::height() const {
            return m_rect.h;
        }

        void LineEditBox::setActive(bool active) {
            m_active = active;
        }

        const SDL_Color& LineEditBox::getHighlightColor(HIGHLIGHT_MODE mode) const {
            return m_active ? (mode == HIGHLIGHT_MODE::HARD ? cst::color::EDIT_BOX_HIGHLIGHT_HARD : cst::color::EDIT_BOX_HIGHLIGHT_SOFT)
                            : cst::color::EDIT_BOX_HIGHLIGHT_DEAD;
        }

        void LineEditBox::appendString(const std::string& str) {
            m_strings.emplace_back(str);
        }

        const std::string& LineEditBox::title() const {
            return m_title;
        }

    } // namespace widget
} // namespace view
