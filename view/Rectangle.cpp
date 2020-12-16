//
// Created by pc on 15-12-20.
//

#include "Rectangle.h"

#include <cassert>

namespace view {

    Rectangle::Rectangle(int left, int top, size_t width, size_t height, const SDL_Color& outlineColor)
        : m_rect{left, top, static_cast<int>(width), static_cast<int>(height)}, m_outlineColor(outlineColor) {
    }

    Rectangle::Rectangle(int left, int top, size_t width, size_t height, const SDL_Color& outlineColor,
                         const SDL_Color& fillColor)
        : m_rect{left, top, static_cast<int>(width), static_cast<int>(height)}, m_outlineColor(outlineColor),
          m_fill(true), m_fillColor(fillColor) {
    }

    void Rectangle::render(SDL_Renderer* renderer) const {
        if (m_fill) {
            SDL_SetRenderDrawColor(renderer, m_fillColor.r, m_fillColor.g, m_fillColor.b, m_fillColor.a);
            SDL_RenderFillRect(renderer, &m_rect);
        }
        SDL_SetRenderDrawColor(renderer, m_outlineColor.r, m_outlineColor.g, m_outlineColor.b, m_outlineColor.a);
        {
            SDL_Rect outlineRect = {m_rect.x, m_rect.y, m_rect.w, static_cast<int>(m_lineThickness)};
            SDL_RenderFillRect(renderer, &outlineRect);
            outlineRect.y += m_rect.h - m_lineThickness;
            SDL_RenderFillRect(renderer, &outlineRect);
        }
        {
            SDL_Rect outlineRect = {m_rect.x, m_rect.y, static_cast<int>(m_lineThickness), m_rect.h};
            SDL_RenderFillRect(renderer, &outlineRect);
            outlineRect.x += m_rect.w - m_lineThickness;
            SDL_RenderFillRect(renderer, &outlineRect);
        }
    }

    void Rectangle::setRect(int left, int top, int width, int height) {
        m_rect = {left, top, width, height};
    }

    void Rectangle::setFillColor(SDL_Color color) {
        m_fillColor = color;
        m_fill      = true;
    }

    void Rectangle::setLineThickNess(size_t thickness) {
        assert(thickness > 0);
        m_lineThickness = thickness;
    }
} // namespace view