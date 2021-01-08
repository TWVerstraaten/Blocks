//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_APPLICATION_H
#define BLOCKS_APPLICATION_H

#include "../model/Model.h"
#include "../view/View.h"
#include "../view/widget/ScrollArea.h"
#include "Application_constants.h"
#include "Application_enums.h"

namespace app {
    class Application_Edit;

    class Application {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        Application();

        /****** NON CONST FUNCTIONS  ******/
        void run();

      private:
        /****** PRIVATE NON CONST FUNCTIONS  ******/
        RUN_MODE  runLevel();
        EDIT_MODE editLevel(Application_Edit& editApp);
        void      handleWindowEvent();

        /****** DATA MEMBERS  ******/
        APP_MODE                  m_appMode  = APP_MODE::EDITING;
        Uint32                    m_timeStep = TIME_STEP_SLOW;
        view::View                m_view;
        model::Model              m_model;
        view::widget::ScrollArea  m_scrollArea;
        view::widget::ScrollArea* m_runningScrollArea;
    };
} // namespace app
#endif // BLOCKS_APPLICATION_H
