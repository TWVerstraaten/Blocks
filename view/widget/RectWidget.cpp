//
// Created by pc on 16-12-20.
//

#include "RectWidget.h"

namespace view::widget {
    RectWidget::RectWidget(SDL_Rect rect) : m_rect(rect) {
    }

    void RectWidget::getFocus() {
        m_hasFocus = true;
    }

    void RectWidget::loseFocus() {
        m_hasFocus = false;
    }

    bool RectWidget::pointIsOverWidget(const ScreenXY& point) const {
        return ((point.x() >= m_rect.x) && (point.x() < (m_rect.x + m_rect.w)) && (point.y() >= m_rect.y) && (point.y() < (m_rect.y + m_rect.h)));
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

} // namespace view::widget