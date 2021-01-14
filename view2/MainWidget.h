#ifndef WINDOW_H
#define WINDOW_H

#include "CommandScrollArea.h"

#include <QList>
#include <QPainterPath>
#include <QWidget>

namespace model {
    class Model;
}

namespace view2 {
    class MainView;

    class MainWidget : public QWidget {
        Q_OBJECT

      public:
        MainWidget();

        void keyPressEvent(QKeyEvent* event) override;

        [[nodiscard]] MainView*          mainView() const;
        [[nodiscard]] CommandScrollArea* scrollArea() const;

      private:
        MainView*          m_mainView;
        CommandScrollArea* m_scrollArea;
    };
} // namespace view2

#endif
