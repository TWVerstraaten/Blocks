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

    class CentralWidget : public QWidget {
        Q_OBJECT

      public:
        CentralWidget();

        void keyPressEvent(QKeyEvent* event) override;

      private:
        MainView*          m_mainView;
        CommandScrollArea* m_commandScrollArea;
    };
} // namespace view2

#endif
