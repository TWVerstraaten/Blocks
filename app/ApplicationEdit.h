////
//// Created by pc on 22-12-20.
////
//
//#ifndef BLOCKS_APPLICATIONEDIT_H
//#define BLOCKS_APPLICATIONEDIT_H
//
//#include "../model/Model.h"
//#include "../view/View.h"
//#include "../view/widget/BlockSelectWidget.h"
//#include "Application_enums.h"
//#include "ModelViewInterface.h"
//
//#include <SDL.h>
//
//namespace app {
//    class ApplicationEdit {
//
//      public:
//        /****** CONSTRUCTORS / DESTRUCTORS  ******/
//        ApplicationEdit(model::Model* model, view::View* view, view::widget::ScrollArea* scrollArea);
//
//        [[nodiscard]] model::Model*             model() const;
//        [[nodiscard]] view::widget::ScrollArea* scrollArea() const;
//
//        /****** CONST FUNCTIONS  ******/
//        [[nodiscard]] bool canStart() const;
//        [[nodiscard]] bool hasFocus() const;
//
//        /****** NON CONST FUNCTIONS  ******/
//        void      handleEvent(const SDL_Event& event);
//        void      finalize();
//        EDIT_MODE performSingleLoop();
//
//      private:
//        /****** PRIVATE NON CONST FUNCTIONS  ******/
//        void init();
//        void undo();
//        void redo();
//        void getActionsFromEditBoxes();
//        void handleLeftMouseMove();
//        void handleRightMouseMove();
//        void mouseWheelEvent(const SDL_Event& event);
//        void keyEvent(const SDL_Event& event);
//        void mouseClickEvent(const SDL_Event& event);
//        void mouseReleaseEvent(const SDL_Event& event);
//        void mouseMoveEvent(const SDL_Event& event);
//        void setButtonBooleans(const SDL_Event& event);
//        void determineFocus(view::ScreenXy screenXy);
//
//        /****** DATA MEMBERS  ******/
//        bool                            m_rightMouseButtonPressed = false;
//        bool                            m_leftMouseButtonPressed  = false;
//        EDIT_MODE                       m_editMode                = EDIT_MODE::EDITING;
//        model::GridXy                   m_previousGridClickPosition{0, 0};
//        view::View*                     m_view;
//        model::Model*                   m_model;
//        view::widget::ScrollArea*       m_scrollArea;
//        view::widget::BlockSelectWidget m_blockSelectWidget;
//        view::ScreenXy                  m_previousMousePosition;
//        ModelViewInterface              m_modelViewInterface;
//    };
//} // namespace app
//#endif // BLOCKS_APPLICATIONEDIT_H
