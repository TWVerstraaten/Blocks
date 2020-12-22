//
// Created by pc on 15-12-20.
//

#include "Application_Level.h"

#include "model/WorldCoordinates.h"
#include "view/Color.h"

#include <algorithm>
#include <cassert>

Application_Level::Application_Level() {
    init();
}

void Application_Level::loop() {

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
        const auto dt = SDL_GetTicks() - m_previousTime;
        if (not m_paused) {
            update(static_cast<double>(1.9 * dt) / m_stepTimeInMilliSeconds);
            m_model.interactClustersWithLevel();
        }
        m_previousTime = SDL_GetTicks();
        m_view.draw(m_model);
        if (!m_paused) {
            m_timeSinceLastStep += dt;
        }

        SDL_RenderPresent(m_view.renderer());
    }
    SDL_StopTextInput();
}

void Application_Level::update(double fractionOfPhase) {
    m_model.update(fractionOfPhase);
}

void Application_Level::mouseWheelEvent() {
    m_view.zoom(m_event.wheel.y);
}

void Application_Level::keyEvent() {
    switch (m_event.type) {
        case SDL_TEXTINPUT:
            for (auto& actionEditBox : m_view.actionEditBoxes()) {
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
            for (auto& actionEditBox : m_view.actionEditBoxes()) {
                if (actionEditBox.hasFocus()) {
                    actionEditBox.handleKeyEvent(m_event);
                    return;
                }
            }

            switch (m_event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    resetModel();
                    pause();
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
            assert(m_pressedKeys.find(m_event.key.keysym.sym) != m_pressedKeys.end());
            m_pressedKeys.erase(m_pressedKeys.find(m_event.key.keysym.sym));
            break;
    }
}

void Application_Level::mouseClickEvent() {
    const auto mousePosition = getMouseCoordinates();

    for (auto& actionEditBox : m_view.actionEditBoxes()) {
        if (not actionEditBox.pointIsOverWidget(mousePosition)) {
            actionEditBox.loseFocus();
        }
    }
    for (auto& actionEditBox : m_view.actionEditBoxes()) {
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

void Application_Level::mouseReleaseEvent() {
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

void Application_Level::mouseMoveEvent() {
    const auto mousePosition = getMouseCoordinates();
    bool       isDone        = false;
    for (auto& actionEditBox : m_view.actionEditBoxes()) {
        if (actionEditBox.hasFocus()) {
            actionEditBox.handleMouseMoveEvent(mousePosition, m_leftMouseButtonPressed);
            isDone = true;
            break;
        }
    }
    if (not isDone) {
        if (m_rightMouseButtonPressed) {
            const auto mouseCoordinates = getMouseCoordinates();
            m_view.translate((mouseCoordinates.x - m_previousMousePosition.x), mouseCoordinates.y - m_previousMousePosition.y);
            m_previousMousePosition = mouseCoordinates;
        }
    }
}

SDL_Point Application_Level::getMouseCoordinates() {
    int xMouse, yMouse;
    SDL_GetMouseState(&xMouse, &yMouse);
    return {xMouse, yMouse};
}

void Application_Level::setTimeStep(Uint32 timeStep) {
    m_stepTimeInMilliSeconds = timeStep;
    unpause();
}

void Application_Level::resetModel() {
    m_previousTime      = SDL_GetTicks();
    m_timeSinceLastStep = 0;
    m_levelHasStarted   = false;
    m_paused            = true;

    m_view.clear();
    m_model.setClusters(m_initialModel.clusters());

    for (auto& cluster : m_model.clusters()) {
        m_view.addActionEditBox(cluster);
    }
}

void Application_Level::unpause() {
    if (not m_levelHasStarted) {
        startRun();
    } else {
        m_paused = false;
    }
}

void Application_Level::pause() {
    m_paused = true;
}

void Application_Level::togglePause() {
    if (m_paused) {
        unpause();
    } else {
        pause();
    }
}

void Application_Level::startRun() {
    if (not canStart()) {
        return;
    }
    m_paused = false;

    auto it = m_initialModel.clusters().begin();
    for (auto& actionEditBox : m_view.actionEditBoxes()) {
        actionEditBox.updateClusterActions(*it);
        ++it;
    }
    resetModel();
    m_levelHasStarted = true;
}

void Application_Level::init() {
    m_view.clear();
    m_initialModel.init();
    resetModel();
}

bool Application_Level::canStart() {
    return std::all_of(m_view.actionEditBoxes().begin(), m_view.actionEditBoxes().end(), [](const view::widget::ActionEditBox& box) {
        return box.canParse();
    });
}
