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

        view::Rectangle backGround{aux::pad(m_rect, 20), {200, 200, 250, 255}};
        backGround.setFillColor({200, 200, 250, 255});
        backGround.setLineThickNess(8);
        backGround.render(renderer);

        if (m_stringIndex != std::numeric_limits<size_t>::max()) {
            view::Rectangle highLight{
                {m_rect.x, static_cast<int>(m_rect.y + m_verticalDelimiters.at(m_stringIndex)), m_rect.w,
                 static_cast<int>(m_verticalDelimiters.at(m_stringIndex + 1) - m_verticalDelimiters.at(m_stringIndex))},
                {200, 200, 250, 255}};
            highLight.setFillColor({241, 241, 250, 255});
            highLight.setLineThickNess(0);
            highLight.render(renderer);
        }
        int yOffset = 0;
        for (const auto& texture : m_textures) {
            AssetHandler::renderTexture(texture.get(),
                                        {m_rect.x, m_rect.y + yOffset, texture->width(), texture->height()}, renderer);
            yOffset += texture->height();
        }
    }

    void ActionEditBox::handleKeyEvent(const SDL_Event& e) {
        if (m_stringIndex == std::numeric_limits<size_t>::max()) {
            return;
        } else {
            assert(m_stringIndex < m_strings.size());
            auto& str = m_strings[m_stringIndex];
            switch (e.type) {
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                        case SDLK_BACKSPACE:
                            if (str.length() > 0) {
                                str.pop_back();
                                m_needsUpdate = true;
                            } else {
                                deleteHighlightedLine();
                            }
                        case SDLK_c:
                            if (SDL_GetModState() & KMOD_CTRL) {
                                SDL_SetClipboardText(str.c_str());
                            }
                            break;
                        case SDLK_v:
                            if (SDL_GetModState() & KMOD_CTRL) {
                                insertBeforeHighlightedLine();
                                m_strings[m_stringIndex] = SDL_GetClipboardText();
                                incrementHighlightIndex();
                                m_needsUpdate = true;
                            }
                            break;
                        case SDLK_x:
                            if (SDL_GetModState() & KMOD_CTRL) {
                                SDL_SetClipboardText(str.c_str());
                                deleteHighlightedLine();
                                m_needsUpdate = true;
                            }
                            break;
                        case SDLK_RETURN:
                            if (SDL_GetModState() & KMOD_SHIFT) {
                                insertBeforeHighlightedLine();
                            } else {
                                m_strings.insert(m_strings.begin() + m_stringIndex + 1, "");
                                ++m_stringIndex;
                                m_needsUpdate = true;
                            }
                            break;
                        case SDLK_UP:
                            m_stringIndex -= m_stringIndex == 0 ? 0 : 1;
                            break;
                        case SDLK_DOWN:
                            if (m_stringIndex + 1 < m_strings.size()) {
                                ++m_stringIndex;
                            } else {
                                if (not m_strings.back().empty()) {
                                    incrementHighlightIndex();
                                }
                            }
                            break;
                    }
                    break;
                case SDL_TEXTINPUT:
                    if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' ||
                                                            e.text.text[0] == 'v' || e.text.text[0] == 'V'))) {
                        str = str.substr(0, m_positionInString) + e.text.text + str.substr(m_positionInString);
                        m_needsUpdate = true;
                    }
                    break;
            }
        }
    }

    void ActionEditBox::handleMouseClickEvent(const SDL_Event& event) {
        assert(event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP);
        int xMouse, yMouse;
        SDL_GetMouseState(&xMouse, &yMouse);

        xMouse -= m_rect.x;
        yMouse -= m_rect.y;
        m_stringIndex = 0;

        while (m_stringIndex < m_verticalDelimiters.size() &&
               static_cast<size_t>(yMouse) > m_verticalDelimiters.at(m_stringIndex)) {
            ++m_stringIndex;
        }
        --m_stringIndex;

        const auto& selected = m_strings.at(m_stringIndex);
        if (selected.empty()) {
            std::cout << 0 << "\n";
        }
        m_positionInString = selected.length();
        while (
            m_assetHandler->font(AssetHandler::FONT_ENUM::MAIN)->widthOfString(selected.substr(0, m_positionInString)) >
            xMouse) {
            --m_positionInString;
        }
        std::cout << m_positionInString << "\n";
    }

    const std::vector<std::string>& ActionEditBox::strings() const {
        return m_strings;
    }

    void ActionEditBox::loseFocus() {
        Widget::loseFocus();
        m_stringIndex = std::numeric_limits<size_t>::max();
    }

    void ActionEditBox::incrementHighlightIndex() {
        ++m_stringIndex;
        if (m_stringIndex >= m_strings.size()) {
            assert(m_stringIndex == m_strings.size());
            m_strings.emplace_back("");
            m_needsUpdate = true;
        }
    }

    void ActionEditBox::deleteHighlightedLine() {
        m_strings.erase(m_strings.begin() + m_stringIndex);
        if (m_strings.empty()) {
            m_strings.emplace_back("");
        }
        if (m_stringIndex >= m_strings.size()) {
            assert(m_stringIndex == m_strings.size());
            --m_stringIndex;
        }
        m_needsUpdate = true;
    }

    void ActionEditBox::insertBeforeHighlightedLine() {
        m_strings.insert(m_strings.begin() + m_stringIndex, "");
        m_needsUpdate = true;
    }
} // namespace view