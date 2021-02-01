#include "Window.h"

#include "../../audio/AudioManager.h"
#include "CentralWidget.h"
#include "MainMenuWidget.h"

namespace view {

    Window::Window(QWidget* parent) : QMainWindow(parent) {
        audio::AudioManager::init(this);

        //        m_centralWidget = new CentralWidget();
        //        connect(m_centralWidget, &CentralWidget::quit, this, &Window::toMainMenu);
        //        setCentralWidget(m_centralWidget);
        toMainMenu();
        setWindowTitle(tr("Blocks"));
        resize(1000, 800);
        m_elapsedTimer.start();
    }

    void Window::toMainMenu() {
        qDebug() << "toMainMenu";

        auto* w = new widget::MainMenuWidget(this);
        setCentralWidget(w);
    }

} // namespace view