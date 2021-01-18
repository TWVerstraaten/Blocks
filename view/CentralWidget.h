#ifndef WINDOW_H
#define WINDOW_H

#include "BlockSelectWidget.h"
#include "CommandScrollArea.h"
#include "action/Action.h"

#include <QUndoStack>
#include <QWidget>

namespace model {
    class Model;
}

class QUndoView;

namespace view {
    class MainView;

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

        [[nodiscard]] CommandScrollArea* commandScrollArea() const;
        [[nodiscard]] MainView*          mainView() const;

      private:
        MainView*          m_mainView;
        CommandScrollArea* m_commandScrollArea;
        QUndoStack         m_qUndoStack;
        QUndoView*         m_qUndoView;
        BlockSelectWidget* m_blockSelectWidget;
    };
} // namespace view

#endif
