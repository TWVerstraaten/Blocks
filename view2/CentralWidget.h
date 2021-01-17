#ifndef WINDOW_H
#define WINDOW_H

#include "CommandScrollArea.h"

#include <QList>
#include <QPainterPath>
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
        CentralWidget();

        void keyPressEvent(QKeyEvent* event) override;

        void addAction(std::unique_ptr<action::Action>&& action);
        void undo();
        void redo();

        [[nodiscard]] CommandScrollArea* commandScrollArea() const;
        [[nodiscard]] MainView*          mainView() const;

      private:
        MainView*                                   m_mainView;
        CommandScrollArea*                          m_commandScrollArea;
        std::stack<std::unique_ptr<action::Action>> m_undoStack;
        std::stack<std::unique_ptr<action::Action>> m_redoStack;
    };
} // namespace view2

#endif
