#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MainWidget.h"
#include "app/Application.h"

#include <QDebug>
#include <QMainWindow>

namespace view2 {

    class MainWindow : public QMainWindow {
        Q_OBJECT
      public:
        explicit MainWindow(QWidget* parent = nullptr);

      protected:
        virtual void keyPressEvent(QKeyEvent* e) override;

      private slots:
        void init();
        void loop();

      private:
        MainWidget*                       m_mainWidget;
        std::unique_ptr<app::Application> m_application{nullptr};

      signals:
    };
} // namespace view2

#endif // MAINWINDOW_H
