//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_APPLICATION_LEVEL_H
#define BLOCKS_APPLICATION_LEVEL_H

#include "../model/Model.h"
#include "../view/View.h"
#include "../view/widget/ScrollArea.h"
#include "Application_enums.h"

namespace app {
    class Application_Edit;

    class Application_Level {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        Application_Level();

        /****** PUBLIC STATIC FUNCTIONS  ******/
        static void updateCommandScrollArea(const std::list<model::Cluster>& clusters,
                                            view::widget::ScrollArea*        scrollArea,
                                            APP_MODE                         mode);

        /****** NON CONST FUNCTIONS  ******/
        void run();

      private:
        /****** PRIVATE NON CONST FUNCTIONS  ******/
        RUN_MODE  runLevel();
        EDIT_MODE editLevel(Application_Edit& editApp);
        void      handleWindowEvent();

        /****** DATA MEMBERS  ******/
        Uint32                   m_timeStep            = cst::TIME_STEP_SLOW;
        bool                     m_pauseAfterFirstStep = false;
        view::View               m_view;
        model::Model             m_model;
        view::widget::ScrollArea m_scrollArea;
    };
} // namespace app
#endif // BLOCKS_APPLICATION_LEVEL_H
