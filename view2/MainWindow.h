#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "CentralWidget.h"
#include "app/Application.h"

#include <QDebug>
#include <QElapsedTimer>
#include <QMainWindow>
#include <QTime>

namespace view2 {

    class MainWindow : public QMainWindow {
        Q_OBJECT
      public:
        explicit MainWindow(QWidget* parent = nullptr);

      private slots:
        void init();
        void loop();

      private:
        QElapsedTimer                 m_elapsedTimer{};
        CentralWidget*                m_mainWidget;
        std::unique_ptr<model::Model> m_model;
    };
} // namespace view2

#endif // MAINWINDOW_H
