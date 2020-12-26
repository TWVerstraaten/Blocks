//
// Created by pc on 26-12-20.
//

#include "ScrollArea.h"

#include "../../global/cst.h"
#include "../../global/fns.h"
#include "../../global/geom.h"
#include "../Mouse.h"
#include "../Rectangle.h"
#include "LineEditBox.h"

#include <cassert>

view::widget::ScrollArea::ScrollArea(const SDL_Rect& rect) : RectWidget(rect) {
}

void view::widget::ScrollArea::loseFocus() {
    RectWidget::loseFocus();
}

void view::widget::ScrollArea::keyEvent(const SDL_Event& event) {
    const auto w = focusedWidget();
    if (w) {
        w->keyEvent(event);
    }
}

void view::widget::ScrollArea::leftClickEvent(const SDL_Event& event) {
    assert(pointIsOverWidget(Mouse::getMouseXY()));
    getFocus();
    for (auto& w : m_children) {
        w->loseFocus();
    }

    const auto w = widgetUnderMouse();
    if (w) {
        w->leftClickEvent(event);
        w->getFocus();
    }
}

void view::widget::ScrollArea::mouseDragEvent(const SDL_Event& event) {
    const auto w = focusedWidget();
    if (w) {
        w->mouseDragEvent(event);
    }
}

void view::widget::ScrollArea::render(SDL_Renderer* renderer) {
    if (m_needsUpdate) {
        update(renderer);
    }
    Rectangle::render(geom::pad(m_rect, cst::LINE_EDIT_PADDING), cst::color::SCROLL_AREA_BACKGROUND, renderer);

    for (const auto& w : m_children) {
        w->render(renderer);
    }
    if (m_firstRender) {
        setHeightAndPositions();
        m_firstRender = false;
    }
}

void view::widget::ScrollArea::init(const view::Assets* assets) {
    m_assets = assets;
    for (size_t i = 0; i != 10; ++i) {
        m_children.emplace_back(new view::widget::LineEditBox(m_rect.x + 10, 0, m_rect.w - 20, 0, assets, "widget " + std::to_string(i)));
        dynamic_cast<view::widget::LineEditBox*>(m_children.back().get())->appendString("AAA");
        dynamic_cast<view::widget::LineEditBox*>(m_children.back().get())->appendString("BBB");
        dynamic_cast<view::widget::LineEditBox*>(m_children.back().get())->appendString("CCC");
    }
    setHeightAndPositions();
}

void view::widget::ScrollArea::setHeightAndPositions() {
    int yOffset = 10;
    for (const auto& w : m_children) {
        yOffset += w->height() + 3 * cst::LINE_EDIT_PADDING;
    }
    m_height = yOffset;
    yOffset  = 10;
    if (static_cast<int>(m_height) > m_rect.h) {
        const auto heightDifference = m_height - m_rect.h;
        yOffset -= heightDifference * m_scrollFraction;
    } else {
        m_scrollFraction = 0.0;
    }
    for (const auto& w : m_children) {
        w->setY(yOffset);
        yOffset += w->height() + 3 * cst::LINE_EDIT_PADDING;
    }
}

void view::widget::ScrollArea::mouseWheelEvent(const SDL_Event& event) {
    assert(event.type == SDL_MOUSEWHEEL);
    m_scrollFraction -= 0.1 * event.wheel.y;
    m_scrollFraction = fns::clamp(m_scrollFraction, 0.0, 1.0);
    setHeightAndPositions();
}

view::widget::RectWidget* view::widget::ScrollArea::widgetUnderMouse() const {
    auto mousePosition = Mouse::getMouseXY();
    for (const auto& w : m_children) {
        if (w->pointIsOverWidget(mousePosition)) {
            return w.get();
        }
    }

    return nullptr;
}

view::widget::RectWidget* view::widget::ScrollArea::focusedWidget() const {
    RectWidget* result = nullptr;
    for (const auto& w : m_children) {
        if (w->hasFocus()) {
            assert(result == nullptr);
            result = w.get();
        }
    }
    return result;
}

void view::widget::ScrollArea::update(SDL_Renderer* renderer) {
    for (auto& w : m_children) {
        w->update(renderer);
    }
    setHeightAndPositions();
}
