//
// Created by pc on 22-12-20.
//

#include "Application_Run.h"

#include "Application_Level.h"

#include <cassert>

Application_Run::Application_Run(const model::Model& model, view::View* view) : m_model(model), m_view(view) {
}

Application_Run::EXIT_CODE Application_Run::loop() {
    while (true) {
        if (m_timeSinceLastStep > m_timeStep) {
            m_model.interactClustersWithInstantBlocks();
            m_model.interactClustersWithDynamicBlocks();
            m_timeSinceLastStep %= m_timeStep;
        }
        while (SDL_PollEvent(&m_event) > 0) {
            switch (m_event.type) {
                case SDL_QUIT:
                    return EXIT_CODE::QUIT;
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
        const auto dt = SDL_GetTicks() - m_previousTime;
        if (not m_paused) {
            update(static_cast<double>(1.9 * dt) / m_timeStep);
            m_model.interactClustersWithLevel();
        }
        m_previousTime = SDL_GetTicks();
        m_view->draw(m_model);
        if (!m_paused) {
            m_timeSinceLastStep += dt;
        }
        if (m_runningMode != RUNNING_MODE::RUNNING) {
            break;
        }
        SDL_RenderPresent(m_view->renderer());
    }

    switch (m_runningMode) {
        case RUNNING_MODE::QUIT:
            return EXIT_CODE::QUIT;
        case RUNNING_MODE::COMPLETED:
            return EXIT_CODE::COMPLETED;
        case RUNNING_MODE::FAILED:
            return EXIT_CODE::FAILED;
        default:
            return EXIT_CODE::QUIT;
    }
}

void Application_Run::mouseWheelEvent() {
    m_view->zoom(m_event.wheel.y);
}

void Application_Run::keyEvent() {
    switch (m_event.type) {
        case SDL_KEYDOWN:
            if (m_pressedKeys.find(m_event.key.keysym.sym) != m_pressedKeys.end()) {
                if (m_event.key.keysym.sym != SDLK_DELETE) {
                    return;
                }
            }
            m_pressedKeys.insert(m_event.key.keysym.sym);

            switch (m_event.key.keysym.sym) {
                case SDLK_ESCAPE:

                    break;
                case SDLK_SPACE:
                    togglePause();
                    break;
                case SDLK_1:
                    setTimeStep(1000);
                    break;
                case SDLK_2:
                    setTimeStep(300);
                    break;
                case SDLK_3:
                    setTimeStep(50);
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            if (m_pressedKeys.find(m_event.key.keysym.sym) != m_pressedKeys.end()) {
                m_pressedKeys.erase(m_pressedKeys.find(m_event.key.keysym.sym));
            }
            break;
    }
}

void Application_Run::mouseClickEvent() {
    const auto mousePosition = Application_Level::getMouseCoordinates();

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

void Application_Run::mouseReleaseEvent() {
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

void Application_Run::mouseMoveEvent() {
    if (m_rightMouseButtonPressed) {
        const auto mouseCoordinates = Application_Level::getMouseCoordinates();
        m_view->translate((mouseCoordinates.x - m_previousMousePosition.x), mouseCoordinates.y - m_previousMousePosition.y);
        m_previousMousePosition = mouseCoordinates;
    }
}

void Application_Run::update(double fractionOfPhase) {
    m_model.update(fractionOfPhase);
}

void Application_Run::setTimeStep(Uint32 timeStep) {
    m_paused   = false;
    m_timeStep = timeStep;
}

void Application_Run::togglePause() {
    m_paused = !m_paused;
}
