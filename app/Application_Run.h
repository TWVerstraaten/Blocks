//
// Created by pc on 22-12-20.
//

#ifndef BLOCKS_APPLICATION_RUN_H
#define BLOCKS_APPLICATION_RUN_H

#include "../model/Model.h"
#include "../view/View.h"
#include "../view/widget/ScrollArea.h"
#include "Application_enums.h"

namespace app {

    class Application_Run {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        Application_Run(const model::Model& model, view::View* view, const view::widget::ScrollArea& scrollArea);

        /****** NON CONST FUNCTIONS  ******/
        void                      setTimeStep(Uint32 timeStep);
        void                      handleEvent(const SDL_Event& event);
        void                      setPauseAfterNextStep(bool pauseAfterNextStep);
        RUN_MODE                  performSingleLoop();
        view::widget::ScrollArea& scrollArea();

      private:
        /****** PRIVATE NON CONST FUNCTIONS  ******/
        void togglePause();
        void performTimeStep();
        void addStoppedClustersToLevel();
        void update(double fractionOfPhase);
        void mouseWheelEvent(const SDL_Event& event);
        void keyEvent(const SDL_Event& event);
        void mouseClickEvent(const SDL_Event& event);
        void mouseReleaseEvent(const SDL_Event& event);
        void mouseMoveEvent(const SDL_Event& event);
        void stopClustersIfNeeded();
        void spliceClustersIfNeeded();
        void draw(Uint32 dT);

        /****** DATA MEMBERS  ******/
        bool                     m_paused                  = false;
        bool                     m_rightMouseButtonPressed = false;
        bool                     m_leftMouseButtonPressed  = false;
        bool                     m_pauseAfterNextStep      = false;
        Uint32                   m_previousTime;
        Uint32                   m_timeSinceLastStep = 0;
        Uint32                   m_timeStep          = cst::TIME_STEP_SLOW;
        RUN_MODE                 m_runningMode       = RUN_MODE::RUNNING;
        view::ScreenXY           m_previousMousePosition;
        model::Model             m_model;
        view::View*              m_view;
        view::widget::ScrollArea m_scrollArea;
    };
} // namespace app
#endif // BLOCKS_APPLICATION_RUN_H
