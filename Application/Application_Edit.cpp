//
// Created by pc on 22-12-20.
//

#include "Application_Edit.h"

#include "../view/Mouse.h"

#include <algorithm>
#include <cassert>

Application_Edit::Application_Edit(model::Model* model, view::View* view) : m_model(model), m_view(view) {
}

Application_Edit::EXIT_CODE Application_Edit::run() {
    init();
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
                case SDL_MOUSEWHEEL:
                    mouseWheelEvent();
                    break;
                default:
                    break;
            }
        }
        if (m_runningMode != RUNNING_MODE::RUNNING) {
            return finalizeAndReturnExitCode();
        }
        m_view->draw(*m_model);
        SDL_RenderPresent(m_view->renderer());
    }
}

void Application_Edit::mouseWheelEvent() {
    m_view->zoom(m_event.wheel.y);
}

void Application_Edit::keyEvent() {
    if (m_focusedWidget) {
        m_focusedWidget->keyEvent(m_event);
    } else {
        if (m_event.type == SDL_KEYDOWN) {
            switch (m_event.key.keysym.sym) {
                case SDLK_SPACE:
                    if (canStart()) {
                        m_runningMode = RUNNING_MODE::DONE_EDITING;
                    }
                    break;
                case SDLK_1:
                    m_timeStep    = global::m_timeStepSlow;
                    m_runningMode = RUNNING_MODE::DONE_EDITING;
                    break;
                case SDLK_2:
                    m_timeStep    = global::m_timeStepMedium;
                    m_runningMode = RUNNING_MODE::DONE_EDITING;
                    break;
                case SDLK_3:
                    m_timeStep    = global::m_timeStepFast;
                    m_runningMode = RUNNING_MODE::DONE_EDITING;
                    break;
                default:
                    break;
            }
        }
    }
}

void Application_Edit::setFocusOnClick() {
    m_focusedWidget          = nullptr;
    const auto mousePosition = Mouse::getMouseCoordinates();
    for (auto& actionEditBox : m_view->actionEditBoxes()) {
        if (not actionEditBox.pointIsOverWidget(mousePosition)) {
            actionEditBox.loseFocus();
        } else {
            actionEditBox.getFocus();
            m_focusedWidget = &actionEditBox;
        }
    }
}

void Application_Edit::mouseClickEvent() {
    const auto mousePosition = Mouse::getMouseCoordinates();
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

    setFocusOnClick();
    if (m_focusedWidget) {
        if (m_leftMouseButtonPressed) {
            m_focusedWidget->leftClickEvent(m_event);
        }
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
    if (m_focusedWidget) {
        if (m_leftMouseButtonPressed) {
            m_focusedWidget->mouseDragEvent(m_event);
        }
    }
    if (m_rightMouseButtonPressed) {
        const auto mouseCoordinates = Mouse::getMouseCoordinates();
        m_view->translate((mouseCoordinates.x - m_previousMousePosition.x), mouseCoordinates.y - m_previousMousePosition.y);
        m_previousMousePosition = mouseCoordinates;
    }
}

void Application_Edit::init() {
    m_view->clear();
    for (auto& cluster : m_model->clusters()) {
        m_view->addActionEditBox(cluster);
    }
}

bool Application_Edit::canStart() const {
    return std::all_of(m_view->actionEditBoxes().begin(), m_view->actionEditBoxes().end(), [](const view::widget::ActionEditBox& box) {
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

Application_Edit::EXIT_CODE Application_Edit::runningModeToExitCode() const {
    switch (m_runningMode) {
        case RUNNING_MODE::QUIT:
            return EXIT_CODE::QUIT;
        case RUNNING_MODE::DONE_EDITING:
            return EXIT_CODE::DONE_EDITING;
        default:
            return EXIT_CODE::QUIT;
    }
}

Application_Edit::EXIT_CODE Application_Edit::finalizeAndReturnExitCode() {
    assert(m_runningMode != RUNNING_MODE::RUNNING);
    if (m_runningMode == RUNNING_MODE::DONE_EDITING) {
        getActionsFromEditBoxes();
    }
    SDL_StopTextInput();
    return runningModeToExitCode();
}
