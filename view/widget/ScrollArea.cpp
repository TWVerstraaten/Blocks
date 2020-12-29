//
// Created by pc on 26-12-20.
//

#include "ScrollArea.h"

#include "../../global/cst.h"
#include "../../global/fns.h"
#include "../../global/geom.h"
#include "../../model/Cluster.h"
#include "../Mouse.h"
#include "../Rectangle.h"
#include "../color.h"

#include <algorithm>

view::widget::ScrollArea::ScrollArea(const SDL_Rect& rect) : RectWidget(rect) {
}

void view::widget::ScrollArea::loseFocus() {
    RectWidget::loseFocus();
    for (auto& w : m_children) {
        w.loseFocus();
    }
}

void view::widget::ScrollArea::keyEvent(const SDL_Event& event) {
    const auto w = focusedWidget();
    if (w) {
        w->keyEvent(event);
    }
}

void view::widget::ScrollArea::leftClickEvent(const SDL_Event& event) {
    assert(pointIsOverWidget(Mouse::mouseXY()));
    getFocus();
    for (auto& w : m_children) {
        w.loseFocus();
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
    update(renderer);

    Rectangle::render(geom::pad(m_rect, cst::LINE_EDIT_PADDING), view::color::SCROLL_AREA_BACKGROUND, renderer);

    for (auto& w : m_children) {
        w.render(renderer);
    }
    if (m_firstRender) {
        setHeightAndPositions();
        m_firstRender = false;
    }
    renderScrollBar(renderer);
}

void view::widget::ScrollArea::init(const view::Assets* assets) {
    assert(assets->initialized());
    m_assets = assets;
}

void view::widget::ScrollArea::setHeightAndPositions() {
    int yOffset = 2 * cst::LINE_EDIT_PADDING;
    for (const auto& w : m_children) {
        yOffset += w.height() + 3 * cst::LINE_EDIT_PADDING;
    }
    m_height = yOffset;
    yOffset  = 2 * cst::LINE_EDIT_PADDING;
    if (static_cast<int>(m_height) > m_rect.h) {
        const auto heightDifference = m_height - m_rect.h;
        yOffset -= heightDifference * m_scrollFraction;
    } else {
        m_scrollFraction = 0.0;
    }
    for (auto& w : m_children) {
        w.setY(yOffset);
        yOffset += w.height() + 3 * cst::LINE_EDIT_PADDING;
    }
}

void view::widget::ScrollArea::mouseWheelEvent(const SDL_Event& event) {
    assert(event.type == SDL_MOUSEWHEEL);
    m_scrollFraction -= 0.1 * event.wheel.y;
    m_scrollFraction = fns::clamp(m_scrollFraction, 0.0, 1.0);
    setHeightAndPositions();
}

view::widget::CommandEditBox* view::widget::ScrollArea::widgetUnderMouse() {
    auto mousePosition = Mouse::mouseXY();
    for (auto& w : m_children) {
        if (w.pointIsOverWidget(mousePosition)) {
            return &w;
        }
    }

    return nullptr;
}

view::widget::CommandEditBox* view::widget::ScrollArea::focusedWidget() {
    CommandEditBox* result = nullptr;
    for (auto& w : m_children) {
        if (w.hasFocus()) {
            return &w;
        }
    }
    return result;
}

void view::widget::ScrollArea::update(SDL_Renderer* renderer) {
    for (auto& w : m_children) {
        w.update(renderer);
    }
    setHeightAndPositions();
    m_needsUpdate = false;
}

void view::widget::ScrollArea::addCommandEditBox(const model::Cluster& cluster) {
    m_children.emplace_back(
        CommandEditBox(m_rect.x + cst::LINE_EDIT_PADDING, 0, cst::LINE_EDIT_WIDTH, 0, m_assets, cluster));
    m_children.back().setHighLightedLine(cluster.commandIndex());
    m_children.back().setActive(cluster.isAlive());
    m_needsUpdate = true;
}

std::list<view::widget::CommandEditBox>& view::widget::ScrollArea::children() {
    return m_children;
}

void view::widget::ScrollArea::renderScrollBar(SDL_Renderer* renderer) {
    assert(m_rect.y == 0);
    Rectangle::render({m_rect.x + m_rect.w - 10, 0, 3, m_rect.h}, view::color::WHITE, renderer);
    const int heightDifference = m_height - m_rect.h;
    if (heightDifference <= 0) {
        Rectangle::render({m_rect.x + m_rect.w - 10, 2, 5, m_rect.h - 4}, view::color::BLACK, renderer);
    } else {
        const int barHeight = m_rect.h * (m_rect.h / static_cast<double>(m_height));
        Rectangle::render(
            {m_rect.x + m_rect.w - 10, static_cast<int>(m_scrollFraction * (m_rect.h - barHeight)), 5, barHeight},
            view::color::BLACK,
            renderer);
    }
}

view::widget::ScrollArea::ScrollArea(const view::widget::ScrollArea& other)
    : RectWidget(other.m_rect), m_children(other.m_children) {
    m_rect           = other.m_rect;
    m_needsUpdate    = other.m_needsUpdate;
    m_firstRender    = other.m_firstRender;
    m_scrollFraction = other.m_scrollFraction;
    m_height         = other.m_height;
    m_assets         = other.m_assets;
}

void view::widget::ScrollArea::setHeight(int height) {
    RectWidget::setHeight(height);
}

void view::widget::ScrollArea::setX(int x) {
    RectWidget::setX(x);
    for (auto& w : m_children) {
        w.setX(m_rect.x + cst::LINE_EDIT_PADDING);
    }
}

std::list<view::widget::CommandEditBox>::iterator view::widget::ScrollArea::findCommandEditBox(size_t index) {
    return std::find_if(m_children.begin(), m_children.end(), [index](const auto& commandEditBox) {
        return commandEditBox.index() == index;
    });
}
