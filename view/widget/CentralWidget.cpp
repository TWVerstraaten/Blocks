#include "CentralWidget.h"

#include "../../Io/Serializer.h"
#include "../../action/RemoveBlockFromClusterAction.h"
#include "../../audio/AudioManager.h"
#include "../../contr/MainInterface.h"
#include "../../contr/MainViewMouseManager.h"
#include "../../misc/defines.h"
#include "../View_constants.h"
#include "BlockSelectWidget.h"

#include <QApplication>
#include <QTimer>
#include <QUndoView>

namespace view::widget {

    CentralWidget::CentralWidget(const std::string& path, Window* window)
        : m_window(window), m_mainViewStash(nullptr), m_commandScrollStash(nullptr), m_qUndoView(new QUndoView(&m_qUndoStack)),
          m_blockSelectWidget(new BlockSelectWidget(this)), m_frameRateLabel(new QLabel(this)) {
        setGeometry(0, 0, view::INITIAL_SCREEN_WIDTH, view::INITIAL_SCREEN_HEIGHT);

        m_layout = new QGridLayout;
        m_layout->setSpacing(0);
        m_layout->setMargin(0);
        setLayout(m_layout);

        m_commandScrollStash.set(new CommandScroll(this));
        m_commandScrollStash->setShouldStashCommandEditBoxes(true);
        m_mainViewStash.set(new MainView(this));
        m_mainViewStash->init(path);
        m_commandScrollStash->addNeeded(m_mainViewStash->model()->clusters());

        m_layout->addWidget(m_mainViewStash.get(), 0, 0, 2, 2);
        m_layout->addWidget(m_blockSelectWidget, 1, 0);
        m_layout->addWidget(m_commandScrollStash.get(), 0, 2, 2, 1);
        m_layout->addWidget(m_frameRateLabel, 2, 0, 1, 3);
        m_frameRateLabel->setFixedHeight(50);
        m_blockSelectWidget->raise();

        m_qUndoStack.clear();

        //        m_layout->addWidget(m_qUndoView, 0, 3, 2, 1);
        //        m_qUndoView->setMaximumWidth(250);
    }

    CentralWidget::~CentralWidget() {
        delete m_qUndoView;
    }

