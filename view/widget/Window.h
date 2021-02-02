#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QElapsedTimer>
#include <QMainWindow>

namespace view::widget {

    class CentralWidget;

    class Window : public QMainWindow {
        Q_OBJECT

      public:
        explicit Window(QWidget* parent = nullptr);

      public slots:
        void toMainMenu();
        void startLevel(const std::string& path);
    };
} // namespace view

#endif // MAINWINDOW_H
