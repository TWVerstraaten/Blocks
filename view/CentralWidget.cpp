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

        m_layout = new QGridLayout;
        m_layout->setSpacing(0);
        m_layout->setMargin(0);
        setLayout(m_layout);

        m_commandScrollArea = new CommandScrollArea(this);
        m_mainView          = new MainView(this);
        m_mainView->init();

        m_layout->addWidget(m_mainView, 0, 0, 2, 2);
        m_mainView->stackUnder(m_blockSelectWidget);
        m_layout->addWidget(m_blockSelectWidget, 1, 0);
        m_layout->addWidget(m_commandScrollArea, 0, 2, 2, 1);

        m_layout->addWidget(m_qUndoView, 0, 3, 2, 1);
        m_qUndoView->setMaximumWidth(250);
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
                tryStart();
                break;
            case Qt::Key_1:
                m_timeStep = app::TIME_STEP_SLOW;
                tryStart();
                break;
            case Qt::Key_2:
                m_timeStep = app::TIME_STEP_MEDIUM;
                tryStart();
                break;
            case Qt::Key_3:
                m_timeStep = app::TIME_STEP_FAST;
                tryStart();
                break;
            case Qt::Key_Escape:
                tryStop();
                break;
        }
    }

    void CentralWidget::addAction(action::Action* action) {
        if (action != nullptr && m_mode == MODE::EDITING) {
            m_qUndoStack.push(action);
        }
    }

    void CentralWidget::undo() {
        if (m_mode == MODE::EDITING) {
            m_qUndoStack.undo();
            update();
        }
    }

    void CentralWidget::redo() {
        if (m_mode == MODE::EDITING) {
            m_qUndoStack.redo();
            update();
        }
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

    void CentralWidget::tryStart() {
        if (m_mode == MODE::RUNNING) {
            return;
        }
        if (std::all_of(D_CIT(m_mainView->model()->clusters()), D_FUNC(cluster, cluster.commandVector().wellFormed()))) {
            startRunning();
        }
    }

    void CentralWidget::mainLoop() {
        if (m_mode == MODE::EDITING) {
            stopRunning();
            return;
        }

        const auto elapsed = m_elapsedTimer.elapsed();
        m_elapsedTimer.restart();
        qDebug() << elapsed;

        if (m_phase == PHASE::MOVE) {
            moveLoop(elapsed);
        } else if (m_phase == PHASE::INTERACT) {
            interactLoop(elapsed);
        }

        if (m_phaseTimer.elapsed() > m_timeStep) {
            togglePhase();
            m_elapsedTimer.restart();
            m_phaseTimer.restart();
        }
        update();
        QTimer::singleShot(0, this, &CentralWidget::mainLoop);
    }

    void CentralWidget::togglePhase() {
        if (m_phase == PHASE::MOVE) {
            endMovePhase();
            m_phase = PHASE::INTERACT;
        } else {
            endInteractPhase();
            m_phase = PHASE::MOVE;
        }
        qDebug() << (m_phase == PHASE::MOVE ? "Move" : "Interact");
    }

    void CentralWidget::tryStop() {
        if (m_mode == MODE::RUNNING) {
            m_mode = MODE::EDITING;
        }
    }

    void CentralWidget::stopRunning() {
        m_mode = MODE::EDITING;

        m_blockSelectWidget->show();
        m_mainViewStash->setViewPort(m_mainView->viewPort());
        m_layout->removeWidget(m_mainView);
        m_layout->removeWidget(m_commandScrollArea);
        delete m_mainView;
        delete m_commandScrollArea;
        m_mainView          = nullptr;
        m_commandScrollArea = nullptr;

        std::swap(m_mainView, m_mainViewStash);
        std::swap(m_commandScrollArea, m_commandScrollAreaStash);

        m_layout->addWidget(m_mainView, 0, 0, 2, 2);
        m_mainView->stackUnder(m_blockSelectWidget);
        m_layout->addWidget(m_commandScrollArea, 0, 2, 2, 1);

        update();
    }

    void CentralWidget::startRunning() {
        m_mode  = MODE::RUNNING;
        m_phase = PHASE::MOVE;

        m_blockSelectWidget->hide();

        m_layout->removeWidget(m_mainView);
        m_layout->removeWidget(m_commandScrollArea);

        std::swap(m_mainView, m_mainViewStash);
        std::swap(m_commandScrollArea, m_commandScrollAreaStash);

        m_commandScrollArea = new CommandScrollArea(this);
        m_mainView          = new MainView(this);
        m_mainView->setViewPort(m_mainViewStash->viewPort());
        m_mainView->init(*m_mainViewStash->model());
        m_mainView->mainViewMouseManager().setBlockEditing(true);
        m_mainView->setCommandScrollArea(m_commandScrollArea);
        m_commandScrollArea->addNeeded(m_mainView->model()->clusters());
        m_commandScrollArea->disable();

        m_layout->addWidget(m_mainView, 0, 0, 2, 2);
        m_mainView->stackUnder(m_blockSelectWidget);
        m_layout->addWidget(m_commandScrollArea, 0, 2, 2, 1);

        m_mainView->model()->resetPhase();
        for (auto& cluster : m_mainView->model()->clusters()) {
            cluster.doCommand(*m_mainView->model());
            cluster.buildSides();
        }

        m_mainView->model()->update(0.0001);
        m_elapsedTimer.restart();
        m_phaseTimer.restart();

        mainLoop();
    }

    void CentralWidget::moveLoop(size_t elapsed) {
        auto& model    = *m_mainView->model();
        auto& clusters = model.clusters();

        model.update(1.1 * elapsed / static_cast<double>(m_timeStep));
        m_commandScrollArea->updateSelection();

        update();
    }

    void CentralWidget::interactLoop(size_t elapsed) {
    }

    void CentralWidget::endMovePhase() {
        m_mainView->model()->resetPhase();
    }

    void CentralWidget::endInteractPhase() {
        m_mainView->model()->resetPhase();
        for (auto& cluster : m_mainView->model()->clusters()) {
            cluster.incrementCommandIndex();
            cluster.doCommand(*m_mainView->model());
            cluster.buildSides();
        }
    }

} // namespace view