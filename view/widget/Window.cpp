#include "Window.h"

#include "../../audio/AudioManager.h"
#include "CentralWidget.h"
#include "MainMenuWidget.h"

namespace view::widget {

    Window::Window(QWidget* parent) : QMainWindow(parent) {
        audio::AudioManager::init(this);
        toMainMenu();
        setWindowTitle(tr("Blocks"));
        resize(1000, 800);
        m_elapsedTimer.start();
    }

    void Window::toMainMenu() {
        qDebug() << "toMainMenu";

        auto* w = new widget::MainMenuWidget(this);
        connect(w->levelSelectWidget(), &widget::LevelSelectWidget::levelSelected, this, &Window::startLevel);
        setCentralWidget(w);
    }

    void Window::startLevel(const std::string& path) {
        auto* c = new CentralWidget(path);
        connect(c, &CentralWidget::quit, this, &Window::toMainMenu);
        setCentralWidget(c);
    }

} // namespace view::widget