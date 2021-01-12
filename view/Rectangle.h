//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_RECTANGLE_H
#define BLOCKS_RECTANGLE_H

#include <SDL.h>

namespace view {

    class Rectangle {
      public:
        /****** PUBLIC STATIC FUNCTIONS  ******/
        static void render(int left, int top, int width, int height, const SDL_Color& color, SDL_Renderer* renderer);
        static void render(SDL_Rect rect, const SDL_Color& color, SDL_Renderer* renderer);

        /****** CONST FUNCTIONS  ******/
        void render(SDL_Renderer* renderer) const;

        /****** NON CONST FUNCTIONS  ******/
        void setRect(int left, int top, int width, int height);
        void setColor(SDL_Color color);

      private:
        /****** PRIVATE NON CONST FUNCTIONS  ******/
        Rectangle(int left, int top, int width, int height, const SDL_Color& color);
        Rectangle(SDL_Rect rect, const SDL_Color& color);

        /****** DATA MEMBERS  ******/
        SDL_Rect  m_rect;
        SDL_Color m_color = {0, 0, 0, 255};
    };
} // namespace view
#endif // BLOCKS_RECTANGLE_H
