#include "MainView.h"

#include "CentralWidget.h"

#include <QApplication>

namespace view {

    MainView::MainView(CentralWidget* centralWidget)
        : QWidget(centralWidget), m_centralWidget(centralWidget), m_commandScrollArea(centralWidget->commandScrollArea()), m_mainViewPainter(this),
          m_mainViewMouseManager(this) {
        m_model = std::make_unique<model::Model>();
        m_model->init();
        m_mainViewMouseManager.m_centralWidget = m_centralWidget;

        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    }

    void MainView::paintEvent(QPaintEvent* event) {
        QPainter painter;
        painter.begin(this);
        m_mainViewPainter.paint(painter, event);
        painter.end();
    }

    void MainView::wheelEvent(QWheelEvent* event) {
#ifdef Q_OS_LINUX
        m_viewPort.zoom(event->angleDelta().y(), event->pos());
#else
        m_viewPort.zoom(event->angleDelta().y(), event->position());
#endif
        update();
    }

    void MainView::mousePressEvent(QMouseEvent* event) {
        setFocus();
        m_mainViewMouseManager.mousePressEvent(event);
        update();
    }

    void MainView::mouseMoveEvent(QMouseEvent* event) {
        m_mainViewMouseManager.mouseMoveEvent(event);
        update();
    }

    model::Model* MainView::model() const {
        return m_model.get();
    }

} // namespace view