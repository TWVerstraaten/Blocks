#include "view/widget/MainWindow.h"

#include <QApplication>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    QApplication app(argc, argv);
    auto         Window = view::MainWindow();
    Window.show();
    return app.exec();
}
