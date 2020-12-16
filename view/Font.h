//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_FONT_H
#define BLOCKS_FONT_H

#include <SDL2/SDL_ttf.h>
#include <string>

class Font {
  public:
    Font(const std::string& path);
    ~Font();

    [[nodiscard]] bool      loadedCorrectly() const;
    [[nodiscard]] TTF_Font* font() const;

  private:
    TTF_Font* m_font;
    bool      m_loadedCorrectly;
};

#endif // BLOCKS_FONT_H
