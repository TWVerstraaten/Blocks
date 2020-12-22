//
// Created by pc on 22-12-20.
//

#include "Application_Edit.h"

#include <algorithm>
#include <cassert>

Application_Edit::Application_Edit(view::View* view) : m_view(view) {
    init();
}

Application_Edit::EXIT_CODE Application_Edit::loop() {
    SDL_StartTextInput();
    while (true) {
        while (SDL_PollEvent(&m_event) > 0) {
            switch (m_event.type) {
                case SDL_QUIT:
                    return EXIT_CODE::QUIT;
                case SDL_TEXTINPUT:
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    keyEvent();
                    break;
                case SDL_WINDOWEVENT:
                    if (m_event.window.event == SDL_WINDOWEVENT_RESIZED) {}
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mouseClickEvent();
                    break;
                case SDL_MOUSEBUTTONUP:
                    mouseReleaseEvent();
                    break;
                case SDL_MOUSEMOTION:
                    mouseMoveEvent();
                    break;
                case SDL_MOUSEWHEEL: {
                    mouseWheelEvent();
                }
            }
        }
        if (m_runningMode != RUNNING_MODE::RUNNING) {
            break;
        }
        m_view->draw(m_model);
        SDL_RenderPresent(m_view->renderer());
    }
    SDL_StopTextInput();
    assert(m_runningMode != RUNNING_MODE::RUNNING);
    switch (m_runningMode) {
        case RUNNING_MODE::QUIT:
            return EXIT_CODE::QUIT;
        case RUNNING_MODE::DONE_EDITING:
            return EXIT_CODE::DONE_EDITING;
        default:
            return EXIT_CODE::QUIT;
    }
}

void Application_Edit::mouseWheelEvent() {
    m_view->zoom(m_event.wheel.y);
}

void Application_Edit::keyEvent() {
    switch (m_event.type) {
        case SDL_TEXTINPUT:
            for (auto& actionEditBox : m_view->actionEditBoxes()) {
                if (actionEditBox.hasFocus()) {
                    actionEditBox.handleKeyEvent(m_event);
                    return;
                }
            }
            break;
        case SDL_KEYDOWN:
            if (m_pressedKeys.find(m_event.key.keysym.sym) != m_pressedKeys.end()) {
                if (m_event.key.keysym.sym != SDLK_DELETE) {
                    return;
                }
            }
            m_pressedKeys.insert(m_event.key.keysym.sym);
            for (auto& actionEditBox : m_view->actionEditBoxes()) {
                if (actionEditBox.hasFocus()) {
                    actionEditBox.handleKeyEvent(m_event);
                    return;
                }
            }

            switch (m_event.key.keysym.sym) {
                case SDLK_SPACE:
                    if (canStart()) {
                        m_runningMode = RUNNING_MODE::DONE_EDITING;
                    }
                    break;
                case SDLK_1:
                    m_runningMode = RUNNING_MODE::DONE_EDITING;
                    break;
                case SDLK_2:
                    m_runningMode = RUNNING_MODE::DONE_EDITING;
                    break;
                case SDLK_3:
                    m_runningMode = RUNNING_MODE::DONE_EDITING;
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            assert(m_pressedKeys.find(m_event.key.keysym.sym) != m_pressedKeys.end());
            m_pressedKeys.erase(m_pressedKeys.find(m_event.key.keysym.sym));
            break;
    }
}

SDL_Point Application_Edit::getMouseCoordinates() {
    int xMouse, yMouse;
    SDL_GetMouseState(&xMouse, &yMouse);
    return {xMouse, yMouse};
}

void Application_Edit::mouseClickEvent() {
    const auto mousePosition = getMouseCoordinates();

    for (auto& actionEditBox : m_view->actionEditBoxes()) {
        if (not actionEditBox.pointIsOverWidget(mousePosition)) {
            actionEditBox.loseFocus();
        }
    }
    for (auto& actionEditBox : m_view->actionEditBoxes()) {
        if (actionEditBox.pointIsOverWidget(mousePosition)) {
            actionEditBox.getFocus();
            actionEditBox.handleMouseClickEvent(m_event, m_leftMouseButtonPressed);
            break;
        }
    }

    switch (m_event.button.button) {
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
}

void Application_Edit::mouseReleaseEvent() {
    switch (m_event.button.button) {
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

void Application_Edit::mouseMoveEvent() {
    const auto mousePosition = getMouseCoordinates();
    bool       isDone        = false;
    for (auto& actionEditBox : m_view->actionEditBoxes()) {
        if (actionEditBox.hasFocus()) {
            actionEditBox.handleMouseMoveEvent(mousePosition, m_leftMouseButtonPressed);
            isDone = true;
            break;
        }
    }
    if (not isDone) {
        if (m_rightMouseButtonPressed) {
            const auto mouseCoordinates = getMouseCoordinates();
            m_view->translate((mouseCoordinates.x - m_previousMousePosition.x), mouseCoordinates.y - m_previousMousePosition.y);
            m_previousMousePosition = mouseCoordinates;
        }
    }
}

void Application_Edit::init() {
    m_view->clear();
    m_model.init();
    for (auto& cluster : m_model.clusters()) {
        m_view->addActionEditBox(cluster);
    }
}

bool Application_Edit::canStart() const {
    return std::all_of(m_view->actionEditBoxes().begin(), m_view->actionEditBoxes().end(), [](const view::widget::ActionEditBox& box) {
        return box.canParse();
    });
}

const model::Model& Application_Edit::model() const {
    return m_model;
}
