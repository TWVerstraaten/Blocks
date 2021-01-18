#include "MainView.h"

#include "CentralWidget.h"
#include "action/DeleteClusterAction.h"
#include "action/MergeClusterAction.h"
#include "action/RemoveBlockAction.h"
#include "action/SplitDisconnectedAction.h"

#include <QApplication>

namespace view {

    MainView::MainView(CentralWidget* centralWidget)
        : QWidget(centralWidget), m_centralWidget(centralWidget), m_commandScrollArea(centralWidget->commandScrollArea()), m_mainViewPainter(this) {
        m_model = std::make_unique<model::Model>();
        m_model->init();

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
        //        m_centralWidget->startActionGlob();
        setFocus();
        m_previousMousePosition = event->pos();
        m_previousGridPosition  = model::GridXY::fromScreenXY(m_previousMousePosition, m_viewPort);
        if (event->button() == Qt::MouseButton::LeftButton) {
            mouseLeftPressEvent();
        }
    }

    void MainView::mouseMoveEvent(QMouseEvent* event) {
        const view::ScreenXY currentMousePosition = event->pos();
        const model::GridXY  currentGridPosition  = model::GridXY::fromScreenXY(currentMousePosition, m_viewPort);
        switch (event->buttons()) {
            case Qt::RightButton:
                m_viewPort.translate((event->x() - m_previousMousePosition.x()), event->y() - m_previousMousePosition.y());
                m_previousMousePosition = event->pos();
                break;
            case Qt::NoButton:
            case Qt::LeftButton:
                if (m_previousGridPosition != currentGridPosition) {
                    mouseLeftDragEvent(currentGridPosition);
                }
            case Qt::MiddleButton:
            default:
                break;
        }
        m_previousMousePosition = currentMousePosition;
        m_previousGridPosition  = currentGridPosition;
        update();
    }

    void MainView::mouseLeftPressEvent() {
        if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
            removeBlock(m_previousGridPosition);
        } else {
            createCluster(m_previousGridPosition);
        }
        repaint();
    }

    void MainView::mouseLeftDragEvent(const model::GridXY& currentGridXY) {
        if (not m_model->level().isFreeStartBlock(currentGridXY)) {
            return;
        }
        if (m_model->noLiveOrStoppedClusterOnBlock(m_previousGridPosition) || not currentGridXY.isAdjacent(m_previousGridPosition)) {
            m_previousGridPosition = currentGridXY;
            mouseLeftPressEvent();
        } else {
            auto baseIt      = m_model->clusterContaining(m_previousGridPosition);
            auto extensionIt = m_model->clusterContaining(currentGridXY);
            if (extensionIt != m_model->clusters().end() && baseIt->index() == extensionIt->index()) {
                return;
            }
            m_centralWidget->startActionGlob();
            createCluster(currentGridXY);
            extensionIt = m_model->clusterContaining(currentGridXY);
            if (baseIt->index() != extensionIt->index()) {
                assert(baseIt != m_model->clusters().end());
                assert(extensionIt != m_model->clusters().end());
                m_centralWidget->addAction(new action::MergeClusterAction(m_model.get(), *baseIt, *extensionIt, m_commandScrollArea));
            }
            m_centralWidget->stopActionGlob();
        }
    }

    void MainView::removeBlock(const model::GridXY& gridXy) {
        auto it = m_model->clusterContaining(gridXy);
        if (it == m_model->clusters().end()) {
            return;
        }

        if (it->size() == 1) {
            m_centralWidget->addAction(new action::DeleteClusterAction(m_centralWidget, *it));
        } else {
            m_centralWidget->startActionGlob();
            m_centralWidget->addAction(new action::RemoveBlockAction(m_model.get(), it->index(), gridXy));
            if (not it->isConnected()) {
                m_centralWidget->addAction(new action::SplitDisconnectedAction(m_model.get(), *it, m_commandScrollArea));
            }
            m_centralWidget->stopActionGlob();
            update();
        }
    }

    void MainView::createCluster(const model::GridXY& gridXy) {
        if (m_model->noLiveOrStoppedClusterOnBlock(gridXy) && m_model->level().isFreeStartBlock(gridXy)) {
            m_model->clusters().emplace_back(gridXy, "CL" + std::to_string(m_model->clusters().size()));
            assert(m_commandScrollArea);
            m_commandScrollArea->add(m_model->clusters().back());
            m_centralWidget->addAction(new action::NewClusterAction(m_centralWidget, m_model->clusters().back()));
            update();
        }
    }

    model::Model* MainView::model() const {
        return m_model.get();
    }

    void MainView::mouseReleaseEvent(QMouseEvent* event) {
    }
} // namespace view