//
// Created by pc on 22-12-20.
//

#include "Application_Edit.h"

#include "../view/Mouse.h"
#include "../view/ScreenXY.h"

#include <algorithm>
#include <cassert>

Application_Edit::Application_Edit(model::Model* model, view::View* view) : m_view(view), m_model(model) {
    init();
}

void Application_Edit::mouseWheelEvent(const SDL_Event& event) {
    m_view->zoom(event.wheel.y);
}

void Application_Edit::keyEvent(const SDL_Event& event) {
    if (m_focusedWidget) {
        m_focusedWidget->keyEvent(event);
    }
}

void Application_Edit::setFocusOnClick() {
    m_focusedWidget          = nullptr;
    const auto mousePosition = Mouse::getMouseXY();
    for (auto& actionEditBox : m_view->actionEditBoxes()) {
        if (not actionEditBox.pointIsOverWidget(mousePosition)) {
            actionEditBox.loseFocus();
        } else {
            actionEditBox.getFocus();
            m_focusedWidget = &actionEditBox;
        }
    }
}

void Application_Edit::mouseClickEvent(const SDL_Event& event) {
    const auto mousePosition = Mouse::getMouseXY();
    switch (event.button.button) {
        case SDL_BUTTON_RIGHT:
            m_rightMouseButtonPressed = true;
            m_previousMousePosition   = mousePosition;
            break;
        case SDL_BUTTON_LEFT:
            m_leftMouseButtonPressed = true;
            m_previousMousePosition  = mousePosition;
            break;
        default:
            break;
    }

    setFocusOnClick();
    if (m_focusedWidget) {
        if (m_leftMouseButtonPressed) {
            m_focusedWidget->leftClickEvent(event);
        }
    }
}

void Application_Edit::mouseReleaseEvent(const SDL_Event& event) {
    switch (event.button.button) {
        case SDL_BUTTON_RIGHT:
            m_rightMouseButtonPressed = false;
            break;
        case SDL_BUTTON_LEFT:
            m_leftMouseButtonPressed = false;
            break;
        default:
            break;
    }
}

void Application_Edit::mouseMoveEvent(const SDL_Event& event) {
    if (m_focusedWidget) {
        if (m_leftMouseButtonPressed) {
            m_focusedWidget->mouseDragEvent(event);
        }
    }
    if (m_rightMouseButtonPressed) {
        const auto mouseXY = Mouse::getMouseXY();
        m_view->translate((mouseXY.x - m_previousMousePosition.x), mouseXY.y - m_previousMousePosition.y);
        m_previousMousePosition = mouseXY;
    }
}

void Application_Edit::init() {
    SDL_StartTextInput();
    m_view->clear();
    m_view->setActionEditBoxes(m_model->clusters());
}

bool Application_Edit::canStart() const {
    return std::all_of(m_view->actionEditBoxes().begin(), m_view->actionEditBoxes().end(), [](const view::widget::LineEditBox& box) {
        return box.canParse();
    });
}

void Application_Edit::getActionsFromEditBoxes() {
    assert(m_model->clusters().size() == m_view->actionEditBoxes().size());
    auto actionEditIt = m_view->actionEditBoxes().begin();
    for (auto& cluster : m_model->clusters()) {
        actionEditIt->updateClusterActions(cluster);
        ++actionEditIt;
    }
}

Uint32 Application_Edit::timeStep() const {
    return m_timeStep;
}

void Application_Edit::finalize() {
    getActionsFromEditBoxes();
    SDL_StopTextInput();
}

void Application_Edit::handleEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_TEXTINPUT:
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            keyEvent(event);
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {}
            break;
        case SDL_MOUSEBUTTONDOWN:
            mouseClickEvent(event);
            break;
        case SDL_MOUSEBUTTONUP:
            mouseReleaseEvent(event);
            break;
        case SDL_MOUSEMOTION:
            mouseMoveEvent(event);
            break;
        case SDL_MOUSEWHEEL:
            mouseWheelEvent(event);
            break;
        default:
            break;
    }
}

Application_Level::EDIT_MODE Application_Edit::performSingleLoop() {
    if (m_editMode != Application_Level::EDIT_MODE::EDITING) {
        finalize();
    }
    m_view->draw(*m_model);
    m_view->renderPresent();
    return m_editMode;
}

bool Application_Edit::hasFocus() {
    return m_focusedWidget == nullptr;
}
