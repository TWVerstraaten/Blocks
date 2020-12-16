//
// Created by pc on 15-12-20.
//

#include "Application.h"

#include <cassert>

Application::Application() {
    m_lastTime          = SDL_GetTicks();
    m_timeSinceLastStep = 0;
}

void Application::loop() {
    bool isRunning = true;
    while (isRunning) {
        if (m_timeSinceLastStep > m_stepTimeInMilliSeconds) {
            m_model.moveClusters();
            m_model.interactClustersWithLevel();
            m_timeSinceLastStep %= m_stepTimeInMilliSeconds;
        }
        while (SDL_PollEvent(&m_event) > 0) {
            switch (m_event.type) {
                case SDL_QUIT:
                    isRunning = false;
                    break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    keyEvent();
                    break;
                case SDL_WINDOWEVENT:
                    if (m_event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        //                        m_model.upda
                    }
                    break;
                case SDL_MOUSEMOTION: {
                    {
                        int xMouse, yMouse;
                        SDL_GetMouseState(&xMouse, &yMouse);
                    }
                } break;
                case SDL_MOUSEWHEEL: {
                    mouseWheelEvent();
                }
            }
        }
        const auto dt = SDL_GetTicks() - m_lastTime;
        update(dt);
        m_lastTime = SDL_GetTicks();
        m_view.draw(m_model);
        if (!m_isPaused) {
            m_timeSinceLastStep += dt;
        }
    }
}

void Application::update(double delta_time) {
    //    m_level.interactClustersWithLevel();
}

void Application::mouseWheelEvent() {
    m_view.zoom(m_event.wheel.y);
}

void Application::keyEvent() {
    switch (m_event.type) {
        case SDL_KEYDOWN:
            if (m_pressedKeys.find(m_event.key.keysym.sym) != m_pressedKeys.end()) {
                return;
            }
            m_pressedKeys.insert(m_event.key.keysym.sym);
            switch (m_event.key.keysym.sym) {
                case SDLK_SPACE:
                    m_isPaused = !m_isPaused;
                    break;
                case SDLK_1:
                    m_stepTimeInMilliSeconds = 1000;
                    m_isPaused               = false;
                    break;
                case SDLK_2:
                    m_stepTimeInMilliSeconds = 300;
                    m_isPaused               = false;
                    break;
                case SDLK_3:
                    m_stepTimeInMilliSeconds = 100;
                    m_isPaused               = false;
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
