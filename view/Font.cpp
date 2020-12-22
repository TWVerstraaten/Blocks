//
// Created by pc on 16-12-20.
//

#include "Font.h"

#include <cassert>
#include <iostream>

namespace view {

    Font::Font(const std::string& path, Uint16 fontSize)
        : m_font(TTF_OpenFont(path.c_str(), fontSize)), m_loadedCorrectly(m_font != nullptr) {
        if (m_font == nullptr) {
            std::cout << "Could not load font! SDL_ttf Error: " << TTF_GetError() << '\n';
        }

        if (m_loadedCorrectly) {
            assert(m_font != nullptr);
        }
    }

    TTF_Font* Font::font() const {
        assert(m_loadedCorrectly);
        return m_font;
    }

    bool Font::loadedCorrectly() const {
        return m_loadedCorrectly;
    }

    Font::~Font() {
        if (m_loadedCorrectly) {
            TTF_CloseFont(m_font);
            m_font = nullptr;
        }
    }

    int Font::widthOfString(const std::string& str) const {
        int w;
        TTF_SizeText(m_font, str.c_str(), &w, nullptr);
        return w;
    }
} // namespace view