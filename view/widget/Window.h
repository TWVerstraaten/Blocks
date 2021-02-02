#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

namespace view::widget {

    class CentralWidget;

    class Window : public QMainWindow {
        Q_OBJECT

      public:
        explicit Window(QWidget* parent = nullptr);

        bool eventFilter(QObject* watched, QEvent* event) override;

      signals:
        void undo();
        void redo();

      public slots:
        void toMainMenu();
        void startLevel(const std::string& path);
    };
} // namespace view::widget

#endif // WINDOW_H
