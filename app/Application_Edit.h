//
// Created by pc on 22-12-20.
//

#ifndef BLOCKS_APPLICATION_EDIT_H
#define BLOCKS_APPLICATION_EDIT_H

#include "../action/Action.h"
#include "../global/cst.h"
#include "../model/Model.h"
#include "../view/View.h"
#include "Application_enums.h"
#include "ModelViewInterface.h"

#include <SDL.h>
#include <stack>

namespace app {
    class Application_Edit {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        Application_Edit(model::Model* model, view::View* view, view::widget::ScrollArea* scrollArea);

        /****** CONST GETTERS  ******/
        Uint32        timeStep() const;
        model::Model* model() const;
        view::widget::ScrollArea* scrollArea() const;

        /****** CONST FUNCTIONS  ******/
        bool canStart() const;
        bool hasFocus() const;

        /****** NON CONST FUNCTIONS  ******/
        void      handleEvent(const SDL_Event& event);
        void      finalize();
        EDIT_MODE performSingleLoop();

      private:
        /****** PRIVATE NON CONST FUNCTIONS  ******/
        void init();
        void mouseWheelEvent(const SDL_Event& event);
        void keyEvent(const SDL_Event& event);
        void mouseClickEvent(const SDL_Event& event);
        void mouseReleaseEvent(const SDL_Event& event);
        void mouseMoveEvent(const SDL_Event& event);
        void getActionsFromEditBoxes();
        void handleLeftMouseMove();
        void handleRightMouseMove();
        void setButtonBooleans(const SDL_Event& event);
        void undo();
        void redo();
        void updateClusterActions();

        /****** DATA MEMBERS  ******/
        bool                                        m_rightMouseButtonPressed = false;
        bool                                        m_leftMouseButtonPressed  = false;
        Uint32                                      m_timeStep                = cst::TIME_STEP_SLOW;
        EDIT_MODE                                   m_editMode                = EDIT_MODE::EDITING;
        model::GridXY                               m_previousGridClickPosition{0, 0};
        view::View*                                 m_view;
        model::Model*                               m_model;
        view::widget::ScrollArea*                   m_scrollArea;
        view::ScreenXY                              m_previousMousePosition;
        std::stack<std::unique_ptr<action::Action>> m_undoStack;
        std::stack<std::unique_ptr<action::Action>> m_redoStack;
        ModelViewInterface                          m_modelViewInterface;
    };
} // namespace app
#endif // BLOCKS_APPLICATION_EDIT_H
