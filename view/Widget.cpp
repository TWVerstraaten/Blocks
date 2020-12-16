//
// Created by pc on 16-12-20.
//

#include "Widget.h"
namespace view {
    Widget::Widget(SDL_Rect rect) : m_rect(rect) {
    }

    void Widget::getFocus() {
        m_hasFocus = true;
    }

    void Widget::loseFocus() {
        m_hasFocus = false;
    }

    bool Widget::pointIsOverWidget(const SDL_Point& point) const {
        return SDL_PointInRect(&point, &m_rect);
    }

    bool Widget::hasFocus() const {
        return m_hasFocus;
    }

} // namespace view