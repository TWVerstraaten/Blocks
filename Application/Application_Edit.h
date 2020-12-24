//
// Created by pc on 22-12-20.
//

#ifndef BLOCKS_APPLICATION_EDIT_H
#define BLOCKS_APPLICATION_EDIT_H

#include "../global/Conf.h"
#include "../model/Model.h"
#include "../view/View.h"
#include "Application_Level.h"

#include <SDL.h>

class Application_Edit {

  public:
    explicit Application_Edit(model::Model* model, view::View* view);

    void                         handleEvent(const SDL_Event& event);
    Application_Level::EDIT_MODE performSingleLoop();
    Uint32                       timeStep() const;
    bool                         canStart() const;
    bool                         hasFocus();
    void                         finalize();

  private:
    void init();
    void mouseWheelEvent(const SDL_Event& event);
    void keyEvent(const SDL_Event& event);
    void mouseClickEvent(const SDL_Event& event);
    void mouseReleaseEvent(const SDL_Event& event);
    void mouseMoveEvent(const SDL_Event& event);
    void getActionsFromEditBoxes();
    void setFocusOnClick();

    bool                         m_rightMouseButtonPressed = false;
    bool                         m_leftMouseButtonPressed  = false;
    Uint32                       m_timeStep                = cst::m_timeStepSlow;
    Application_Level::EDIT_MODE m_editMode                = Application_Level::EDIT_MODE::EDITING;
    view::Widget*                m_focusedWidget           = nullptr;
    view::View*                  m_view;
    model::Model*                m_model;
    SDL_Point                    m_previousMousePosition{};
};

#endif // BLOCKS_APPLICATION_EDIT_H
