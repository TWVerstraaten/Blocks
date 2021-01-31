#include "Window.h"

#include "../../audio/AudioManager.h"
#include "CentralWidget.h"

namespace view {

    Window::Window(QWidget* parent) : QMainWindow(parent) {
        audio::AudioManager::init(this);

        m_centralWidget = new CentralWidget();
        setCentralWidget(m_centralWidget);
        setWindowTitle(tr("Blocks"));
        resize(1000, 800);
        m_elapsedTimer.start();
    }

} // namespace view