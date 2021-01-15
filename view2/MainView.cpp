#include "MainView.h"

#include "../view/toColor.h"

#include <QApplication>
#include <QFontDatabase>
#include <QMouseEvent>
#include <QPainter>
#include <global/geom.h>

namespace view2 {
    MainView::MainView(QWidget* parent) : QWidget(parent) {
        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    }

    void MainView::paintEvent(QPaintEvent* event) {
        if (not m_isInitialized) {
            return;
        }
        QPainter painter;
        painter.begin(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(event->rect(), view::color::BACKGROUND_COLOR);

        const auto blockShrinkInScreen = m_viewPort.worldToScreenLength(app::BLOCK_SHRINK_IN_WORLD);
        const auto shrinkInScreenXY    = view::ScreenXY{blockShrinkInScreen, blockShrinkInScreen};
        const auto shrunkBlockSize     = m_viewPort.blockSizeInScreen() - 2 * blockShrinkInScreen;

        for (const auto& [point, type] : m_model->level().floorBlocks()) {
            const auto position = view::ScreenXY::fromGridXY(point, m_viewPort) + shrinkInScreenXY;

            painter.setBrush({view::getColor(type)});
            painter.drawRect(QRect{position.x(), position.y(), shrunkBlockSize, shrunkBlockSize});
        }

        for (const auto& stoppedCluster : m_model->level().stoppedClusters()) {
            drawConnected(stoppedCluster.gridXY(), view::color::DARK_GREY_COLOR, painter);
        }
        for (const auto& cluster : m_model->clusters()) {
            drawConnected(cluster.gridXY(), view::color::CLUSTER_COLOR, painter);
            const auto namePosition = view::ScreenXY::fromWorldXY(*cluster.gridXY().begin() + app::HALF_BLOCK_IN_WORLD, m_viewPort);

            const int     id     = QFontDatabase::addApplicationFont("assets/UbuntuMono-Italic.ttf");
            const QString family = QFontDatabase::applicationFontFamilies(id).at(0);
            const QFont   font(family, 12);
            painter.setFont(font);

            QFontMetrics fontMetrics(font);
            int          width  = fontMetrics.horizontalAdvance(cluster.name().c_str());
            int          height = fontMetrics.height();
            painter.fillRect(namePosition.x() - 4, namePosition.y() + 2 - height, width + 8, height + 4, view::color::WHITE_COLOR);
            painter.drawText(namePosition.x(), namePosition.y(), cluster.name().c_str());
        }
        for (const auto& [point, type] : m_model->level().dynamicBlocks()) {
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
        for (const auto& [point, type] : m_model->level().instantBlocks()) {
            const auto position = view::ScreenXY::fromGridXY(point, m_viewPort) + shrinkInScreenXY;
            switch (type) {
                case model::INSTANT_BLOCK_TYPE::KILL:
                    painter.drawImage(QRect{position.x(), position.y(), shrunkBlockSize, shrunkBlockSize}, QImage("assets/kill.png"));
                    break;
            }
        }
        painter.end();
    }

    void MainView::wheelEvent(QWheelEvent* event) {
        m_viewPort.zoom(event->angleDelta().y(), event->position());
    }

    void MainView::mousePressEvent(QMouseEvent* event) {
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
    }

    void MainView::drawConnected(const model::GridXYSet& blocks, const QColor& color, QPainter& painter) const {
        const auto pixmap  = connectedPixmap(blocks, color);
        const auto topLeft = view::ScreenXY::fromWorldXY(model::WorldXY(model::GridXY{geom::minX(blocks), geom::minY(blocks)}), m_viewPort);

        painter.drawPixmap(topLeft.x(), topLeft.y(), *pixmap);
    }

    void MainView::init(CommandScrollArea* commandScrollArea) {
        m_model = std::make_unique<model::Model>();
        m_model->init();
        m_commandScrollArea = commandScrollArea;
        m_isInitialized     = true;
    }

    void MainView::mouseLeftPressEvent() {
        if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
            removeBlock(m_previousGridPosition);
        } else {
            addBlock(m_previousGridPosition);
        }
    }

    void MainView::mouseLeftDragEvent(const model::GridXY& currentGridXY) {
        if (not m_model->level().isFreeStartBlock(currentGridXY)) {
            return;
        }
        if (m_model->noLiveOrStoppedClusterOnBlock(m_previousGridPosition)) {
            m_previousGridPosition = currentGridXY;
            mouseLeftPressEvent();
            return;
        }
        addBlock(currentGridXY);
        auto baseIt      = m_model->clusterContaining(m_previousGridPosition);
        auto extensionIt = m_model->clusterContaining(currentGridXY);

        assert(baseIt != m_model->clusters().end());
        assert(extensionIt != m_model->clusters().end());

        baseIt->gridXY().merge(extensionIt->gridXY());
        m_model->clearEmpty();
        m_commandScrollArea->removeUnnecessary(m_model->clusters());
    }

    void MainView::removeBlock(const model::GridXY& gridXy) {
        auto it = m_model->clusterContaining(gridXy);
        if (it == m_model->clusters().end()) {
            return;
        }

        it->removeBLock(gridXy);
        if (it->isEmpty()) {
            m_model->clearEmpty();
            m_commandScrollArea->removeUnnecessary(m_model->clusters());
            return;
        }
        if (it->isConnected()) {
            return;
        }

        auto newClusters = it->collectAllButFirstComponent();
        for (auto& newCluster : newClusters) {
            m_commandScrollArea->add(newCluster);
        }

        m_model->clusters().splice(m_model->clusters().end(), newClusters);
        m_model->clearEmpty();
        m_commandScrollArea->removeUnnecessary(m_model->clusters());
    }

    void MainView::addBlock(const model::GridXY& gridXy) {
        if (m_model->noLiveOrStoppedClusterOnBlock(gridXy) && m_model->level().isFreeStartBlock(gridXy)) {
            m_model->clusters().emplace_back(gridXy, "CL" + std::to_string(m_model->clusters().size()));
            m_commandScrollArea->add(m_model->clusters().back());
        }
    }

    std::unique_ptr<QPixmap> MainView::connectedPixmap(const model::GridXYSet& blocks, const QColor& color) const {
        const auto shrunkSize  = m_viewPort.blockSizeInScreen() - 2 * m_viewPort.worldToScreenLength(app::BLOCK_SHRINK_IN_WORLD);
        const auto minX        = geom::minX(blocks);
        const auto minY        = geom::minY(blocks);
        const auto maxX        = geom::maxX(blocks);
        const auto maxY        = geom::maxY(blocks);
        const auto width       = (maxX - minX + 1) * m_viewPort.blockSizeInScreen();
        const auto height      = (maxY - minY + 1) * m_viewPort.blockSizeInScreen();
        const auto twiceShrink = 2 * m_viewPort.worldToScreenLength(app::BLOCK_SHRINK_IN_WORLD);

        assert(width > 0);
        assert(height > 0);

        auto* result = new QPixmap{width + 20, height + 20};
        result->fill(Qt::transparent);
        QPainter painter(result);
        painter.setBrush(QBrush{color});
        using namespace model;
        for (const auto block : blocks) {
            const auto           positionInWorld = block - GridXY{minX, minY} + WorldXY{app::BLOCK_SHRINK_IN_WORLD, app::BLOCK_SHRINK_IN_WORLD};
            const view::ScreenXY position        = {m_viewPort.worldToScreenLength(positionInWorld.x()),
                                             m_viewPort.worldToScreenLength(positionInWorld.y())};
            painter.fillRect(QRect{position.x(), position.y(), shrunkSize, shrunkSize}, QBrush{color});
            const bool l = blocks.find(block.neighbor(GridXY::DIRECTION::LEFT)) != blocks.end();
            const bool u = blocks.find(block.neighbor(GridXY::DIRECTION::UP)) != blocks.end();
            if (u && l) {
                const bool ul = blocks.find(block.neighbor(GridXY::DIRECTION::UP).neighbor(GridXY::DIRECTION::LEFT)) != blocks.end();
                if (ul) {
                    painter.fillRect(QRect{position.x() - twiceShrink - 1, position.y() - twiceShrink - 1, twiceShrink + 2, twiceShrink + 2}, color);
                }
            }
            if (u) {
                painter.fillRect(QRect{position.x(), position.y() - twiceShrink - 1, shrunkSize, twiceShrink + 2}, color);
            }
            if (l) {
                painter.fillRect(QRect{position.x() - twiceShrink - 1, position.y(), twiceShrink + 2, shrunkSize}, color);
            }
        }
        return std::unique_ptr<QPixmap>(result);
    }

} // namespace view2