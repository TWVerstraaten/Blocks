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

    void run();

  private:
    view::View   m_view;
    model::Model m_model;
};

#endif // BLOCKS_APPLICATION_LEVEL_H
