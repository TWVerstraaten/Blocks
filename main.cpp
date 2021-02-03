#include "view/widget/Window.h"

#include <QApplication>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    QApplication app(argc, argv);
    auto         window = view::widget::Window();
    window.show();
    return app.exec();
}
