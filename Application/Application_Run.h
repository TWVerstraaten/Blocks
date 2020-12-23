//
// Created by pc on 22-12-20.
//

#ifndef BLOCKS_APPLICATION_RUN_H
#define BLOCKS_APPLICATION_RUN_H

#include "../model/Model.h"
#include "../view/View.h"
#include "Application_Level.h"

class Application_Run {

  public:
    Application_Run(const model::Model& model, view::View* view);

    void                        setTimeStep(Uint32 timeStep);
    void                        handleEvent(const SDL_Event& event);
    Application_Level::RUN_MODE performSingleLoop();
    void                        setPauseAfterNextStep(bool pauseAfterNextStep);

  private:
    void update(double fractionOfPhase);
    void togglePause();
    void mouseWheelEvent(const SDL_Event& event);
    void keyEvent(const SDL_Event& event);
    void mouseClickEvent(const SDL_Event& event);
    void mouseReleaseEvent(const SDL_Event& event);
    void mouseMoveEvent(const SDL_Event& event);
    void performTimeStep();

    bool                        m_paused                  = false;
    bool                        m_rightMouseButtonPressed = false;
    bool                        m_leftMouseButtonPressed  = false;
    bool                        m_pauseAfterNextStep      = false;
    Uint32                      m_previousTime{};
    Uint32                      m_timeSinceLastStep = 0;
    Uint32                      m_timeStep          = 300;
    Application_Level::RUN_MODE m_runningMode       = Application_Level::RUN_MODE::RUNNING;
    SDL_Point                   m_previousMousePosition{};
    model::Model                m_model;
    view::View*                 m_view;
};

#endif // BLOCKS_APPLICATION_RUN_H
