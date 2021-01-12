//
// Created by pc on 22-12-20.
//

#include "ApplicationEdit.h"

#include "../global/defines.h"
#include "../view/Mouse.h"
#include "../view/widget/BlockSelectWidget_constants.h"
#include "Application.h"

#include <cassert>

namespace app {

    ApplicationEdit::ApplicationEdit(model::Model* model, view::View* view, view::widget::ScrollArea* scrollArea)
        : m_view(view), m_model(model), m_scrollArea(scrollArea),
          m_blockSelectWidget({0,
                               static_cast<int>(m_view->windowHeight() - view::widget::BLOCK_SELECT_WIDGET_HEIGHT),
                               view::widget::BLOCK_SELECT_WIDGET_WIDTH,
                               view::widget::BLOCK_SELECT_WIDGET_HEIGHT}) {
        init();
        m_blockSelectWidget.init(m_view->assets());
    }

    void ApplicationEdit::mouseWheelEvent(const SDL_Event& event) {
        if (m_scrollArea->pointIsOverWidget(view::Mouse::mouseXY())) {
            m_scrollArea->mouseWheelEvent(event);
        } else if (m_blockSelectWidget.pointIsOverWidget(view::Mouse::mouseXY())) {
        } else {
            m_view->zoom(event.wheel.y);
        }
    }

    void ApplicationEdit::keyEvent(const SDL_Event& event) {
        if (event.type == SDL_KEYDOWN && SDL_GetModState() & KMOD_CTRL) {
            if (event.key.keysym.sym == SDLK_z) {
                undo();
                return;
            } else if (event.key.keysym.sym == SDLK_r) {
                redo();
                return;
            }
        }
        if (m_scrollArea->hasFocus()) {
            m_modelViewInterface.handleKeyEvent(event, *m_scrollArea, *m_model);
        }
    }

    void ApplicationEdit::mouseClickEvent(const SDL_Event& event) {
        setButtonBooleans(event);
        determineFocus(view::Mouse::mouseXY());
        if (m_scrollArea->hasFocus()) {
            m_scrollArea->leftClickEvent(event);
        } else if (m_blockSelectWidget.hasFocus()) {
            m_blockSelectWidget.leftClickEvent(event);
        } else {
            if (event.button.button == SDL_BUTTON_LEFT) {
                m_previousGridClickPosition = model::GridXY::fromScreenXY(view::Mouse::mouseXY(), m_view->viewPort());
                if (SDL_GetModState() & KMOD_CTRL) {
                    m_modelViewInterface.leftClickControl(
                        *m_model, *m_scrollArea, m_previousGridClickPosition, m_blockSelectWidget.selectedBlockType());
                } else {
                    m_modelViewInterface.leftMouseClick(
                        *m_model, *m_scrollArea, m_previousGridClickPosition, m_blockSelectWidget.selectedBlockType());
                }
            }
        }
    }

    void ApplicationEdit::mouseReleaseEvent(const SDL_Event& event) {
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

    void ApplicationEdit::mouseMoveEvent(const SDL_Event& event) {
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

    void ApplicationEdit::init() {
        ModelViewInterface::updateCommandScrollArea(*m_model, *m_scrollArea, APP_MODE::EDITING);
        m_scrollArea->update(m_view->renderer());
    }

    bool ApplicationEdit::canStart() const {
        return std::all_of(__CIT(m_scrollArea->children()), __FUNC(box, box.canParse()));
    }

    void ApplicationEdit::getActionsFromEditBoxes() {
        assert(m_model->clusters().size() == m_scrollArea->children().size());
        auto commandEdit = m_scrollArea->children().begin();
        for (auto& cluster : m_model->clusters()) {
            commandEdit->updateClusterCommands(cluster);
            ++commandEdit;
        }
    }

    void ApplicationEdit::finalize() {
        getActionsFromEditBoxes();
        m_scrollArea->loseFocus();
    }

    void ApplicationEdit::handleEvent(const SDL_Event& event) {
        switch (event.type) {
            case SDL_TEXTINPUT:
            case SDL_KEYDOWN:
                keyEvent(event);
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

    EDIT_MODE ApplicationEdit::performSingleLoop() {
        if (m_editMode != EDIT_MODE::EDITING) {
            finalize();
        }
        m_view->draw(*m_model);
        m_view->drawScrollArea(m_scrollArea);
        m_view->drawBlockSelectWidget(m_blockSelectWidget);
        m_view->renderPresent();
        return m_editMode;
    }

    bool ApplicationEdit::hasFocus() const {
        return (not m_scrollArea->hasFocus()) || (not m_scrollArea->hasChildWithFocus());
    }

    void ApplicationEdit::handleLeftMouseMove() {
        assert(m_leftMouseButtonPressed);
        assert(not m_rightMouseButtonPressed);
        const auto currentGridPosition = model::GridXY::fromScreenXY(view::Mouse::mouseXY(), m_view->viewPort());
        if (currentGridPosition != m_previousGridClickPosition) {
            if (SDL_GetModState() & KMOD_CTRL) {
                m_modelViewInterface.leftClickControl(*m_model, *m_scrollArea, currentGridPosition, m_blockSelectWidget.selectedBlockType());
            } else {
                m_modelViewInterface.leftMouseDrag(
                    *m_model, *m_scrollArea, currentGridPosition, m_previousGridClickPosition, m_blockSelectWidget.selectedBlockType());
            }
            m_previousGridClickPosition = currentGridPosition;
        }
    }

    void ApplicationEdit::handleRightMouseMove() {
        assert(m_rightMouseButtonPressed);
        assert(not m_leftMouseButtonPressed);
        const auto mouseXY = view::Mouse::mouseXY();
        m_view->translate((mouseXY.x() - m_previousMousePosition.x()), mouseXY.y() - m_previousMousePosition.y());
        m_previousMousePosition = mouseXY;
    }

    void ApplicationEdit::setButtonBooleans(const SDL_Event& event) {
        assert(event.type == SDL_MOUSEBUTTONDOWN);
        switch (event.button.button) {
            case SDL_BUTTON_RIGHT:
                m_rightMouseButtonPressed = true;
                m_previousMousePosition   = view::Mouse::mouseXY();
                break;
            case SDL_BUTTON_LEFT:
                m_leftMouseButtonPressed = true;
                m_previousMousePosition  = view::Mouse::mouseXY();
                break;
            default:
                break;
        }
    }

    model::Model* ApplicationEdit::model() const {
        return m_model;
    }

    void ApplicationEdit::undo() {
        m_modelViewInterface.undo(*this);
    }

    void ApplicationEdit::redo() {
        m_modelViewInterface.redo(*this);
    }

    view::widget::ScrollArea* ApplicationEdit::scrollArea() const {
        return m_scrollArea;
    }

    void ApplicationEdit::determineFocus(view::ScreenXY screenXY) {
        m_scrollArea->loseFocus();
        m_blockSelectWidget.loseFocus();
        if (m_scrollArea->pointIsOverWidget(screenXY)) {
            m_scrollArea->getFocus();
        } else if (m_blockSelectWidget.pointIsOverWidget(screenXY)) {
            m_blockSelectWidget.getFocus();
        }
    }
} // namespace app