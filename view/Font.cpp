//
// Created by pc on 16-12-20.
//

#include "Font.h"

#include <cassert>
#include <iostream>

Font::Font(const std::string& path) : m_font(TTF_OpenFont(path.c_str(), 28)), m_loadedCorrectly(m_font != nullptr) {
    if (m_font == nullptr) {
        std::cout << "Could not load font! SDL_ttf Error: " << TTF_GetError() << '\n';
    }

    if (m_loadedCorrectly) {
        assert(m_font != nullptr);
    }
}

TTF_Font* Font::font() const {
    return m_font;
}

bool Font::loadedCorrectly() const {
    return m_loadedCorrectly;
}

Font::~Font() {
    TTF_CloseFont(m_font);
    m_font = nullptr;
}
