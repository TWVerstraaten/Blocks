//
// Created by pc on 22-12-20.
//

#include "Application_Edit.h"

#include "../view/Mouse.h"
#include "../view/ScreenXY.h"

#include <algorithm>
#include <cassert>

Application_Edit::Application_Edit(model::Model* model, view::View* view) : m_view(view), m_model(model) {
    init();
}

void Application_Edit::mouseWheelEvent(const SDL_Event& event) {
    m_view->zoom(event.wheel.y);
}

void Application_Edit::keyEvent(const SDL_Event& event) {
    if (m_focusedWidget) {
        m_focusedWidget->keyEvent(event);
    }
}

void Application_Edit::setFocusOnClick() {
    m_focusedWidget          = nullptr;
    const auto mousePosition = Mouse::getMouseXY();
    for (auto& actionEditBox : m_view->actionEditBoxes()) {
        if (not actionEditBox.pointIsOverWidget(mousePosition)) {
            actionEditBox.loseFocus();
        } else {
            actionEditBox.getFocus();
            m_focusedWidget = &actionEditBox;
        }
    }
}

void Application_Edit::mouseClickEvent(const SDL_Event& event) {
    const auto mousePosition = Mouse::getMouseXY();
    switch (event.button.button) {
        case SDL_BUTTON_RIGHT:
            m_rightMouseButtonPressed = true;
            m_previousMousePosition   = mousePosition;
            break;
        case SDL_BUTTON_LEFT:
            m_leftMouseButtonPressed = true;
            m_previousMousePosition  = mousePosition;
            break;
        default:
            break;
    }

    setFocusOnClick();
    if (m_focusedWidget) {
        if (m_leftMouseButtonPressed) {
            m_focusedWidget->leftClickEvent(event);
        }
    } else {
        if (event.button.button == SDL_BUTTON_LEFT) {
            m_previousGridClickPosition = model::GridXY::fromScreenXY({mousePosition.x, mousePosition.y}, m_view->viewPort());
            if (SDL_GetModState() & KMOD_CTRL) {
                clearBlock(m_previousGridClickPosition);
            } else {
                m_model->addBlock(m_previousGridClickPosition);
                m_view->updateActionBoxes(m_model->clusters());
            }
        }
    }
}

void Application_Edit::mouseReleaseEvent(const SDL_Event& event) {
    switch (event.button.button) {
        case SDL_BUTTON_RIGHT:
            m_rightMouseButtonPressed = false;
            break;
        case SDL_BUTTON_LEFT:
            m_leftMouseButtonPressed = false;
            break;
        default:
            break;
    }
}

void Application_Edit::mouseMoveEvent(const SDL_Event& event) {
    if (m_focusedWidget) {
        if (m_leftMouseButtonPressed) {
            m_focusedWidget->mouseDragEvent(event);
        }
    } else {
        if (m_rightMouseButtonPressed) {
            const auto mouseXY = Mouse::getMouseXY();
            m_view->translate((mouseXY.x - m_previousMousePosition.x), mouseXY.y - m_previousMousePosition.y);
            m_previousMousePosition = mouseXY;
        } else if (m_leftMouseButtonPressed) {
            const auto currentGridPosition = model::GridXY::fromScreenXY(Mouse::getMouseXY(), m_view->viewPort());
            if (currentGridPosition != m_previousGridClickPosition) {
                if (SDL_GetModState() & KMOD_CTRL) {
                    clearBlock(currentGridPosition);
                } else {
                    addBlock(currentGridPosition);
                }
                m_previousGridClickPosition = currentGridPosition;
            }
        }
    }
}

void Application_Edit::init() {
    SDL_StartTextInput();
    m_view->clear();
    m_view->initActionBoxes(m_model->clusters());
}

bool Application_Edit::canStart() const {
    return std::all_of(m_view->actionEditBoxes().begin(), m_view->actionEditBoxes().end(), [](const view::widget::LineEditBox& box) {
        return box.canParse();
    });
}

void Application_Edit::getActionsFromEditBoxes() {
    assert(m_model->clusters().size() == m_view->actionEditBoxes().size());
    auto actionEditIt = m_view->actionEditBoxes().begin();
    for (auto& cluster : m_model->clusters()) {
        actionEditIt->updateClusterActions(cluster);
        ++actionEditIt;
    }
}

Uint32 Application_Edit::timeStep() const {
    return m_timeStep;
}

void Application_Edit::finalize() {
    getActionsFromEditBoxes();
    SDL_StopTextInput();
}

void Application_Edit::handleEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_TEXTINPUT:
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            keyEvent(event);
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {}
            break;
        case SDL_MOUSEBUTTONDOWN:
            mouseClickEvent(event);
            break;
        case SDL_MOUSEBUTTONUP:
            mouseReleaseEvent(event);
            break;
        case SDL_MOUSEMOTION:
            mouseMoveEvent(event);
            break;
        case SDL_MOUSEWHEEL:
            mouseWheelEvent(event);
            break;
        default:
            break;
    }
}

Application_Level::EDIT_MODE Application_Edit::performSingleLoop() {
    if (m_editMode != Application_Level::EDIT_MODE::EDITING) {
        finalize();
    }
    m_view->draw(*m_model);
    m_view->renderPresent();
    return m_editMode;
}

bool Application_Edit::hasFocus() {
    return m_focusedWidget == nullptr;
}

void Application_Edit::clearBlock(const model::GridXY& gridXY) {
    if (m_model->level().isFreeStartBlock(gridXY)) {
        m_model->clearBlock(gridXY);
        m_view->updateActionBoxes(m_model->clusters());
    }
}

void Application_Edit::addBlock(const model::GridXY& gridXY) {
    if (m_model->level().isFreeStartBlock(m_previousGridClickPosition)) {
        m_model->linkBlocks(m_previousGridClickPosition, gridXY);
    } else {
        m_model->addBlock(gridXY);
    }
    m_view->updateActionBoxes(m_model->clusters());
}
