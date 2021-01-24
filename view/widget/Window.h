#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QElapsedTimer>
#include <QMainWindow>

namespace view {

    class CentralWidget;

    class Window : public QMainWindow {
        Q_OBJECT

      public:
        explicit Window(QWidget* parent = nullptr);

      private:
        QElapsedTimer  m_elapsedTimer;
        CentralWidget* m_centralWidget;
    };
} // namespace view

#endif // MAINWINDOW_H