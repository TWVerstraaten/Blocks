#include "MainWindow.h"

#include "MainView.h"

#include <QDockWidget>
#include <QKeyEvent>
#include <QTime>
#include <QTimer>
#include <memory>
namespace view2 {

    MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
        m_mainWidget = new CentralWidget();
        setCentralWidget(m_mainWidget);
        setWindowTitle(tr("Transformations"));
        resize(1000, 800);

        QTimer::singleShot(0, this, &MainWindow::init);
    }

    void MainWindow::loop() {
        const auto elapsedTime = m_elapsedTimer.elapsed();
        update();
        m_elapsedTimer.restart();
    }

    void MainWindow::init() {
        auto timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::loop);
        timer->start(0);

        m_model = std::make_unique<model::Model>();
        m_model->init();
        m_mainWidget->mainView()->init(m_mainWidget->commandScrollArea());
        m_elapsedTimer.start();
    }

} // namespace view2