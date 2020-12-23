//
// Created by pc on 16-12-20.
//

#include "RectWidget.h"

namespace view {
    RectWidget::RectWidget(SDL_Rect rect) : m_rect(rect) {
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

} // namespace view