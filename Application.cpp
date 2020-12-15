//
// Created by pc on 15-12-20.
//

#include "Application.h"

Application::Application() {
    m_lastTime          = SDL_GetTicks();
    m_timeSinceLastStep = 0;
}

void Application::loop() {
    bool isRunning = true;
    while (isRunning) {
        if (m_timeSinceLastStep > m_stepTimeInMilliSeconds) {
            m_model.doStep();
            m_timeSinceLastStep %= m_stepTimeInMilliSeconds;
        }
        while (SDL_PollEvent(&m_event) > 0) {
            switch (m_event.type) {
                case SDL_QUIT:
                    isRunning = false;
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
        m_timeSinceLastStep += dt;
        m_lastTime = SDL_GetTicks();
        m_view.draw(m_model);
    }
}

void Application::update(double delta_time) {
    //    m_level.doStep();
}

void Application::mouseWheelEvent() {
    m_view.zoom(m_event.wheel.y);
}
