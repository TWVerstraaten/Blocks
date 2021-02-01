#include "view/widget/Window.h"

#include <QApplication>
#include <iostream>

class A {
  public:
    virtual void f() {
        std::cout << "A" << '\n';
    }
};

class B : public A {
  public:
    void f() override {
        std::cout << "B" << '\n';
    }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    QApplication app(argc, argv);
    auto         Window = view::Window();
    Window.show();
    return app.exec();
}
