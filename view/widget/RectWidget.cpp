//
// Created by pc on 16-12-20.
//

#include "RectWidget.h"

#include <cassert>

namespace view { namespace widget {
    RectWidget::RectWidget(SDL_Rect rect) : m_rect(rect) {
        assert(m_rect.x >= -10);
        assert(m_rect.y >= -10);
        assert(m_rect.w <= 3200);
        assert(m_rect.h  <= 3200);
    }

    void RectWidget::getFocus() {
        m_hasFocus = true;
    }

    void RectWidget::loseFocus() {
        m_hasFocus = false;
    }

    bool RectWidget::pointIsOverWidget(const SDL_Point& point) const {
        return SDL_PointInRect(&point, &m_rect);
    }

    bool RectWidget::hasFocus() const {
        return m_hasFocus;
    }

    void RectWidget::setWidth(int width) {
        m_rect.w = width;
    }

    void RectWidget::setHeight(int height) {
        m_rect.h = height;
    }

    void RectWidget::setX(int x) {
        m_rect.x = x;
    }

    void RectWidget::setY(int y) {
        m_rect.y = y;
    }

    int RectWidget::height() const {
        return m_rect.h;
    }

} } // namespace view::widget