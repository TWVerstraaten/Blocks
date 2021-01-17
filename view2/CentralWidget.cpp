#include "CentralWidget.h"

#include "../model/Model.h"
#include "MainView.h"

#include <QApplication>
#include <QKeyEvent>
#include <QTimer>
#include <action/GlobAction.h>
#include <app/ApplicationEdit.h>

namespace view2 {

    CentralWidget::CentralWidget() {
        auto* layout = new QGridLayout;

        setGeometry(0, 0, 1000, 800);
        setLayout(layout);

        m_commandScrollArea = new CommandScrollArea(this);
        m_mainView          = new MainView(this);
        layout->addWidget(m_mainView, 0, 0);
        layout->addWidget(m_commandScrollArea, 0, 1);
    }

    void CentralWidget::keyPressEvent(QKeyEvent* event) {
        if ((QApplication::keyboardModifiers() & Qt::ControlModifier) && event->key() == Qt::Key_Z) {
            if (QApplication::keyboardModifiers() & Qt::ShiftModifier) {
                redo();
            } else {
                undo();
            }
            update();
            return;
        }

        switch (event->key()) {
            case Qt::Key_Space:
                qDebug() << "Space pressed";
                break;
        }
    }

    void CentralWidget::addAction(std::unique_ptr<action::Action>&& action) {
        if (action != nullptr) {
            if (m_undoMode == UNDO_MODE::GLOB) {
                assert(m_undoStack.top()->type() == action::ACTION_TYPE::GLOB);
                dynamic_cast<action::GlobAction*>(m_undoStack.top().get())->add(std::move(action));
            } else {
                switch (action->type()) {

                    case action::ACTION_TYPE::ADD_BLOCK_TO_CLUSTER:
                        qDebug() << "Add Block to Cluster";
                        break;
                    case action::ACTION_TYPE::ADD_CLUSTER:
                        qDebug() << "Add Cluster";
                        break;
                    case action::ACTION_TYPE::GENERIC_COMMAND_EDIT_BOX:
                        qDebug() << "Command edit box";
                        break;
                    case action::ACTION_TYPE::GENERIC_MODEL:
                        qDebug() << "Model";
                        break;
                    case action::ACTION_TYPE::REMOVE_BLOCK_FROM_CLUSTER:
                        qDebug() << "Remove block";
                        break;
                    case action::ACTION_TYPE::REMOVE_CLUSTER:
                        qDebug() << "Remove cluster";
                        break;
                    case action::ACTION_TYPE::ADD_LEVEL_BLOCK_ACTION:
                    case action::ACTION_TYPE::REMOVE_LEVEL_BLOCK_ACTION:
                    case action::ACTION_TYPE::GENERIC_TEXT_EDIT_ACTION:
                    case action::ACTION_TYPE::GLOB:
                        break;
                }

                m_undoStack.push(std::move(action));
            }
            m_redoStack = {};
        }
    }

    void CentralWidget::undo() {
        if (m_undoStack.empty()) {
            return;
        }
        m_undoStack.top()->undoAction();
        m_redoStack.push(std::move(m_undoStack.top()));
        assert(m_undoStack.top() == nullptr);
        m_undoStack.pop();
    }

    void CentralWidget::redo() {
        if (m_redoStack.empty()) {
            return;
        }
        m_redoStack.top()->redoAction();
        m_undoStack.push(std::move(m_redoStack.top()));
        assert(m_redoStack.top() == nullptr);
        m_redoStack.pop();
    }

    CommandScrollArea* CentralWidget::commandScrollArea() const {
        return m_commandScrollArea;
    }

    MainView* CentralWidget::mainView() const {
        return m_mainView;
    }

    void CentralWidget::setUndoMode(CentralWidget::UNDO_MODE undoMode) {
        m_undoMode = undoMode;
    }

    void CentralWidget::mousePressEvent(QMouseEvent* event) {
        startActionGlob();
    }

    void CentralWidget::mouseReleaseEvent(QMouseEvent* event) {
        stopActionGlob();
    }

    void CentralWidget::startActionGlob() {
        assert(m_undoMode != UNDO_MODE::GLOB);
        m_undoStack.emplace(new action::GlobAction);
        m_undoMode = UNDO_MODE::GLOB;
    }

    void CentralWidget::stopActionGlob() {
        assert(m_undoMode == UNDO_MODE::GLOB);
        assert(m_undoStack.top()->type() == action::ACTION_TYPE::GLOB);
        m_undoMode = UNDO_MODE::NORMAL;

        auto* globAction = dynamic_cast<action::GlobAction*>(m_undoStack.top().get());
        if (globAction->isEmpty()) {
            m_undoStack.pop();
        }
        if (globAction->single()) {
            addAction(globAction->first());
            m_undoStack.pop();
        }
    }

} // namespace view2