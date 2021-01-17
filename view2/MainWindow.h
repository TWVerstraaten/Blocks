#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "CentralWidget.h"

#include <QDebug>
#include <QElapsedTimer>
#include <QMainWindow>
#include <QTime>

namespace view2 {

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
