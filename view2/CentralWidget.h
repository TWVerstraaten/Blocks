#ifndef WINDOW_H
#define WINDOW_H

#include "CommandScrollArea.h"

#include <QList>
#include <QPainterPath>
#include <QUndoStack>
#include <QWidget>
#include <action/Action.h>
#include <memory>
#include <stack>

namespace model {
    class Model;
}

namespace view2 {
    class MainView;

    class CentralWidget : public QWidget {
        Q_OBJECT

      public:
        enum class UNDO_MODE { NORMAL, GLOB };

        CentralWidget();

        void keyPressEvent(QKeyEvent* event) override;

        void mousePressEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;

        void addAction(std::unique_ptr<action::Action>&& action);
        void startActionGlob();
        void stopActionGlob();
        void undo();
        void redo();

        [[nodiscard]] CommandScrollArea* commandScrollArea() const;
        [[nodiscard]] MainView*          mainView() const;

        void setUndoMode(UNDO_MODE undoMode);

      private:
        UNDO_MODE                                   m_undoMode = UNDO_MODE::NORMAL;
        MainView*                                   m_mainView;
        CommandScrollArea*                          m_commandScrollArea;
        std::stack<std::unique_ptr<action::Action>> m_undoStack;
        std::stack<std::unique_ptr<action::Action>> m_redoStack;
        QUndoStack                                  m_qUndoStack;
    };
} // namespace view2

#endif
