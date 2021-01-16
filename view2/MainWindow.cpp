#include "MainWindow.h"

#include "MainView.h"

#include <QDockWidget>
#include <QKeyEvent>
#include <QTimer>

namespace view2 {

    MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
        m_mainWidget = new CentralWidget();
        setCentralWidget(m_mainWidget);
        setWindowTitle(tr("Blocks"));
        resize(1000, 800);

        m_elapsedTimer.start();

        setEnabled(false);
        auto timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::loop);
        timer->start(0);
        m_mainWidget->mainView()->init(m_mainWidget->commandScrollArea());
        setEnabled(true);
    }

    void MainWindow::loop() {
        const auto elapsedTime = m_elapsedTimer.elapsed();
        update();
        m_elapsedTimer.restart();
    }

    void MainWindow::init() {
    }

} // namespace view2