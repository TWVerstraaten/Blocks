#define SDL_MAIN_HANDLED

#include "app/Application.h"

#include <QApplication>
#include <view2/MainWindow.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {

    QApplication app(argc, argv);
    auto Window = view2::MainWindow();
    Window.show();
    return app.exec();
}
