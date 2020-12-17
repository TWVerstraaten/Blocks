//
// Created by pc on 16-12-20.
//

#include "ActionEditBox.h"

#include "AssetHandler.h"
#include "Aux.h"
#include "Rectangle.h"

#include <cassert>
#include <iostream>

namespace view {
    ActionEditBox::ActionEditBox(int x, int y, int w, int h, const AssetHandler* assetHandler,
                                 const model::Cluster& cluster)
        : Widget({x, y, w, h}), m_assetHandler(assetHandler) {
        for (const auto& action : cluster.clusterActions()) {
            std::string str;
            switch (action.m_modifier) {
                case model::ClusterAction::MODIFIER::NONE:
                    str += "  ";
                    break;
                case model::ClusterAction::MODIFIER::IGNORE:
                    str += "+ ";
                    break;
                case model::ClusterAction::MODIFIER::SKIP:
                    str += "- ";
                    break;
            }
            switch (action.m_action) {
                case model::ClusterAction::ACTION::MOVE_UP:
                    str += "FWD";
                    break;
                case model::ClusterAction::ACTION::MOVE_DOWN:
                    str += "BCK";
                    break;
                case model::ClusterAction::ACTION::MOVE_LEFT:
                    str += "LFT";
                    break;
                case model::ClusterAction::ACTION::MOVE_RIGHT:
                    str += "RIT";
                    break;
            }
            m_strings.push_back(str);
        }
    }

    void ActionEditBox::init(SDL_Renderer* renderer) {
        m_textures.clear();
        m_verticalDelimiters.clear();

        int yOffset = 0;
        for (const auto& str : m_strings) {
            m_verticalDelimiters.push_back(yOffset);
            m_textures.emplace_back(
                Texture::buildFromText(str.length() == 0 ? " " : str, {0, 0, 0, 255}, renderer,
                                       m_assetHandler->font(AssetHandler::FONT_ENUM::MAIN)->font()));
            yOffset += m_textures.back()->height();
        }
        m_verticalDelimiters.push_back(yOffset);
        m_rect.h = yOffset;
    }

    void ActionEditBox::render(SDL_Renderer* renderer) {
        if (m_needsUpdate) {
            init(renderer);
            m_needsUpdate = false;
        }

        Rectangle::render(aux::pad(m_rect, 10), {200, 200, 250, 255}, renderer);
        if (not m_selectionData.empty()) {
            renderSelection(renderer);
        }

        int yOffset = 0;
        for (const auto& texture : m_textures) {
            AssetHandler::renderTexture(texture.get(),
                                        {m_rect.x, m_rect.y + yOffset, texture->width(), texture->height()}, renderer);
            yOffset += texture->height();
        }
    }

    void ActionEditBox::handleKeyEvent(const SDL_Event& event) {
        if (m_selectionData.m_stringIndex == std::numeric_limits<size_t>::max()) {
            return;
        } else {
            assert(m_selectionData.m_stringIndex < m_strings.size());
            switch (event.type) {
                case SDL_KEYDOWN:
                    handleKeyDown(event);
                    break;
                case SDL_TEXTINPUT:
                    auto& str = m_strings[m_selectionData.m_stringIndex];
                    if (!(SDL_GetModState() & KMOD_CTRL && (event.text.text[0] == 'c' || event.text.text[0] == 'C' ||
                                                            event.text.text[0] == 'v' || event.text.text[0] == 'V'))) {
                        str = str.substr(0, m_selectionData.m_charIndex) + event.text.text +
                              str.substr(m_selectionData.m_charIndex);
                        m_needsUpdate = true;
                    }
                    break;
            }
        }
    }

    void ActionEditBox::handleMouseClickEvent(const SDL_Event& event, bool leftClicked) {
        assert(event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP);
        if (leftClicked) {
            return;
        }
        m_selectionData.reset();
        int xMouse, yMouse;
        SDL_GetMouseState(&xMouse, &yMouse);

        xMouse -= m_rect.x;
        yMouse -= m_rect.y;
        getSelectionFromLocalMousePoint(xMouse, yMouse, m_selectionData.m_stringIndex, m_selectionData.m_charIndex);
        m_selectionData.m_mode = SelectionData::MODE::SINGLE;
    }

    const std::vector<std::string>& ActionEditBox::strings() const {
        return m_strings;
    }

    void ActionEditBox::loseFocus() {
        Widget::loseFocus();
        m_selectionData.reset();
    }

    void ActionEditBox::incrementHighlightIndex() {
        ++m_selectionData.m_stringIndex;
        if (m_selectionData.m_stringIndex >= m_strings.size()) {
            assert(m_selectionData.m_stringIndex == m_strings.size());
            m_strings.emplace_back("");
            m_needsUpdate = true;
        }
    }

