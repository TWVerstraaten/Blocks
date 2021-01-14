#include "MainWindow.h"

#include "MainView.h"

#include <QDockWidget>
#include <QKeyEvent>
#include <QTimer>
#include <memory>
namespace view2 {

    MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
        m_mainWidget = new MainWidget();
        setCentralWidget(m_mainWidget);
        setWindowTitle(tr("Transformations"));
        resize(1000, 800);

        QTimer::singleShot(0, this, &MainWindow::init);
    }

    void MainWindow::keyPressEvent(QKeyEvent* e) {
        switch (e->key()) {
            case Qt::Key_A:
                m_mainWidget->scrollArea()->add();
                break;
            case Qt::Key_R:
                m_mainWidget->scrollArea()->remove();
                break;
        }
    }

    void MainWindow::loop() {
        update();
    }

    void MainWindow::init() {
        auto timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::loop);
        timer->start(0);

        m_application = std::make_unique<app::Application>();
        m_mainWidget->mainView()->setCurrentModel(&m_application->model());
        m_application->run();

        update();
    }
} // namespace view2