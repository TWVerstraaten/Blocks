//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_APPLICATION_LEVEL_H
#define BLOCKS_APPLICATION_LEVEL_H

#include "model/Model.h"
#include "view/View.h"

class Application_Level {

  public:
    Application_Level();

    void             run();
    static SDL_Point getMouseCoordinates();

    static const Uint32 m_timeStepSlow   = 1000;
    static const Uint32 m_timeStepMedium = 300;
    static const Uint32 m_timeStepFast   = 50;

  private:
    view::View   m_view;
    model::Model m_model;
};

#endif // BLOCKS_APPLICATION_LEVEL_H
