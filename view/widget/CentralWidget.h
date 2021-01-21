#ifndef WINDOW_H
#define WINDOW_H

#include "../../app/Application_constants.h"
#include "../../datstr/CircularBuffer.h"
#include "../../datstr/Stash.h"
#include "CentralWidget_enums.h"
#include "CommandScrollArea.h"
#include "MainView.h"

#include <QElapsedTimer>
#include <QGridLayout>
#include <QLabel>
#include <QUndoStack>
#include <QWidget>

class QUndoView;

namespace model {
    class Model;
}

namespace action {
    class Action;
}

namespace view {
    class CommandScrollArea;
    class BlockSelectWidget;

    class CentralWidget : public QWidget {
        Q_OBJECT

      public:
        CentralWidget();

        void keyPressEvent(QKeyEvent* event) override;

        void mousePressEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;

        void addAction(action::Action* action);
        void startActionGlob();
        void stopActionGlob();
        void undo();
        void redo();

        [[nodiscard]] BlockSelectWidget* blockSelectWidget() const;
        [[nodiscard]] CommandScrollArea* commandScrollArea() const;
        [[nodiscard]] MainView*          mainView() const;

      protected:
        void paintEvent(QPaintEvent* event) override;

      private:
        void tryStart();
        void tryStop();
        void startRunning();
        void stopRunning();
        void mainLoop();
        void togglePhase();
        void endMovePhase();
        void endInteractPhase();
        void startMovePhase();
        void startInteractPhase();
        void moveLoop(size_t elapsed);
        void interactLoop(size_t elapsed);

        enum class MODE { EDITING, RUNNING };

        MODE  m_mode  = MODE::EDITING;
        PHASE m_phase = PHASE::MOVE;

        size_t m_timeStep = app::TIME_STEP_SLOW;

        datstr::Stash<MainView>          m_mainView;
        datstr::Stash<CommandScrollArea> m_commandScrollArea;
        QGridLayout*                     m_layout;
        QUndoStack                       m_qUndoStack;
        QUndoView*                       m_qUndoView;
        BlockSelectWidget*               m_blockSelectWidget;
        QElapsedTimer                    m_phaseTimer;
        QElapsedTimer                    m_elapsedTimer;
        QElapsedTimer                    m_frameRateTimer;
        QLabel*                          m_frameRateLabel;
        datstr::CircularBuffer           m_circularBuffer{50};
    };

} // namespace view

#endif
