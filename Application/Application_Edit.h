//
// Created by pc on 22-12-20.
//

#ifndef BLOCKS_APPLICATION_EDIT_H
#define BLOCKS_APPLICATION_EDIT_H

#include "../Global.h"
#include "../model/Model.h"
#include "../view/View.h"

#include <SDL2/SDL.h>

class Application_Edit {

  public:
    explicit Application_Edit(model::Model* model, view::View* view);

    enum class EXIT_CODE { QUIT, DONE_EDITING };

    EXIT_CODE run();

    Uint32 timeStep() const;

  private:
    enum class RUNNING_MODE { RUNNING, QUIT, DONE_EDITING };

    EXIT_CODE finalizeAndReturnExitCode();
    EXIT_CODE runningModeToExitCode() const;
    void      init();
    void      mouseWheelEvent();
    void      keyEvent();
    void      mouseClickEvent();
    void      mouseReleaseEvent();
    void      mouseMoveEvent();
    bool      canStart() const;
    void      getActionsFromEditBoxes();
    void      setFocusOnClick();

    view::Widget* m_focusedWidget = nullptr;
    Uint32        m_timeStep      = global::m_timeStepSlow;
    RUNNING_MODE  m_runningMode   = RUNNING_MODE::RUNNING;
    model::Model* m_model;
    view::View*   m_view;
    SDL_Event     m_event{};
    bool          m_rightMouseButtonPressed = false;
    bool          m_leftMouseButtonPressed  = false;
    SDL_Point     m_previousMousePosition{};
};

#endif // BLOCKS_APPLICATION_EDIT_H
