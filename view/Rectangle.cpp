//
// Created by pc on 15-12-20.
//

#include "Rectangle.h"

#include <cassert>

namespace view {

    Rectangle::Rectangle(int left, int top, int width, int height, const SDL_Color& color)
        : m_rect{left, top, width, height}, m_color(color) {
    }

    Rectangle::Rectangle(SDL_Rect rect, const SDL_Color& color) : m_rect{rect}, m_color(color) {
    }

    void Rectangle::render(SDL_Renderer* renderer) const {
        SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
        SDL_RenderFillRect(renderer, &m_rect);
    }

    void Rectangle::setRect(int left, int top, int width, int height) {
        m_rect = {left, top, width, height};
    }

    void Rectangle::setColor(SDL_Color color) {
        m_color = color;
    }

    void Rectangle::render(int left, int top, int width, int height, const SDL_Color& color, SDL_Renderer* renderer) {
        static Rectangle rectangle(left, top, width, height, color);
        rectangle.setRect(left, top, width, height);
        rectangle.setColor(color);
        rectangle.render(renderer);
    }

    void Rectangle::render(SDL_Rect rect, const SDL_Color& color, SDL_Renderer* renderer) {
        Rectangle rectangle(rect, color);
        rectangle.setRect(rect.x, rect.y, rect.w, rect.h);
        rectangle.setColor(color);
        rectangle.render(renderer);
    }
} // namespace view