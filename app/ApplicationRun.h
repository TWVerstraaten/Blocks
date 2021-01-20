////
//// Created by pc on 22-12-20.
////
//
//#ifndef BLOCKS_APPLICATIONRUN_H
//#define BLOCKS_APPLICATIONRUN_H
//
//#include "../model/Model.h"
//#include "../view/View.h"
//#include "../view/widget/ScrollArea.h"
//#include "Application_constants.h"
//#include "Application_enums.h"
//
//namespace app {
//
//    class ApplicationRun {
//
//      public:
//        enum class CURRENT_STEP { MOVING, INTERACT };
//
//        /****** CONSTRUCTORS / DESTRUCTORS  ******/
//        ApplicationRun(const model::Model& model, view::View* view, const view::widget::ScrollArea& scrollArea);
//
//        /****** NON CONST FUNCTIONS  ******/
//        void                      setTimeStep(Uint32 timeStep);
//        void                      handleEvent(const SDL_Event& event);
//        void                      killDoubleGrabbers();
//        void                      doConwayStep();
//        RUN_MODE                  performSingleLoop();
//        view::widget::ScrollArea& scrollArea();
//
//      private:
//        /****** PRIVATE NON CONST FUNCTIONS  ******/
//        void togglePause();
//        void initializeMovingStep();
//        void initializeInteractStep();
//        void addStoppedClustersToLevel();
//        void mouseWheelEvent(const SDL_Event& event);
//        void keyEvent(const SDL_Event& event);
//        void mouseClickEvent(const SDL_Event& event);
//        void mouseReleaseEvent(const SDL_Event& event);
//        void mouseMoveEvent(const SDL_Event& event);
//        void draw();
//
//        /****** DATA MEMBERS  ******/
//        bool                     m_paused                  = false;
//        bool                     m_rightMouseButtonPressed = false;
//        bool                     m_leftMouseButtonPressed  = false;
//        CURRENT_STEP             m_currentStep             = CURRENT_STEP::INTERACT;
//        Uint32                   m_previousTime;
//        int                      m_timeSinceLastStep = 0;
//        Uint32                   m_timeStep          = TIME_STEP_SLOW;
//        RUN_MODE                 m_runningMode       = RUN_MODE::RUNNING;
//        view::ScreenXy           m_previousMousePosition;
//        model::Model             m_model;
//        view::View*              m_view;
//        view::widget::ScrollArea m_scrollArea;
//    };
//} // namespace app
//#endif // BLOCKS_APPLICATIONRUN_H
