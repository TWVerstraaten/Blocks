#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QElapsedTimer>
#include <QMainWindow>

namespace view {

    class CentralWidget;

    class MainWindow : public QMainWindow {
        Q_OBJECT

      public:
        explicit MainWindow(QWidget* parent = nullptr);

      private:
        QElapsedTimer  m_elapsedTimer;
        CentralWidget* m_centralWidget;
    };
} // namespace view

#endif // MAINWINDOW_H
