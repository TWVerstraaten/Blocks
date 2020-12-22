//
// Created by pc on 22-12-20.
//

#ifndef BLOCKS_APPLICATION_EDIT_H
#define BLOCKS_APPLICATION_EDIT_H

#include "model/Model.h"
#include "view/View.h"

#include <SDL2/SDL.h>

class Application_Edit {

  public:
    explicit Application_Edit(view::View* view);

    enum class EXIT_CODE { QUIT, DONE_EDITING };

    EXIT_CODE           loop();
    const model::Model& model() const;

  private:
    enum class RUNNING_MODE { RUNNING, QUIT, DONE_EDITING };

    void      init();
    void      mouseWheelEvent();
    void      keyEvent();
    void      mouseClickEvent();
    void      mouseReleaseEvent();
    void      mouseMoveEvent();
    bool      canStart() const;
    SDL_Point getMouseCoordinates();

    RUNNING_MODE          m_runningMode = RUNNING_MODE::RUNNING;
    model::Model          m_model;
    view::View*           m_view = new view::View();
    SDL_Event             m_event;
    std::set<SDL_Keycode> m_pressedKeys;
    bool                  m_rightMouseButtonPressed = false;
    bool                  m_leftMouseButtonPressed  = false;
    SDL_Point             m_previousMousePosition;
};

#endif // BLOCKS_APPLICATION_EDIT_H
