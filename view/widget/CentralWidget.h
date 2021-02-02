#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include "../../app/Application_constants.h"
#include "../../datstr/CircularBuffer.h"
#include "../../datstr/Stash.h"
#include "CentralWidget_enums.h"
#include "CommandScroll.h"
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

namespace view::widget {
    class Window;
    class CommandScroll;
    class BlockSelectWidget;

    class CentralWidget : public QWidget {
        Q_OBJECT

      public:
        CentralWidget(const std::string& path, Window* window);
        ~CentralWidget() override;

        void keyPressEvent(QKeyEvent* event) override;

        void mousePressEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;

        void addAction(action::Action* action);
        void startActionGlob();
        void stopActionGlob();

        void setWindow(Window* window);

        [[nodiscard]] BlockSelectWidget* blockSelectWidget() const;
        [[nodiscard]] CommandScroll*     commandScrollArea() const;
        [[nodiscard]] MainView*          mainView() const;
        [[nodiscard]] Window*            mainWindow() const;

      public slots:
        void undo();
        void redo();

      signals:
        void quit();

      protected:
        void paintEvent(QPaintEvent* event) override;

      private:
        void tryStart();
        void handleEscape();
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
        void saveLevel();

        enum class MODE { EDITING, RUNNING };

        MODE  m_mode  = MODE::EDITING;
        PHASE m_phase = PHASE::MOVE;

        size_t m_timeStep = app::TIME_STEP_SLOW;

        Window*                      m_window = nullptr;
        datstr::Stash<MainView>      m_mainViewStash;
        datstr::Stash<CommandScroll> m_commandScrollStash;
        QGridLayout*                 m_layout;
        QUndoStack                   m_qUndoStack;
        QUndoView*                   m_qUndoView;
        BlockSelectWidget*           m_blockSelectWidget;
        QElapsedTimer                m_phaseTimer;
        QElapsedTimer                m_elapsedTimer;
        QElapsedTimer                m_frameRateTimer;
        QLabel*                      m_frameRateLabel;
        datstr::CircularBuffer       m_circularBuffer{90};
    };

} // namespace view::widget

#endif //  CENTRALWIDGET_H
