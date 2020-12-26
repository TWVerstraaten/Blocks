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
    if (m_view->scrollArea().pointIsOverWidget(Mouse::getMouseXY())) {
        m_view->scrollArea().mouseWheelEvent(event);
    } else {
        m_view->zoom(event.wheel.y);
    }
}

void Application_Edit::keyEvent(const SDL_Event& event) {
    if (m_view->scrollArea().hasFocus()) {
        m_view->scrollArea().keyEvent(event);
    }
}

void Application_Edit::mouseClickEvent(const SDL_Event& event) {
    if (m_view->scrollArea().pointIsOverWidget(Mouse::getMouseXY())) {
        m_view->scrollArea().getFocus();
    } else {
        m_view->scrollArea().loseFocus();
    }
    setButtonBooleans(event);
    if (m_view->scrollArea().hasFocus()) {
        m_view->scrollArea().leftClickEvent(event);
    } else {
        if (event.button.button == SDL_BUTTON_LEFT) {
            m_previousGridClickPosition = model::GridXY::fromScreenXY(Mouse::getMouseXY(), m_view->viewPort());
            if (SDL_GetModState() & KMOD_CTRL) {
                clearBlock(m_previousGridClickPosition);
            } else {
                addBlock(m_previousGridClickPosition);
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
    if (m_leftMouseButtonPressed) {
        if (m_view->scrollArea().pointIsOverWidget(Mouse::getMouseXY())) {
            m_view->scrollArea().mouseDragEvent(event);
        }
    }
    if (m_view->scrollArea().hasFocus()) {
        if (m_leftMouseButtonPressed) {
            m_view->scrollArea().mouseDragEvent(event);
        }
    } else {
        if (m_rightMouseButtonPressed) {
            handleRightMouseMove();
        } else if (m_leftMouseButtonPressed) {
            handleLeftMouseMove();
        }
    }
}

void Application_Edit::init() {
    SDL_StartTextInput();
    m_view->clear();
    m_view->initActionBoxes(m_model->clusters());
}

bool Application_Edit::canStart() const {
    return std::all_of(m_view->actionEditBoxes().begin(), m_view->actionEditBoxes().end(), [](const view::widget::ActionEditBox& box) {
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
    return not m_view->scrollArea().hasFocus();
}

void Application_Edit::clearBlock(const model::GridXY& gridXY) {
    if (m_model->level().isFreeStartBlock(gridXY)) {
        m_model->clearBlock(gridXY);
        m_view->updateActionBoxes(m_model->clusters());
    }
}

void Application_Edit::addBlock(const model::GridXY& gridXY) {
    if (not m_model->level().isFreeStartBlock(gridXY)) {
        return;
    }
    if (m_model->level().isFreeStartBlock(m_previousGridClickPosition)) {
        m_model->linkBlocks(m_previousGridClickPosition, gridXY);
    } else {
        m_model->addBlock(gridXY);
    }
    m_view->updateActionBoxes(m_model->clusters());
}

void Application_Edit::handleLeftMouseMove() {
    assert(m_leftMouseButtonPressed);
    assert(not m_rightMouseButtonPressed);
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

void Application_Edit::handleRightMouseMove() {
    assert(m_rightMouseButtonPressed);
    assert(not m_leftMouseButtonPressed);
    const auto mouseXY = Mouse::getMouseXY();
    m_view->translate((mouseXY.x - m_previousMousePosition.x), mouseXY.y - m_previousMousePosition.y);
    m_previousMousePosition = mouseXY;
}

void Application_Edit::setButtonBooleans(const SDL_Event& event) {
    assert(event.type == SDL_MOUSEBUTTONDOWN);
    switch (event.button.button) {
        case SDL_BUTTON_RIGHT:
            m_rightMouseButtonPressed = true;
            m_previousMousePosition   = Mouse::getMouseXY();
            break;
        case SDL_BUTTON_LEFT:
            m_leftMouseButtonPressed = true;
            m_previousMousePosition  = Mouse::getMouseXY();
            break;
        default:
            break;
    }
}
