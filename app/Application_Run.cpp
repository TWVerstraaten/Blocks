//
// Created by pc on 22-12-20.
//

#include "Application_Run.h"

#include "../view/Mouse.h"
#include "ModelViewInterface.h"

namespace app {
    Application_Run::Application_Run(const model::Model& model, view::View* view, const view::widget::ScrollArea& scrollArea)
        : m_view(view), m_scrollArea(scrollArea) {
        m_model             = model;
        m_timeSinceLastStep = 0;
        m_previousTime      = SDL_GetTicks();
    }

    void Application_Run::mouseWheelEvent(const SDL_Event& event) {
        m_view->zoom(event.wheel.y);
    }

    void Application_Run::keyEvent(const SDL_Event& event) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    m_runningMode = RUN_MODE::GIVE_UP;
                    break;
                case SDLK_SPACE:
                    togglePause();
                    break;
                case SDLK_TAB:
                    m_paused             = false;
                    m_pauseAfterNextStep = true;
                    break;
                case SDLK_1:
                    setTimeStep(cst::TIME_STEP_SLOW);
                    break;
                case SDLK_2:
                    setTimeStep(cst::TIME_STEP_MEDIUM);
                    break;
                case SDLK_3:
                    setTimeStep(cst::TIME_STEP_FAST);
                    break;
                default:
                    break;
            }
        }
    }

    void Application_Run::mouseClickEvent(const SDL_Event& event) {
        switch (event.button.button) {
            case SDL_BUTTON_RIGHT:
                m_rightMouseButtonPressed = true;
                m_previousMousePosition   = view::Mouse::mouseXY();
                break;
            case SDL_BUTTON_LEFT:
                m_leftMouseButtonPressed = true;
                m_previousMousePosition  = view::Mouse::mouseXY();
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
            const auto mouseXY = view::Mouse::mouseXY();
            m_view->translate((mouseXY.x() - m_previousMousePosition.x()), mouseXY.y() - m_previousMousePosition.y());
            m_previousMousePosition = mouseXY;
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

    void Application_Run::performTimeStep() {
        m_model.startPhase();
        ModelViewInterface::interactWithInstantBlocks(m_model, m_scrollArea);
        ModelViewInterface::interactWithDynamicBlocks(m_model, m_scrollArea);
        for (auto& cluster : m_model.clusters()) {
            cluster.performPendingOperationOrNextCommand(m_model);
        }

        auto it = std::partition(m_model.clusters().begin(), m_model.clusters().end(), [](const auto& cluster) {
            return cluster.state() != model::CLUSTER_STATE::STOPPED;
        });

        m_model.level().stoppedClusters().splice(m_model.level().stoppedClusters().end(), m_model.clusters(), it, m_model.clusters().end());
        m_model.level().createBoundaries();
        m_model.finishInteractions();
        ModelViewInterface::updateCommandScrollArea(m_model, m_scrollArea, APP_MODE::RUNNING);
        if (m_pauseAfterNextStep) {
            m_pauseAfterNextStep = false;
            m_paused             = true;
        }
        m_timeSinceLastStep %= m_timeStep;
    }

    RUN_MODE Application_Run::performSingleLoop() {
        if (m_runningMode != RUN_MODE::RUNNING) {
            return m_runningMode;
        }
        if (m_timeSinceLastStep > m_timeStep) {
            performTimeStep();
        }

        const auto dt = SDL_GetTicks() - m_previousTime;
        if (not m_paused) {
            update(1.9 * dt / m_timeStep);
            m_timeSinceLastStep += dt;
        }
        m_previousTime = SDL_GetTicks();
        m_view->draw(m_model);
        m_view->drawScrollArea(&m_scrollArea);
        m_view->assets()->renderText(std::to_string(1000.0 / dt), view::ScreenXY{10, m_view->windowSize().y() - 40}, m_view->renderer());
        m_view->renderPresent();

        return RUN_MODE::RUNNING;
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

    view::widget::ScrollArea& Application_Run::scrollArea() {
        return m_scrollArea;
    }
} // namespace app