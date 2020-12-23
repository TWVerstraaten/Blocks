//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_APPLICATION_LEVEL_H
#define BLOCKS_APPLICATION_LEVEL_H

#include "../model/Model.h"
#include "../view/View.h"

class Application_Level {

  public:
    Application_Level();

    enum class RUN_MODE { QUIT, RUNNING, COMPLETED, FAILED, GIVE_UP };
    enum class EDIT_MODE { QUIT, EDITING, DONE_EDITING };

    void run();

  private:
    Application_Level::RUN_MODE runLevel();
    EDIT_MODE                   editLevel();

    Uint32       m_timestep = global::m_timeStepSlow;
    view::View   m_view;
    model::Model m_model;
};

#endif // BLOCKS_APPLICATION_LEVEL_H
