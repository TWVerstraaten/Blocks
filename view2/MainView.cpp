#include "MainView.h"

#include "../app/Application_constants.h"
#include "../model/Model.h"
#include "../view/color.h"
#include "../view/toColor.h"
#include "view/View.h"

#include <QMouseEvent>
#include <QPainter>
#include <QTime>

namespace view2 {
    MainView::MainView(QWidget* parent) : QWidget(parent) {
        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    }

    void MainView::paintEvent(QPaintEvent* event) {
        if (not m_currentModel) {
            return;
        }
        QPainter painter;
        painter.begin(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(event->rect(), view::color::BACKGROUND_COLOR);

        const auto blockShrinkInScreen = m_viewPort.worldToScreenLength(app::BLOCK_SHRINK_IN_WORLD);
        const auto shrinkInScreenXY    = view::ScreenXY{blockShrinkInScreen, blockShrinkInScreen};
        const auto shrunkBlockSize     = m_viewPort.blockSizeInScreen() - 2 * blockShrinkInScreen;

        for (const auto& [point, type] : m_currentModel->level().floorBlocks()) {
            const auto position = view::ScreenXY::fromGridXY(point, m_viewPort) + shrinkInScreenXY;

            painter.setBrush({view::getColor(type)});
            painter.drawRect(QRect{position.x(), position.y(), shrunkBlockSize, shrunkBlockSize});
        }

        for (const auto& stoppedCluster : m_currentModel->level().stoppedClusters()) {
            drawConnected(stoppedCluster.gridXY(), view::color::DARK_GREY_COLOR, painter);
        }
        for (const auto& cluster : m_currentModel->clusters()) {
            drawConnected(cluster.gridXY(), view::color::CLUSTER_COLOR, painter);
        }
        for (const auto& [point, type] : m_currentModel->level().dynamicBlocks()) {
            const auto position = view::ScreenXY::fromGridXY(point, m_viewPort) + shrinkInScreenXY;
            switch (type) {
                case model::DYNAMIC_BLOCK_TYPE::ROTATE_CW:
                    painter.drawImage(QRect{position.x(), position.y(), shrunkBlockSize, shrunkBlockSize}, QImage("assets/arrow_cw.png"));
                    break;
                case model::DYNAMIC_BLOCK_TYPE::ROTATE_CCW:
                    painter.drawImage(QRect{position.x(), position.y(), shrunkBlockSize, shrunkBlockSize}, QImage("assets/arrow_ccw.png"));
                    break;
            }
        }
        for (const auto& [point, type] : m_currentModel->level().instantBlocks()) {
            const auto position = view::ScreenXY::fromGridXY(point, m_viewPort) + shrinkInScreenXY;
            switch (type) {
                case model::INSTANT_BLOCK_TYPE::KILL:
                    painter.drawImage(QRect{position.x(), position.y(), shrunkBlockSize, shrunkBlockSize}, QImage("assets/kill.png"));
                    break;
            }
        }

        painter.end();
    }

    void MainView::setCurrentModel(const model::Model* model) {
        m_currentModel = model;
    }

    void MainView::wheelEvent(QWheelEvent* event) {
        m_viewPort.zoom(event->angleDelta().y() / 3);
    }

    void MainView::mousePressEvent(QMouseEvent* event) {
        m_previousMousePosition = event->pos();
    }

    void MainView::mouseMoveEvent(QMouseEvent* event) {
        switch (event->buttons()) {

            case Qt::RightButton:
                m_viewPort.translate((event->x() - m_previousMousePosition.x()), event->y() - m_previousMousePosition.y());
                m_previousMousePosition = event->pos();
                break;
            case Qt::NoButton:
            case Qt::LeftButton:
            case Qt::MiddleButton:
            default:
                break;
        }
        m_previousMousePosition = event->pos();
    }

    void MainView::drawConnected(const model::GridXYSet& blocks, const QColor& color, QPainter& painter) const {
        const auto blockShrinkInScreen = m_viewPort.worldToScreenLength(app::BLOCK_SHRINK_IN_WORLD);
        const auto shrunkBlockSize     = m_viewPort.blockSizeInScreen() - 2 * blockShrinkInScreen;
        const auto shrinkInScreenXY    = view::ScreenXY{blockShrinkInScreen, blockShrinkInScreen};

        painter.setBrush(color);
        for (const auto& point : blocks) {
            const auto position = view::ScreenXY::fromGridXY(point, m_viewPort) + shrinkInScreenXY;

            painter.drawRect(QRect{position.x(), position.y(), shrunkBlockSize, shrunkBlockSize});
        }
    }
} // namespace view2