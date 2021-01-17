#include "CentralWidget.h"

#include "../model/Model.h"
#include "MainView.h"

#include <QApplication>
#include <QKeyEvent>
#include <QTimer>
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
            m_undoStack.push(std::move(action));
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

} // namespace view2