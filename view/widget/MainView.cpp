#include "MainView.h"

#include "../../contr/MainViewMouseManager.h"
#include "../../model/Model.h"
#include "CentralWidget.h"
#include "CommandScroll.h"
#include "MainViewPainter.h"

#include <QDebug>

namespace view::widget {

    MainView::MainView(CentralWidget* centralWidget)
        : QWidget(centralWidget), m_centralWidget(centralWidget), m_model(nullptr), m_commandScrollArea(centralWidget->commandScrollArea()),
          m_mainViewPainter(new MainViewPainter(this)), m_mainViewMouseManager(new contr::MainViewMouseManager(this)) {
        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    }

    void MainView::init(const std::string& path) {
        m_model = new model::Model{};
        m_model->init(path);
        m_commandScrollArea->addNeeded(m_model->clusters());
        m_mainViewMouseManager->setCentralWidget(m_centralWidget);
        m_mainViewMouseManager->setModel(m_model);
        m_model->level().buildSides();
    }

    void MainView::init(const model::Model& model) {
        m_model = new model::Model{model};
        m_mainViewMouseManager->setCentralWidget(m_centralWidget);
        m_mainViewMouseManager->setModel(m_model);
    }

    void MainView::paintEvent(QPaintEvent* event) {
        m_model->buildClusterSides();
        QPainter painter;
        painter.begin(this);
        m_mainViewPainter->paint(painter, event);
        painter.end();
    }

    void MainView::wheelEvent(QWheelEvent* event) {
#ifdef Q_OS_LINUX
        m_viewPort.zoom(event->angleDelta().y(), view::ScreenXy{event->pos()});
#else
        m_viewPort.zoom(event->angleDelta().y(), view::ScreenXy{event->position()});
#endif
        update();
    }

    void MainView::mousePressEvent(QMouseEvent* event) {
        setFocus();
        m_mainViewMouseManager->mousePressEvent(event);
        update();
    }

    void MainView::mouseMoveEvent(QMouseEvent* event) {
        m_mainViewMouseManager->mouseMoveEvent(event);
        update();
    }

    model::Model* MainView::model() const {
        return m_model;
    }

    contr::MainViewMouseManager& MainView::mainViewMouseManager() {
        return *m_mainViewMouseManager;
    }

    void MainView::setCommandScrollArea(CommandScroll* commandScrollArea) {
        m_commandScrollArea = commandScrollArea;
    }

    CommandScroll* MainView::commandScrollArea() {
        return m_commandScrollArea;
    }

    void MainView::setViewPort(const ViewPort& viewPort) {
        m_viewPort = viewPort;
    }

    const ViewPort& MainView::viewPort() const {
        return m_viewPort;
    }

    MainView::~MainView() {
        delete m_mainViewPainter;
        delete m_mainViewMouseManager;
        delete m_model;
    }

    ViewPort& MainView::viewPort() {
        return m_viewPort;
    }

} // namespace view::widget