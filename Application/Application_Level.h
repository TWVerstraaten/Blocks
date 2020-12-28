//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_APPLICATION_LEVEL_H
#define BLOCKS_APPLICATION_LEVEL_H

#include "../model/Model.h"
#include "../view/View.h"
#include "../view/widget/ScrollArea.h"

class Application_Edit;

class Application_Level {

  public:
    Application_Level();

    enum class RUN_MODE { QUIT, RUNNING, COMPLETED, FAILED, GIVE_UP };
    enum class EDIT_MODE { QUIT, EDITING, DONE_EDITING };
    enum class MODE { EDITING, RUNNING };

    void        run();
    static void updateCommandScrollArea(const std::list<model::Cluster>& clusters, view::widget::ScrollArea* scrollArea, MODE mode);

  private:
    Application_Level::RUN_MODE  runLevel();
    Application_Level::EDIT_MODE editLevel(Application_Edit& editApp);

    Uint32                   m_timeStep            = cst::TIME_STEP_SLOW;
    bool                     m_pauseAfterFirstStep = false;
    view::View               m_view;
    model::Model             m_model;
    view::widget::ScrollArea m_scrollArea;
    void                     handleWindowEvent();
};

#endif // BLOCKS_APPLICATION_LEVEL_H
