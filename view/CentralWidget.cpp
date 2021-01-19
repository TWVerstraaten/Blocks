#include "CentralWidget.h"

#include "MainView.h"
#include "global/defines.h"

#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <QUndoView>

namespace view {

    CentralWidget::CentralWidget() : m_qUndoView(new QUndoView(&m_qUndoStack)), m_blockSelectWidget(new BlockSelectWidget(this)) {
        setGeometry(0, 0, 1000, 800);

        auto* layout = new QGridLayout;
        layout->setSpacing(0);
        layout->setMargin(0);
        setLayout(layout);

        m_commandScrollArea = new CommandScrollArea(this);
        m_mainView          = new MainView(this);

        layout->addWidget(m_mainView, 0, 0, 2, 2);
        m_mainView->stackUnder(m_blockSelectWidget);
        layout->addWidget(m_blockSelectWidget, 1, 0);
        layout->addWidget(m_commandScrollArea, 0, 2, 2, 1);

        //        layout->addWidget(m_qUndoView, 0, 3, 2, 1);
        //        m_qUndoView->setMaximumWidth(250);
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

    void CentralWidget::mousePressEvent([[maybe_unused]] QMouseEvent* event) {
        //        startActionGlob();
    }

    void CentralWidget::mouseReleaseEvent([[maybe_unused]] QMouseEvent* event) {
        //        stopActionGlob();
    }

    void CentralWidget::startActionGlob() {
        m_qUndoStack.beginMacro("Macro");
    }

    void CentralWidget::stopActionGlob() {
        m_qUndoStack.endMacro();
    }

    BlockSelectWidget* CentralWidget::blockSelectWidget() const {
        return m_blockSelectWidget;
    }
} // namespace view