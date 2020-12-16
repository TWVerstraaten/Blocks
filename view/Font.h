//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_FONT_H
#define BLOCKS_FONT_H

#include <SDL2/SDL_ttf.h>
#include <string>

namespace view {

    class Font {
      public:
        Font(const std::string& path, size_t fontSize);
        ~Font();

        Font(const Font& other) = delete;

        bool      loadedCorrectly() const;
        TTF_Font* font() const;

      private:
        TTF_Font* m_font;
        bool      m_loadedCorrectly = false;
    };
} // namespace view
#endif // BLOCKS_FONT_H
