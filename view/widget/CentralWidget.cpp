#include "CentralWidget.h"

#include "../../action/Action.h"
#include "../../misc/defines.h"
#include "../../model/Model.h"
#include "../View_constants.h"
#include "BlockSelectWidget.h"
#include "MainViewMouseManager.h"

#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <QUndoView>
#include <action/RemoveBlockFromClusterAction.h>
#include <action/SplitDisconnectedAction.h>

namespace view {

    CentralWidget::CentralWidget()
        : m_mainView(nullptr), m_commandScrollArea(nullptr), m_qUndoView(new QUndoView(&m_qUndoStack)),
          m_blockSelectWidget(new BlockSelectWidget(this)), m_frameRateLabel(new QLabel(this)) {
        setGeometry(0, 0, view::INITIAL_SCREEN_WIDTH, view::INITIAL_SCREEN_HEIGHT);

        m_layout = new QGridLayout;
        m_layout->setSpacing(0);
        m_layout->setMargin(0);
        setLayout(m_layout);

        m_commandScrollArea.set(new CommandScrollArea(this));
        m_mainView.set(new MainView(this));
        m_mainView->init();

        m_layout->addWidget(m_mainView.get(), 0, 0, 2, 2);
        m_mainView->stackUnder(m_blockSelectWidget);
        m_layout->addWidget(m_blockSelectWidget, 1, 0);
        m_layout->addWidget(m_commandScrollArea.get(), 0, 2, 2, 1);
        m_layout->addWidget(m_frameRateLabel, 2, 0, 1, 3);
        m_frameRateLabel->setFixedHeight(50);

        //        m_layout->addWidget(m_qUndoView, 0, 3, 2, 1);
        //        m_qUndoView->setMaximumWidth(250);
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
        return m_commandScrollArea.get();
    }

    MainView* CentralWidget::mainView() const {
        return m_mainView.get();
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
        //        qDebug() << elapsed;

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
        m_mainView.stashedValue()->setViewPort(m_mainView->viewPort());
        m_layout->removeWidget(m_mainView.get());
        m_layout->removeWidget(m_commandScrollArea.get());

        m_mainView.recover();
        m_commandScrollArea.recover();

        m_commandScrollArea->show();

        m_layout->addWidget(m_mainView.get(), 0, 0, 2, 2);
        m_mainView->stackUnder(m_blockSelectWidget);
        m_layout->addWidget(m_commandScrollArea.get(), 0, 2, 2, 1);

        update();
    }

    void CentralWidget::startRunning() {
        m_mode  = MODE::RUNNING;
        m_phase = PHASE::MOVE;

        m_blockSelectWidget->hide();

        m_layout->removeWidget(m_mainView.get());
        m_layout->removeWidget(m_commandScrollArea.get());

        m_mainView.stash();
        m_mainView.set(new MainView(this));
        m_commandScrollArea.stash();
        m_commandScrollArea.set(new CommandScrollArea(this));

        m_mainView->setViewPort(m_mainView.stashedValue()->viewPort());
        m_mainView->init(*m_mainView.stashedValue()->model());
        m_mainView->mainViewMouseManager().setBlockEditing(true);
        m_mainView->setCommandScrollArea(m_commandScrollArea.get());
        m_mainView->model()->resetPhase();
        m_commandScrollArea->addNeeded(m_mainView->model()->clusters());
        m_commandScrollArea->disable();

        m_layout->addWidget(m_mainView.get(), 0, 0, 2, 2);
        m_mainView->stackUnder(m_blockSelectWidget);
        m_layout->addWidget(m_commandScrollArea.get(), 0, 2, 2, 1);

        for (auto& cluster : m_mainView->model()->clusters()) {
            cluster.doCommand(*m_mainView->model());
            cluster.buildSides();
        }

        m_mainView->model()->update(0.0001);
        m_commandScrollArea->updateSelection();

        m_elapsedTimer.restart();
        m_phaseTimer.restart();

        mainLoop();
    }

    void CentralWidget::moveLoop(size_t elapsed) {
        auto& model = *m_mainView->model();
        model.update(1.1 * elapsed / static_cast<double>(m_timeStep));

        update();
    }

    void CentralWidget::interactLoop(size_t elapsed) {
        update();
    }

    void CentralWidget::endMovePhase() {
        startInteractPhase();
    }

    void CentralWidget::endInteractPhase() {
        startMovePhase();
    }

    void CentralWidget::startMovePhase() {
        m_mainView->model()->resetPhase();
        for (auto& cluster : m_mainView->model()->clusters()) {
            cluster.doCommand(*m_mainView->model());
            cluster.buildSides();
        }
        m_mainView->model()->update(0.0001);
        m_commandScrollArea->updateSelection();
    }

    void CentralWidget::startInteractPhase() {
        auto*       model    = m_mainView->model();
        auto&       clusters = model->clusters();
        const auto& level    = model->level();
        model->resetPhase();
        for (auto& cluster : model->clusters()) {
            cluster.incrementCommandIndex();
        }

        const auto& instantBlocks = level.instantBlocks();
        for (const auto& [point, type] : instantBlocks) {
            switch (type) {
                case model::INSTANT_BLOCK_TYPE::KILL:
                    for (auto& cluster : clusters) {
                        if (cluster.contains(point)) {
                            action::RemoveBlockFromClusterAction(model, cluster.index(), point).redo();
                            break;
                        }
                    }
                    break;
            }
        }
        model->clearEmpty();
        model->splitDisconnectedClusters();
        model->clearEmpty();
        m_commandScrollArea->addNeeded(clusters);

        m_commandScrollArea->updateSelection();
        update();
    }

    void CentralWidget::paintEvent(QPaintEvent* event) {
        QWidget::paintEvent(event);

        m_circularBuffer.add(m_frameRateTimer.elapsed());
        m_frameRateLabel->setText(QString("Ave:\t %1\n"
                                          "Max:\t %2\n"
                                          "Min:\t %3\n")
                                      .arg(1000.0 / static_cast<double>(m_circularBuffer.average()))
                                      .arg(1000.0 / static_cast<double>(m_circularBuffer.min()))
                                      .arg(1000.0 / static_cast<double>(m_circularBuffer.max())));
        m_frameRateTimer.restart();
    }

} // namespace view