    void CentralWidget::keyPressEvent(QKeyEvent* event) {
        if ((QApplication::keyboardModifiers() & Qt::ControlModifier) && event->key() == Qt::Key_E) {
            using namespace Io;
            std::cout << "===========================================================\n";
            std::cout << *m_mainViewStash->model() << '\n';
            std::cout << "===========================================================\n";
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
                handleEscape();
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

    CommandScroll* CentralWidget::commandScrollArea() const {
        return m_commandScrollStash.get();
    }

    MainView* CentralWidget::mainView() const {
        return m_mainViewStash.get();
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
        if (std::all_of(D_CIT(m_mainViewStash->model()->clusters()), D_FUNC(cluster, cluster.commandVector().wellFormed()))) {
            startRunning();
        }
    }

    void CentralWidget::mainLoop() {
        const auto elapsed = m_elapsedTimer.elapsed();
        m_elapsedTimer.restart();

        if (m_mode == MODE::EDITING) {
            return;
        }

        if (m_phase == PHASE::MOVE) {
            moveLoop(elapsed);
        } else if (m_phase == PHASE::INTERACT) {
            interactLoop(elapsed);
        }
        if (m_phaseTimer.elapsed() > m_timeStep) {
            qDebug() << m_phaseTimer.elapsed();
            m_elapsedTimer.restart();
            m_phaseTimer.restart();

            togglePhase();
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
    }

    void CentralWidget::handleEscape() {
        if (m_mode == MODE::RUNNING) {
            m_mode = MODE::EDITING;
            stopRunning();
        } else {
            emit stopLevel();
        }
    }

    void CentralWidget::stopRunning() {
        m_mode = MODE::EDITING;

        m_blockSelectWidget->show();
        m_mainViewStash.stashedValue()->setViewPort(m_mainViewStash->viewPort());
        m_layout->removeWidget(m_mainViewStash.get());
        m_layout->removeWidget(m_commandScrollStash.get());

        m_mainViewStash.recover();
        m_commandScrollStash.recover();

        m_commandScrollStash->show();

        m_layout->addWidget(m_mainViewStash.get(), 0, 0, 2, 2);
        m_mainViewStash->stackUnder(m_blockSelectWidget);
        m_layout->addWidget(m_commandScrollStash.get(), 0, 2, 2, 1);

        update();
    }

    void CentralWidget::startRunning() {
        m_mode  = MODE::RUNNING;
        m_phase = PHASE::MOVE;

        m_blockSelectWidget->hide();

        m_layout->removeWidget(m_mainViewStash.get());
        m_layout->removeWidget(m_commandScrollStash.get());

        m_mainViewStash.stash();
        m_mainViewStash.set(new MainView(this));
        m_commandScrollStash.stash();
        m_commandScrollStash.set(new CommandScroll(this));

        m_mainViewStash->setViewPort(m_mainViewStash.stashedValue()->viewPort());
        m_mainViewStash->init(*m_mainViewStash.stashedValue()->model());
        m_mainViewStash->mainViewMouseManager().setBlockEditing(true);
        m_mainViewStash->setCommandScrollArea(m_commandScrollStash.get());
        m_mainViewStash->model()->resetPhase();
        m_commandScrollStash->addNeeded(m_mainViewStash->model()->clusters());
        m_commandScrollStash->disable();

        m_layout->addWidget(m_mainViewStash.get(), 0, 0, 2, 2);
        m_layout->addWidget(m_commandScrollStash.get(), 0, 2, 2, 1);
        m_blockSelectWidget->raise();

        m_elapsedTimer.restart();
        m_phaseTimer.restart();
        endInteractPhase();

        mainLoop();
    }

    void CentralWidget::moveLoop(size_t elapsed) {
        auto& model = *m_mainViewStash->model();
        model.update(1.1 * elapsed / static_cast<double>(m_timeStep));

        update();
    }

    void CentralWidget::interactLoop(size_t elapsed) {
        auto& model = *m_mainViewStash->model();
        model.update(1.1 * elapsed / static_cast<double>(m_timeStep));

        update();
    }

    void CentralWidget::endMovePhase() {
        if (m_timeStep != app::TIME_STEP_FAST) {
            audio::AudioManager::play(audio::SOUNDS::CLICK);
        }
        for (auto& cluster : m_mainViewStash->model()->clusters()) {
            cluster.incrementCommandIndex();
        }
        startInteractPhase();
    }

    void CentralWidget::endInteractPhase() {
        if (m_timeStep != app::TIME_STEP_FAST) {
            audio::AudioManager::play(audio::SOUNDS::CLICK);
        }
        startMovePhase();
    }

    void CentralWidget::startMovePhase() {
        contr::MainInterface::startMovePhase(*m_mainViewStash->model(), *m_commandScrollStash);
    }

    void CentralWidget::startInteractPhase() {
        contr::MainInterface::startInteractPhase(*m_mainViewStash->model(), *m_commandScrollStash);
        update();
    }

    void CentralWidget::paintEvent(QPaintEvent* event) {
        QWidget::paintEvent(event);

        m_circularBuffer.add(m_frameRateTimer.elapsed());
        m_frameRateLabel->setText(QString("Ave:\t %1\n"
                                          "Max:\t %2\n"
                                          "Min:\t %3\n")
                                      .arg(1000.0 / m_circularBuffer.average())
                                      .arg(1000.0 / m_circularBuffer.min())
                                      .arg(1000.0 / m_circularBuffer.max()));
        m_frameRateTimer.restart();
    }

    void CentralWidget::saveLevel() {
        using namespace Io;

        std::ofstream levelFile;
        levelFile.open("levels/level1.lev", std::fstream::trunc);
        levelFile << *mainView()->model();
        levelFile.close();
    }

    Window* CentralWidget::mainWindow() const {
        return m_window;
    }

    void CentralWidget::setWindow(Window* window) {
        m_window = window;
    }

} // namespace view::widget
