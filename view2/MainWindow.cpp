#include "MainWindow.h"

#include "MainView.h"

#include <QDockWidget>
#include <QTimer>

namespace view2 {

    MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
        m_centralWidget = new CentralWidget();
        setCentralWidget(m_centralWidget);
        setWindowTitle(tr("Blocks"));
        resize(1000, 800);

        m_elapsedTimer.start();
    }



} // namespace view2