//
// Created by pc on 15-12-20.
//

#include "Application.h"

#include <cassert>

Application::Application() {
    m_lastTime          = SDL_GetTicks();
    m_timeSinceLastStep = 0;

    for (const auto& cluster : m_model.clusters()) {
        m_view.addActionEditBox(cluster);
    }
}

void Application::loop() {
    bool isRunning = true;
    SDL_StartTextInput();
    while (isRunning) {
        if (m_timeSinceLastStep > m_stepTimeInMilliSeconds) {
            m_model.interactClustersWithInstantBlocks();
            m_model.interactClustersWithDynamicBlocks();
            m_timeSinceLastStep %= m_stepTimeInMilliSeconds;
        }
        while (SDL_PollEvent(&m_event) > 0) {
            switch (m_event.type) {
                case SDL_QUIT:
                    isRunning = false;
                    break;
                case SDL_TEXTEDITING:
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
        const auto dt = SDL_GetTicks() - m_lastTime;
        if (not m_isPaused) {
            update(static_cast<double>(1.1 * dt) / m_stepTimeInMilliSeconds);
        }
        m_lastTime = SDL_GetTicks();
        m_view.draw(m_model);
        if (!m_isPaused) {
            m_timeSinceLastStep += dt;
        }
    }
    SDL_StopTextInput();
}

void Application::update(double fractionOfPhase) {
    m_model.update(fractionOfPhase);
}

void Application::mouseWheelEvent() {
    m_view.zoom(m_event.wheel.y);
}

void Application::keyEvent() {
    switch (m_event.type) {
        case SDL_TEXTINPUT:
            for (auto& actionEditBox : m_view.actionEditBoxes()) {
                if (actionEditBox->hasFocus()) {
                    actionEditBox->handleKeyEvent(m_event);
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
            for (auto& actionEditBox : m_view.actionEditBoxes()) {
                if (actionEditBox->hasFocus()) {
                    actionEditBox->handleKeyEvent(m_event);
                    return;
                }
            }

            switch (m_event.key.keysym.sym) {
                case SDLK_SPACE:
                    m_isPaused = !m_isPaused;
                    break;
                case SDLK_1:
                    setTimeStep(1000);
                    break;
                case SDLK_2:
                    setTimeStep(300);
                    break;
                case SDLK_3:
                    setTimeStep(100);
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

void Application::mouseClickEvent() {
    const auto mousePosition = getMouseCoordinates();

    for (auto& actionEditBox : m_view.actionEditBoxes()) {
        actionEditBox->loseFocus();
    }

    for (auto& actionEditBox : m_view.actionEditBoxes()) {
        if (actionEditBox->pointIsOverWidget(mousePosition)) {
            actionEditBox->getFocus();
            actionEditBox->handleMouseClickEvent(m_event);
            return;
        }
    }

    switch (m_event.button.button) {
        case SDL_BUTTON_RIGHT:
            m_rightMouseButtonPressed = true;
            m_previousMousePosition   = mousePosition;
            break;
        default:
            break;
    }
}

void Application::mouseReleaseEvent() {
    switch (m_event.button.button) {
        case SDL_BUTTON_RIGHT:
            m_rightMouseButtonPressed = false;
            break;
        default:
            break;
    }
}

void Application::mouseMoveEvent() {
    if (m_rightMouseButtonPressed) {
        const auto mouseCoordinates = getMouseCoordinates();
        m_view.translate((mouseCoordinates.x - m_previousMousePosition.x),
                         mouseCoordinates.y - m_previousMousePosition.y);
        m_previousMousePosition = mouseCoordinates;
    }
}

SDL_Point Application::getMouseCoordinates() {
    int xMouse, yMouse;
    SDL_GetMouseState(&xMouse, &yMouse);
    return {xMouse, yMouse};
}

void Application::setTimeStep(Uint32 timeStep) {
    m_stepTimeInMilliSeconds = timeStep;
    m_isPaused               = false;
}
