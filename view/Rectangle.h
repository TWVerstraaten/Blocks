//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_RECTANGLE_H
#define BLOCKS_RECTANGLE_H

#include <SDL.h>

class SDL_Renderer;

namespace view {

    class Rectangle {
      public:
        static void render(int left, int top, int width, int height, const SDL_Color& color, SDL_Renderer* renderer);
        static void render(SDL_Rect rect, const SDL_Color& color, SDL_Renderer* renderer);

        void render(SDL_Renderer* renderer) const;
        void setRect(int left, int top, int width, int height);
        void setColor(SDL_Color color);

      private:
        Rectangle(int left, int top, int width, int height, const SDL_Color& color);
        Rectangle(SDL_Rect rect, const SDL_Color& color);

        SDL_Rect  m_rect;
        SDL_Color m_color = {0, 0, 0};
    };
} // namespace view
#endif // BLOCKS_RECTANGLE_H
