//
// Created by pc on 22-12-20.
//

#include "Application_Run.h"

#include "../view/Mouse.h"
#include "../view/ScreenXY.h"

Application_Run::Application_Run(const model::Model& model, view::View* view) : m_view(view) {
    m_model             = model;
    m_timeSinceLastStep = 0;
    m_previousTime      = SDL_GetTicks();
    m_view->initActionBoxes(m_model.clusters());
}

void Application_Run::mouseWheelEvent(const SDL_Event& event) {
    m_view->zoom(event.wheel.y);
}

void Application_Run::keyEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                m_runningMode = Application_Level::RUN_MODE::GIVE_UP;
                break;
            case SDLK_SPACE:
                togglePause();
                break;
            case SDLK_TAB:
                m_paused             = false;
                m_pauseAfterNextStep = true;
                break;
            case SDLK_1:
                setTimeStep(cst::m_timeStepSlow);
                break;
            case SDLK_2:
                setTimeStep(cst::m_timeStepMedium);
                break;
            case SDLK_3:
                setTimeStep(cst::m_timeStepFast);
                break;
            default:
                break;
        }
    }
}

void Application_Run::mouseClickEvent(const SDL_Event& event) {
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
}

void Application_Run::mouseReleaseEvent(const SDL_Event& event) {
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

void Application_Run::mouseMoveEvent(const SDL_Event& event) {
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

void Application_Run::performTimeStep() {
    if (not m_currentStepIsFirst) {
        m_model.preStep();
    }
    m_model.interactClustersWithInstantBlocks();
    m_model.interactClustersWithDynamicBlocks();
    m_view->updateActionBoxes(m_model.clusters());
    if (m_pauseAfterNextStep) {
        m_pauseAfterNextStep = false;
        m_paused             = true;
    }
    m_timeSinceLastStep %= m_timeStep;
    m_currentStepIsFirst = false;
}

Application_Level::RUN_MODE Application_Run::performSingleLoop() {
    if (m_runningMode != Application_Level::RUN_MODE::RUNNING) {
        return m_runningMode;
    }
    if (m_timeSinceLastStep > m_timeStep) {
        performTimeStep();
    }

    const auto dt = SDL_GetTicks() - m_previousTime;
    if (not m_paused) {
        update(1.3 * dt / m_timeStep);
        m_timeSinceLastStep += dt;
    }
    m_previousTime = SDL_GetTicks();
    m_view->updateActionBoxes(m_model.clusters());
    m_view->draw(m_model);
    m_view->assets().renderText(std::to_string(1000.0 / dt), view::ScreenXY{10, m_view->windowSize().y - 40}, m_view->renderer());
    m_view->renderPresent();

    return Application_Level::RUN_MODE::RUNNING;
}

void Application_Run::handleEvent(const SDL_Event& event) {
    switch (event.type) {
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
        case SDL_MOUSEWHEEL: {
            mouseWheelEvent(event);
        }
    }
}

void Application_Run::setPauseAfterNextStep(bool pauseAfterNextStep) {
    m_pauseAfterNextStep = pauseAfterNextStep;
}
