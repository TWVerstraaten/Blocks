#include "MainView.h"

#include "CentralWidget.h"

namespace view {

    MainView::MainView(CentralWidget* centralWidget)
        : QWidget(centralWidget), m_centralWidget(centralWidget), m_commandScrollArea(centralWidget->commandScrollArea()), m_mainViewPainter(this),
          m_mainViewMouseManager(this) {
        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    }

    void MainView::init() {
        m_model = std::make_unique<model::Model>();
        m_model->init();
        m_mainViewMouseManager.m_centralWidget = m_centralWidget;
        m_mainViewMouseManager.m_model         = m_model.get();
    }

    void MainView::init(const model::Model& model) {
        m_model                                = std::make_unique<model::Model>(model);
        m_mainViewMouseManager.m_centralWidget = m_centralWidget;
        m_mainViewMouseManager.m_model         = m_model.get();
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

    MainViewMouseManager& MainView::mainViewMouseManager() {
        return m_mainViewMouseManager;
    }

    void MainView::startRunning() {
        std::swap(m_model, m_modelStash);
        m_model = std::make_unique<model::Model>(*m_modelStash);
    }

    void MainView::stopRunning() {
        std::swap(m_model, m_modelStash);
        m_modelStash.reset(nullptr);
    }

    void MainView::setCommandScrollArea(CommandScrollArea* commandScrollArea) {
        m_commandScrollArea = commandScrollArea;
    }

    CommandScrollArea* MainView::commandScrollArea() {
        return m_commandScrollArea;
    }

    void MainView::setModel(const std::unique_ptr<model::Model>& model) {
        m_model = std::make_unique<model::Model>(*model);
    }

    void MainView::setViewPort(const ViewPort& viewPort) {
        m_viewPort = viewPort;
    }

    const ViewPort& MainView::viewPort() const {
        return m_viewPort;
    }

} // namespace view