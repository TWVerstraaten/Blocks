//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_RECTANGLE_H
#define BLOCKS_RECTANGLE_H

#include <SDL2/SDL.h>

class SDL_Renderer;

namespace view {

    class Rectangle {
      public:
        Rectangle(int left, int top, size_t width, size_t height, const SDL_Color& outlineColor);
        Rectangle(int left, int top, size_t width, size_t height, const SDL_Color& outlineColor,
                  const SDL_Color& fillColor);
        Rectangle(SDL_Rect rect , const SDL_Color& outlineColor);
        Rectangle(SDL_Rect rect, const SDL_Color& outlineColor,
                  const SDL_Color& fillColor);

        void render(SDL_Renderer* renderer) const;
        void setRect(int left, int top, int width, int height);
        void setFillColor(SDL_Color color);
        void setLineThickNess(size_t thickness);

      private:
        SDL_Rect  m_rect;
        SDL_Color m_outlineColor;
        bool      m_fill          = false;
        SDL_Color m_fillColor     = {0, 0, 0};
        size_t    m_lineThickness = 4;
    };
} // namespace view
#endif // BLOCKS_RECTANGLE_H
