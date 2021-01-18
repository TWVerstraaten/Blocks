#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QElapsedTimer>
#include <QMainWindow>

namespace view2 {

    class CentralWidget;

    class MainWindow : public QMainWindow {
        Q_OBJECT

      public:
        explicit MainWindow(QWidget* parent = nullptr);

      private:
        QElapsedTimer  m_elapsedTimer;
        CentralWidget* m_centralWidget;
    };
} // namespace view2

#endif // MAINWINDOW_H
