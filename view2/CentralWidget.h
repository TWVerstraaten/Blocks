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

class QUndoView;

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

        void addAction(action::Action* action);
        void startActionGlob();
        void stopActionGlob();
        void undo();
        void redo();

        [[nodiscard]] CommandScrollArea* commandScrollArea() const;
        [[nodiscard]] MainView*          mainView() const;

      private:
        MainView*          m_mainView;
        CommandScrollArea* m_commandScrollArea;
        QUndoStack         m_qUndoStack;
        QUndoView*         m_qUndoView;
    };
} // namespace view2

#endif
