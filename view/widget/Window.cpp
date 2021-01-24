#include "Window.h"

#include "CentralWidget.h"

namespace view {

    Window::Window(QWidget* parent) : QMainWindow(parent) {
        m_centralWidget = new CentralWidget();
        setCentralWidget(m_centralWidget);
        setWindowTitle(tr("Blocks"));
        resize(1000, 800);
        m_elapsedTimer.start();
    }

} // namespace view