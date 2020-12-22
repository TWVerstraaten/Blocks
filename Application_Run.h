//
// Created by pc on 22-12-20.
//

#ifndef BLOCKS_APPLICATION_RUN_H
#define BLOCKS_APPLICATION_RUN_H

#include "model/Model.h"
#include "view/View.h"

class Application_Run {

  public:
    Application_Run(const model::Model& model, view::View* view);

    enum class EXIT_CODE { QUIT, COMPLETED, FAILED, GAVE_UP };

    EXIT_CODE run();
    void      setTimeStep(Uint32 timeStep);

  private:
    enum class RUNNING_MODE { RUNNING, QUIT, COMPLETED, FAILED, GAVE_UP };

    void update(double fractionOfPhase);
    void togglePause();
    void mouseWheelEvent();
    void keyEvent();
    void mouseClickEvent();
    void mouseReleaseEvent();
    void mouseMoveEvent();

    RUNNING_MODE          m_runningMode             = RUNNING_MODE::RUNNING;
    bool                  m_paused                  = false;
    bool                  m_rightMouseButtonPressed = false;
    bool                  m_leftMouseButtonPressed  = false;
    Uint32                m_previousTime;
    Uint32                m_timeSinceLastStep = 0;
    Uint32                m_timeStep          = 300;
    SDL_Event             m_event;
    SDL_Point             m_previousMousePosition;
    model::Model          m_model;
    view::View*           m_view;
    std::set<SDL_Keycode> m_pressedKeys;
};

#endif // BLOCKS_APPLICATION_RUN_H
