//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_APPLICATION_H
#define BLOCKS_APPLICATION_H

#include "model/Model.h"
#include "view/View.h"

#include <SDL2/SDL.h>

class Application {

  public:
    Application();

    void loop();
    void update(double delta_time);

  private:
    void mouseWheelEvent();

    model::Model m_model;
    view::View   m_view;
    SDL_Event    m_event;
    Uint32       m_lastTime;
    Uint32       m_timeSinceLastStep;
    Uint32       m_stepTimeInMilliSeconds = 500;
};

#endif // BLOCKS_APPLICATION_H
