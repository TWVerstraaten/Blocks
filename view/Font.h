//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_FONT_H
#define BLOCKS_FONT_H

#include <SDL_ttf.h>
#include <string>

namespace view {

    class Font {
      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        Font(const std::string& path, Uint32 fontSize);
        ~Font();

        /****** CONST GETTERS  ******/
        [[nodiscard]] bool      loadedCorrectly() const;
        [[nodiscard]] TTF_Font* font() const;

        /****** CONST FUNCTIONS  ******/
        [[nodiscard]] int widthOfString(const std::string& str) const;

      private:
        /****** DATA MEMBERS  ******/
        bool      m_loadedCorrectly = false;
        TTF_Font* m_font;
    };
} // namespace view
#endif // BLOCKS_FONT_H
