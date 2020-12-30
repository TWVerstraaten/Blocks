//
// Created by pc on 22-12-20.
//

#ifndef BLOCKS_APPLICATION_EDIT_H
#define BLOCKS_APPLICATION_EDIT_H

#include "../model/Model.h"
#include "../view/View.h"
#include "../view/widget/BlockSelectWidget.h"
#include "Application_enums.h"
#include "ModelViewInterface.h"

#include <SDL.h>

namespace app {
    class Application_Edit {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        Application_Edit(model::Model* model, view::View* view, view::widget::ScrollArea* scrollArea);

        /****** CONST GETTERS  ******/
        [[nodiscard]] Uint32                    timeStep() const;
        [[nodiscard]] model::Model*             model() const;
        [[nodiscard]] view::widget::ScrollArea* scrollArea() const;

        /****** CONST FUNCTIONS  ******/
        [[nodiscard]] bool canStart() const;
        [[nodiscard]] bool hasFocus() const;

        /****** NON CONST FUNCTIONS  ******/
        void      handleEvent(const SDL_Event& event);
        void      finalize();
        EDIT_MODE performSingleLoop();

      private:
        /****** PRIVATE NON CONST FUNCTIONS  ******/
        void init();
        void undo();
        void redo();
        void getActionsFromEditBoxes();
        void handleLeftMouseMove();
        void handleRightMouseMove();
        void mouseWheelEvent(const SDL_Event& event);
        void keyEvent(const SDL_Event& event);
        void mouseClickEvent(const SDL_Event& event);
        void mouseReleaseEvent(const SDL_Event& event);
        void mouseMoveEvent(const SDL_Event& event);
        void setButtonBooleans(const SDL_Event& event);
        void determineFocus(view::ScreenXY screenXY);

        /****** DATA MEMBERS  ******/
        bool                            m_rightMouseButtonPressed = false;
        bool                            m_leftMouseButtonPressed  = false;
        Uint32                          m_timeStep                = cst::TIME_STEP_SLOW;
        EDIT_MODE                       m_editMode                = EDIT_MODE::EDITING;
        model::GridXY                   m_previousGridClickPosition{0, 0};
        view::View*                     m_view;
        model::Model*                   m_model;
        view::widget::ScrollArea*       m_scrollArea;
        view::widget::BlockSelectWidget m_blockSelectWidget;
        view::ScreenXY                  m_previousMousePosition;
        ModelViewInterface              m_modelViewInterface;
    };
} // namespace app
#endif // BLOCKS_APPLICATION_EDIT_H
