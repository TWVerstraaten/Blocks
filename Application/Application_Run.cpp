//
// Created by pc on 22-12-20.
//

#include "Application_Run.h"

#include "../view/Mouse.h"
#include "Application_Edit.h"

#include <cassert>

Application_Run::Application_Run(const model::Model& model, view::View* view) : m_model(model), m_view(view) {
}

Application_Run::EXIT_CODE Application_Run::run() {
    m_timeSinceLastStep = 0;
    m_previousTime      = SDL_GetTicks();
    while (true) {
        if (m_timeSinceLastStep > m_timeStep) {
            performStep();
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
        if (m_runningMode != RUNNING_MODE::RUNNING) {
            return runningModeToExitCode();
        }

        if (not m_paused) {
            const auto dt = SDL_GetTicks() - m_previousTime;
            update(1.3 * dt / m_timeStep);
            m_timeSinceLastStep += dt;
        }
        m_previousTime = SDL_GetTicks();
        m_view->draw(m_model);
        SDL_RenderPresent(m_view->renderer());
    }
}

void Application_Run::mouseWheelEvent() {
    m_view->zoom(m_event.wheel.y);
}

void Application_Run::keyEvent() {
    if (m_event.type == SDL_KEYDOWN) {
        switch (m_event.key.keysym.sym) {
            case SDLK_ESCAPE:
                m_runningMode = RUNNING_MODE::GIVE_UP;
                break;
            case SDLK_SPACE:
                togglePause();
                break;
            case SDLK_TAB:
                m_paused             = false;
                m_pauseAfterNextStep = true;
                break;
            case SDLK_1:
                setTimeStep(global::m_timeStepSlow);
                break;
            case SDLK_2:
                setTimeStep(global::m_timeStepMedium);
                break;
            case SDLK_3:
                setTimeStep(global::m_timeStepFast);
                break;
            default:
                break;
        }
    }
}

void Application_Run::mouseClickEvent() {
    const auto mousePosition = Mouse::getMouseXY();

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
        const auto mouseXY = Mouse::getMouseXY();
        m_view->translate((mouseXY.x - m_previousMousePosition.x), mouseXY.y - m_previousMousePosition.y);
        m_previousMousePosition = mouseXY;
    }
}

void Application_Run::update(double fractionOfPhase) {
    m_model.update(fractionOfPhase);
    m_model.interactClustersWithLevel();
}

void Application_Run::setTimeStep(Uint32 timeStep) {
    m_paused   = false;
    m_timeStep = timeStep;
}

void Application_Run::togglePause() {
    m_paused = !m_paused;
}

void Application_Run::performStep() {
    m_model.interactClustersWithInstantBlocks();
    m_model.interactClustersWithDynamicBlocks();

    auto actionEditIt = m_view->actionEditBoxes().begin();
    for (auto& cluster : m_model.clusters()) {
        actionEditIt->setHighLightedLine(cluster.currentActionIndex());
        ++actionEditIt;
    }

    if (m_pauseAfterNextStep) {
        m_pauseAfterNextStep = false;
        m_paused             = true;
    }
    m_timeSinceLastStep %= m_timeStep;
}

Application_Run::EXIT_CODE Application_Run::runningModeToExitCode() const {
    assert(m_runningMode != RUNNING_MODE::RUNNING);
    switch (m_runningMode) {
        case RUNNING_MODE::QUIT:
            return EXIT_CODE::QUIT;
        case RUNNING_MODE::COMPLETED:
            return EXIT_CODE::COMPLETED;
        case RUNNING_MODE::FAILED:
            return EXIT_CODE::FAILED;
        case RUNNING_MODE::GIVE_UP:
            return EXIT_CODE::GIVE_UP;
        default:
            return EXIT_CODE::QUIT;
    }
}