    void ActionEditBox::deleteHighlightedLine() {
        m_strings.erase(m_strings.begin() + m_selectionData.m_stringIndex);
        if (m_strings.empty()) {
            m_strings.emplace_back("");
        }
        if (m_selectionData.m_stringIndex >= m_strings.size()) {
            assert(m_selectionData.m_stringIndex == m_strings.size());
            --m_selectionData.m_stringIndex;
        }
        m_needsUpdate = true;
    }

    void ActionEditBox::insertBeforeHighlightedLine() {
        m_strings.insert(m_strings.begin() + m_selectionData.m_stringIndex, "");
        m_needsUpdate = true;
    }

    void ActionEditBox::handleKeyDown(const SDL_Event& event) {
        assert(event.type == SDL_KEYDOWN);
        if (SDL_GetModState() & KMOD_CTRL) {
            handleKeyDownControlPressed(event);
        }
        auto& str = m_strings[m_selectionData.m_stringIndex];
        switch (event.key.keysym.sym) {
            case SDLK_BACKSPACE:
                if (str.length() > 0) {
                    str.pop_back();
                    --m_selectionData.m_charIndex;
                    m_needsUpdate = true;
                } else {
                    deleteHighlightedLine();
                    m_selectionData.m_stringIndex -= m_selectionData.m_stringIndex == 0 ? 0 : 1;
                    m_selectionData.m_charIndex = m_strings.at(m_selectionData.m_stringIndex).length();
                }
                break;
            case SDLK_RETURN:
                if (SDL_GetModState() & KMOD_SHIFT) {
                    insertBeforeHighlightedLine();
                } else {
                    m_strings.insert(m_strings.begin() + m_selectionData.m_stringIndex + 1, "");
                    ++m_selectionData.m_stringIndex;
                    m_needsUpdate = true;
                }
                break;
            case SDLK_UP:
                m_selectionData.m_stringIndex -= m_selectionData.m_stringIndex == 0 ? 0 : 1;
                break;
            case SDLK_DOWN:
                if (m_selectionData.m_stringIndex + 1 < m_strings.size()) {
                    ++m_selectionData.m_stringIndex;
                } else {
                    if (not m_strings.back().empty()) {
                        incrementHighlightIndex();
                    }
                }
                break;
        }
    }

    void ActionEditBox::handleKeyDownControlPressed(const SDL_Event& event) {
        auto& str = m_strings[m_selectionData.m_stringIndex];
        switch (event.key.keysym.sym) {
            case SDLK_c:
                SDL_SetClipboardText(str.c_str());
                break;
            case SDLK_v:
                insertBeforeHighlightedLine();
                m_strings[m_selectionData.m_stringIndex] = SDL_GetClipboardText();
                incrementHighlightIndex();
                m_needsUpdate = true;
                break;
            case SDLK_x:
                SDL_SetClipboardText(str.c_str());
                deleteHighlightedLine();
                m_needsUpdate = true;
                break;
        }
    }

    void ActionEditBox::getSelectionFromLocalMousePoint(const int xMouse, const int yMouse, size_t& stringIndex,
                                                        size_t& charIndex) const {
        stringIndex = 0;
        if (yMouse >= 0) {
            while (stringIndex < m_verticalDelimiters.size() &&
                   static_cast<size_t>(yMouse) > m_verticalDelimiters.at(stringIndex)) {
                ++stringIndex;
            }
            if (stringIndex > 0) {
                --stringIndex;
            }
            if (stringIndex >= m_strings.size()) {
                --stringIndex;
            }
        }

        const auto& selected = m_strings.at(stringIndex);
        charIndex            = selected.length();
        while (m_assetHandler->font(AssetHandler::FONT_ENUM::MAIN)->widthOfString(selected.substr(0, charIndex)) >
               xMouse) {
            --charIndex;
        }
    }

    void ActionEditBox::handleMouseMoveEvent(const SDL_Point& mousePointInWorld, bool leftClicked) {
        if (not leftClicked) {
            return;
        }
        int xMouse, yMouse;
        SDL_GetMouseState(&xMouse, &yMouse);

        xMouse -= m_rect.x;
        yMouse -= m_rect.y;
        getSelectionFromLocalMousePoint(xMouse, yMouse, m_selectionData.m_secondStringIndex,
                                        m_selectionData.m_secondCharIndex);
        m_selectionData.m_mode = SelectionData::MODE::DOUBLE;
        m_selectionData.fix();
    }

