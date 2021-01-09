//
// Created by pc on 22-12-20.
//

#include "ApplicationRun.h"

#include "../global/defines.h"
#include "../view/Mouse.h"
#include "ModelViewInterface.h"

namespace app {
    ApplicationRun::ApplicationRun(const model::Model& model, view::View* view, const view::widget::ScrollArea& scrollArea)
        : m_view(view), m_scrollArea(scrollArea) {
        m_model             = model;
        m_timeSinceLastStep = 0;
        m_previousTime      = SDL_GetTicks();
    }

    void ApplicationRun::mouseWheelEvent(const SDL_Event& event) {
        m_view->zoom(event.wheel.y);
    }

    void ApplicationRun::keyEvent(const SDL_Event& event) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    m_runningMode = RUN_MODE::GIVE_UP;
                    break;
                case SDLK_SPACE:
                    togglePause();
                    break;
                case SDLK_1:
                    setTimeStep(TIME_STEP_SLOW);
                    break;
                case SDLK_2:
                    setTimeStep(TIME_STEP_MEDIUM);
                    break;
                case SDLK_3:
                    setTimeStep(TIME_STEP_FAST);
                    break;
                default:
                    break;
            }
        }
    }

    void ApplicationRun::mouseClickEvent(const SDL_Event& event) {
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

    void ApplicationRun::mouseReleaseEvent(const SDL_Event& event) {
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

    void ApplicationRun::mouseMoveEvent(const SDL_Event& event) {
        if (m_rightMouseButtonPressed) {
            const auto mouseXY = view::Mouse::mouseXY();
            m_view->translate((mouseXY.x() - m_previousMousePosition.x()), mouseXY.y() - m_previousMousePosition.y());
            m_previousMousePosition = mouseXY;
        }
    }

    void ApplicationRun::setTimeStep(Uint32 timeStep) {
        m_paused   = false;
        m_timeStep = timeStep;
    }

    void ApplicationRun::togglePause() {
        m_paused = !m_paused;
    }

    void ApplicationRun::initializeInteractStep() {
        for (auto& cluster : m_model.clusters()) {
            if (cluster.isAlive()) {
                cluster.resetPhase();
                cluster.incrementCommandIndex();
            }
        }
        ModelViewInterface::interactWithInstantBlocks(m_model, m_scrollArea);
        ModelViewInterface::removeActionBoxesOfRemovedClusters(m_model.clusters(), m_scrollArea);
        ModelViewInterface::updateSelection(m_model.clusters(), m_scrollArea);
        m_currentStep = CURRENT_STEP::INTERACT;
    }

    void ApplicationRun::initializeMovingStep() {
        m_model.startPhase();
        for (auto& cluster : m_model.clusters()) {
            ModelViewInterface::stopSpliceOrKillIfNeeded(m_model.level(), cluster);
        }
        addStoppedClustersToLevel();
        for (auto& cluster : m_model.clusters()) {
            if (not ModelViewInterface::interactWithDynamicBlocks(m_model.level(), cluster)) {
                cluster.doCommand(m_model);
            }
        }
        for (auto& cluster : m_model.clusters()) {
            cluster.buildSides();
        }
        m_model.level().createBoundaries();
        ModelViewInterface::updateCommandScrollArea(m_model, m_scrollArea, APP_MODE::RUNNING);
        m_currentStep = CURRENT_STEP::MOVING;
    }

    RUN_MODE ApplicationRun::performSingleLoop() {
        const auto currentTime = SDL_GetTicks();
        if (not m_paused) {
            m_timeSinceLastStep += SDL_GetTicks() - m_previousTime;
            const auto dt = currentTime - m_previousTime;
            switch (m_currentStep) {
                case CURRENT_STEP::MOVING:
                    if (m_timeSinceLastStep >= m_timeStep) {
                        initializeInteractStep();
                        m_timeSinceLastStep %= m_timeStep;
                    } else {
                        m_model.update(dt / static_cast<double>(m_timeStep));
                    }
                    break;
                case CURRENT_STEP::INTERACT:
                    if (m_timeSinceLastStep >= m_timeStep) {
                        initializeMovingStep();
                        m_timeSinceLastStep %= m_timeStep;
                        m_model.update(dt / static_cast<double>(m_timeStep));
                    }
                    break;
            }
        }
        draw();
        m_previousTime = currentTime;
        return m_runningMode;
    }

    void ApplicationRun::handleEvent(const SDL_Event& event) {
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

    view::widget::ScrollArea& ApplicationRun::scrollArea() {
        return m_scrollArea;
    }

    void ApplicationRun::addStoppedClustersToLevel() {
        auto it = std::partition(__IT(m_model.clusters()), __FUNC(cluster, cluster.state() != model::CLUSTER_STATE::STOPPED));
        m_model.level().stoppedClusters().splice(m_model.level().stoppedClusters().end(), m_model.clusters(), it, m_model.clusters().end());
    }

    void ApplicationRun::draw() {
        m_view->draw(m_model);
        m_view->drawScrollArea(&m_scrollArea);
        m_view->renderPresent();
    }
} // namespace app