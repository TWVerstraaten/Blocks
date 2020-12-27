//
// Created by pc on 22-12-20.
//

#include "Application_Edit.h"

#include "../view/Mouse.h"
#include "../view/ScreenXY.h"

#include <algorithm>
#include <cassert>

Application_Edit::Application_Edit(model::Model* model, view::View* view, view::widget::ScrollArea* scrollArea)
    : m_view(view), m_model(model), m_scrollArea(scrollArea) {
    init();
}

void Application_Edit::mouseWheelEvent(const SDL_Event& event) {
    if (m_scrollArea->pointIsOverWidget(Mouse::getMouseXY())) {
        m_scrollArea->mouseWheelEvent(event);
    } else {
        m_view->zoom(event.wheel.y);
    }
}

void Application_Edit::keyEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (SDL_GetModState() & KMOD_CTRL) {
            if (event.key.keysym.sym == SDLK_z) {
                if (not m_undoStack.empty()) {
                    m_undoStack.top()->undoAction(*this);
                    m_redoStack.push(std::move(m_undoStack.top()));
                    m_undoStack.pop();
                    Application_Level::updateCommandScrollArea(m_model->clusters(), m_scrollArea);
                }
            } else if (event.key.keysym.sym == SDLK_r) {
                if (not m_redoStack.empty()) {
                    m_redoStack.top()->redoAction(*this);
                    m_undoStack.push(std::move(m_redoStack.top()));
                    m_redoStack.pop();
                    Application_Level::updateCommandScrollArea(m_model->clusters(), m_scrollArea);
                }
            }
        }
    }
    if (m_scrollArea->hasFocus()) {
        m_scrollArea->keyEvent(event);
    }
}

void Application_Edit::mouseClickEvent(const SDL_Event& event) {
    if (m_scrollArea->pointIsOverWidget(Mouse::getMouseXY())) {
        m_scrollArea->getFocus();
    } else {
        m_scrollArea->loseFocus();
    }
    setButtonBooleans(event);
    if (m_scrollArea->hasFocus()) {
        m_scrollArea->leftClickEvent(event);
    } else {
        if (event.button.button == SDL_BUTTON_LEFT) {
            m_previousGridClickPosition = model::GridXY::fromScreenXY(Mouse::getMouseXY(), m_view->viewPort());
            if (SDL_GetModState() & KMOD_CTRL) {
                clearBlock(m_previousGridClickPosition);
            } else {
                addAction(m_model->addBlock(m_previousGridClickPosition));
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
        if (m_scrollArea->pointIsOverWidget(Mouse::getMouseXY())) {
            m_scrollArea->getFocus();
        }
    }
    if (m_scrollArea->hasFocus()) {
        if (m_leftMouseButtonPressed) {
            m_scrollArea->mouseDragEvent(event);
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
    Application_Level::updateCommandScrollArea(m_model->clusters(), m_scrollArea);
    m_scrollArea->update(m_view->renderer());
}

bool Application_Edit::canStart() const {
    return std::all_of(m_scrollArea->children().begin(), m_scrollArea->children().end(), [](const view::widget::CommandEditBox& box) {
        return box.canParse();
    });
}

void Application_Edit::getActionsFromEditBoxes() {
    assert(m_model->clusters().size() == m_scrollArea->children().size());
    auto actionEditIt = m_scrollArea->children().begin();
    for (auto& cluster : m_model->clusters()) {
        actionEditIt->updateClusterCommands(cluster);
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
    m_view->draw(m_scrollArea);
    m_view->renderPresent();
    return m_editMode;
}

bool Application_Edit::hasFocus() {
    return not m_scrollArea->hasFocus();
}

void Application_Edit::clearBlock(const model::GridXY& gridXY) {
    if (m_model->level().isFreeStartBlock(gridXY)) {
        m_model->clearBlock(gridXY);
    }
}

void Application_Edit::addBlock(const model::GridXY& gridXY) {
    if (not m_model->level().isFreeStartBlock(gridXY)) {
        return;
    }
    if (m_model->level().isFreeStartBlock(m_previousGridClickPosition)) {
        addAction(m_model->linkBlocks(m_previousGridClickPosition, gridXY));
    } else {

        auto a = m_model->addBlock(gridXY);
        if (a) {
            m_undoStack.push(std::move(a));
        }
    }
    Application_Level::updateCommandScrollArea(m_model->clusters(), m_scrollArea);
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
    Application_Level::updateCommandScrollArea(m_model->clusters(), m_scrollArea);
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
model::Model* Application_Edit::model() const {
    return m_model;
}
void Application_Edit::addAction(std::unique_ptr<model::action::Action>&& action) {
    if (action) {
        m_undoStack.push(std::move(action));
        m_redoStack = {};
    }
}