    void ActionEditBox::drawDashAt(size_t stringIndex, size_t charIndex, SDL_Renderer* renderer) const {
        Rectangle::render(
            static_cast<int>(m_assetHandler->font(AssetHandler::FONT_ENUM::MAIN)
                                 ->widthOfString(m_strings.at(stringIndex).substr(0, charIndex))),
            static_cast<int>(m_rect.y + m_verticalDelimiters.at(stringIndex)), 2,
            static_cast<int>(m_verticalDelimiters.at(stringIndex + 1) - m_verticalDelimiters.at(stringIndex)),
            {0, 0, 0, 255}, renderer);
    }

    void ActionEditBox::renderSelection(SDL_Renderer* renderer) {
        switch (m_selectionData.m_mode) {
            case SelectionData::MODE::SINGLE: {
                highlightString(m_selectionData.m_stringIndex, renderer);
                assert(m_selectionData.m_charIndex <= m_strings.at(m_selectionData.m_stringIndex).length());
                drawDashAt(m_selectionData.m_stringIndex, m_selectionData.m_charIndex, renderer);
            } break;
            case SelectionData::MODE::DOUBLE:
                if ((m_selectionData.m_stringIndex > m_selectionData.m_secondStringIndex) ||
                    (m_selectionData.m_stringIndex == m_selectionData.m_secondStringIndex &&
                     m_selectionData.m_charIndex > m_selectionData.m_secondCharIndex)) {
                    highlightRange(m_selectionData.m_secondStringIndex, m_selectionData.m_secondCharIndex,
                                   m_selectionData.m_stringIndex, m_selectionData.m_charIndex, renderer);
                } else {
                    highlightRange(m_selectionData.m_stringIndex, m_selectionData.m_charIndex,
                                   m_selectionData.m_secondStringIndex, m_selectionData.m_secondCharIndex, renderer);
                }
                break;
        }
        if (m_selectionData.m_mode == ActionEditBox::SelectionData::MODE::DOUBLE) {
            assert(m_selectionData.m_secondStringIndex <= m_strings.at(m_selectionData.m_secondStringIndex).length());
            drawDashAt(m_selectionData.m_secondStringIndex, m_selectionData.m_secondCharIndex, renderer);
        }
    }

    void ActionEditBox::highlightString(size_t stringIndex, SDL_Renderer* renderer) const {
        assert(stringIndex < m_strings.size());
        Rectangle::render(
            m_rect.x, static_cast<int>(m_rect.y + m_verticalDelimiters.at(stringIndex)), m_rect.w,
            static_cast<int>(m_verticalDelimiters.at(stringIndex + 1) - m_verticalDelimiters.at(stringIndex)),
            {255, 200, 250, 255}, renderer);
    }

    void ActionEditBox::highlightStringPartial(size_t stringIndex, size_t firstChar, size_t lastChar,
                                               SDL_Renderer* renderer) const {
        assert(stringIndex < m_strings.size());
        assert(lastChar >= firstChar);
        if (lastChar == firstChar) {
            return;
        }
        const auto leftOffset = m_assetHandler->font(AssetHandler::FONT_ENUM::MAIN)
                                    ->widthOfString(m_strings.at(stringIndex).substr(0, firstChar));
        const auto rightOffset = lastChar == std::string ::npos
                                   ? -m_rect.w + m_textures.at(stringIndex)->width()
                                   : m_assetHandler->font(AssetHandler::FONT_ENUM::MAIN)
                                         ->widthOfString(m_strings.at(stringIndex).substr(lastChar));

        Rectangle::render(
            m_rect.x + leftOffset, static_cast<int>(m_rect.y + m_verticalDelimiters.at(stringIndex)),
            m_textures.at(stringIndex)->width() - (leftOffset + rightOffset),
            static_cast<int>(m_verticalDelimiters.at(stringIndex + 1) - m_verticalDelimiters.at(stringIndex)),
            {255, 200, 250, 255}, renderer);
    }

    void ActionEditBox::highlightRange(size_t firstStringIndex, size_t firstCharIndex, size_t secondStringIndex,
                                       size_t secondCharIndex, SDL_Renderer* renderer) const {
        assert(firstStringIndex <= secondStringIndex);
        if (firstStringIndex == secondStringIndex) {
            assert(firstCharIndex <= secondCharIndex);
            highlightStringPartial(firstStringIndex, firstCharIndex, secondCharIndex, renderer);
        } else {
            highlightStringPartial(firstStringIndex, firstCharIndex, std::string ::npos, renderer);
            highlightStringPartial(secondStringIndex, 0, secondCharIndex, renderer);
            for (size_t i = firstStringIndex + 1; i < secondStringIndex; ++i) {
                highlightString(i, renderer);
            }
        }
    }

} // namespace view