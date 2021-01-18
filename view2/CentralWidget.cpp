#include "CentralWidget.h"

#include "../model/Model.h"
#include "MainView.h"

#include <QApplication>
#include <QKeyEvent>
#include <QTimer>
#include <QUndoView>
#include <global/defines.h>

namespace view2 {

    CentralWidget::CentralWidget() : m_qUndoView(new QUndoView(&m_qUndoStack)) {
        setGeometry(0, 0, 1000, 800);

        m_qUndoView->show();
        m_qUndoView->setGeometry(QRect{20, 20, 200, 200});
        auto* layout = new QGridLayout;
        layout->setSpacing(0);
        layout->setMargin(0);
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
                if (std::all_of(D_CIT(m_mainView->model()->clusters()), D_FUNC(cluster, cluster.commandVector().wellFormed()))) {
                    qDebug() << "Start :D";
                }
                break;
        }
    }

    void CentralWidget::addAction(action::Action* action) {
        if (action != nullptr) {
            m_qUndoStack.push(action);
        }
    }

    void CentralWidget::undo() {
        m_qUndoStack.undo();
    }

    void CentralWidget::redo() {
        m_qUndoStack.redo();
    }

    CommandScrollArea* CentralWidget::commandScrollArea() const {
        return m_commandScrollArea;
    }

    MainView* CentralWidget::mainView() const {
        return m_mainView;
    }

    void CentralWidget::mousePressEvent(QMouseEvent* event) {
        //        startActionGlob();
    }

    void CentralWidget::mouseReleaseEvent(QMouseEvent* event) {
        //        stopActionGlob();
    }

    void CentralWidget::startActionGlob() {
        m_qUndoStack.beginMacro("Macro");
    }

    void CentralWidget::stopActionGlob() {
        m_qUndoStack.endMacro();
    }
} // namespace view2