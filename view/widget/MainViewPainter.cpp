//
// Created by teunv on 1/18/2021.
//

#include "MainViewPainter.h"

#include "../../misc/AlignedRectangle.h"
#include "../../misc/defines.h"
#include "../../misc/geom.h"
#include "../../model/Model.h"
#include "../FontManager.h"
#include "../PixmapManager.h"
#include "../toColor.h"
#include "MainView.h"

#include <QDebug>

namespace view {

    using namespace model;

    MainViewPainter::MainViewPainter(const MainView* mainView) : m_mainView(mainView), m_viewPort(&m_mainView->m_viewPort) {
    }

    void MainViewPainter::paint(QPainter& painter, QPaintEvent* event) {
        paint(painter, event->rect(), *m_mainView->model(), m_mainView->viewPort());
    }

    void MainViewPainter::drawConnected(const GridXyVector& blocks, const QColor& color, QPainter& painter, const ViewPort& viewPort) {
        const auto topLeft = ScreenXy::fromWorldXy(WorldXy(GridXy{geom::minX(blocks), geom::minY(blocks)}), viewPort);
        const auto pixmap  = connectedPixmap(blocks, color, viewPort);
        painter.drawPixmap(topLeft.x(), topLeft.y(), pixmap);
    }

    void MainViewPainter::paint(QPainter& painter, const QRect& rect, const Model& model, const ViewPort& viewPort, bool paintNames) {
        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(rect, color::BACKGROUND);

        const auto blockShrinkInScreen = viewPort.worldToScreen(app::BLOCK_SHRINK_IN_WORLD);
        const auto shrinkInScreenXy    = ScreenXy{blockShrinkInScreen, blockShrinkInScreen};
        const auto shrunkBlockSize     = viewPort.blockSizeInScreen() - 2 * blockShrinkInScreen;

        for (const auto& [point, type] : model.level().floorBlocks()) {
            const auto position = ScreenXy::fromGridXy(point, viewPort) + shrinkInScreenXy;
            painter.setBrush({toColor(type)});
            painter.drawRect(QRect{position.x(), position.y(), shrunkBlockSize, shrunkBlockSize});
        }
        for (const auto& stoppedCluster : model.level().stoppedClusters()) {
            drawConnected(stoppedCluster.gridXyVector(), color::CLUSTER_STOPPED, painter, viewPort);
        }
        for (const auto& cluster : model.clusters()) {
            drawCluster(cluster, painter, viewPort, paintNames);
        }
        for (const auto& [point, type] : model.level().dynamicBlocks()) {
            const auto position = ScreenXy::fromGridXy(point, viewPort) + shrinkInScreenXy;
            painter.drawPixmap(QRect{position.x(), position.y(), shrunkBlockSize, shrunkBlockSize},
                               PixmapManager::pixmap(type, QSize{shrunkBlockSize, shrunkBlockSize}));
        }
        for (const auto& [point, type] : model.level().instantBlocks()) {
            const auto position = ScreenXy::fromGridXy(point, viewPort) + shrinkInScreenXy;
            painter.drawPixmap(QRect{position.x(), position.y(), shrunkBlockSize, shrunkBlockSize},
                               PixmapManager::pixmap(type, QSize{shrunkBlockSize, shrunkBlockSize}));
        }
    }

    QPixmap MainViewPainter::connectedPixmap(const GridXyVector& blocks, const QColor& color, const ViewPort& viewPort) {
        using namespace model;

        const auto shrunkSize  = viewPort.blockSizeInScreen() - 2 * viewPort.worldToScreen(app::BLOCK_SHRINK_IN_WORLD);
        const auto twiceShrink = 2 * viewPort.worldToScreen(app::BLOCK_SHRINK_IN_WORLD);
        const auto minX        = geom::minX(blocks);
        const auto minY        = geom::minY(blocks);
        const auto maxX        = geom::maxX(blocks);
        const auto maxY        = geom::maxY(blocks);
        const auto width       = (maxX - minX + 1) * viewPort.blockSizeInScreen();
        const auto height      = (maxY - minY + 1) * viewPort.blockSizeInScreen();

        assert(width > 0);
        assert(height > 0);

        QPixmap result{width + 20, height + 20};
        result.fill(Qt::transparent);
        QPainter painter(&result);
        painter.setBrush(color);
        for (const auto block : blocks) {
            const auto positionInWorld = block - GridXy{minX, minY} + WorldXy{app::BLOCK_SHRINK_IN_WORLD, app::BLOCK_SHRINK_IN_WORLD};
            const auto position        = ScreenXy{viewPort.worldToScreen(positionInWorld.x()), viewPort.worldToScreen(positionInWorld.y())};
            painter.fillRect(QRect{position.x(), position.y(), shrunkSize, shrunkSize}, QBrush{color});

            const bool leftOccupied = std::find(D_CIT(blocks), block.neighbor(GridXy::DIRECTION::LEFT)) != blocks.end();
            const bool upOccupied   = std::find(D_CIT(blocks), block.neighbor(GridXy::DIRECTION::UP)) != blocks.end();

            if (upOccupied && leftOccupied) {
                painter.fillRect(QRect{position.x(), position.y() - twiceShrink - 1, shrunkSize, twiceShrink + 2}, color);
                painter.fillRect(QRect{position.x() - twiceShrink - 1, position.y(), twiceShrink + 2, shrunkSize}, color);

                const bool upAndLeftOccupied = std::find(D_CIT(blocks), block + GridXy{-1, -1}) != blocks.end();
                if (upAndLeftOccupied) {
                    painter.fillRect(QRect{position.x() - twiceShrink - 1, position.y() - twiceShrink - 1, twiceShrink + 2, twiceShrink + 2}, color);
                }
            } else {
                if (upOccupied) {
                    painter.fillRect(QRect{position.x(), position.y() - twiceShrink - 1, shrunkSize, twiceShrink + 2}, color);
                } else if (leftOccupied) {
                    painter.fillRect(QRect{position.x() - twiceShrink - 1, position.y(), twiceShrink + 2, shrunkSize}, color);
                }
            }
        }
        return result;
    }

    void MainViewPainter::drawCluster(const Cluster& cluster, QPainter& painter, const ViewPort& viewPort, bool paintNames) {
        const auto f = cluster.phaseTransformation();
        if (cluster.phaseTransformation()) {
            drawConnected(
                cluster.gridXyVector(), cluster.isAlive() ? color::CLUSTER : color::CLUSTER_DEAD_COLOR, painter, cluster.angle(), f, viewPort);
        }
        const auto namePosition =
            ScreenXy::fromWorldXy(f(WorldXy(*cluster.gridXyVector().begin()) + WorldXy{5, app::HALF_BLOCK_SIZE_IN_WORLD}), viewPort);
        if (paintNames) {
            painter.setFont(FontManager::font(FONT_ENUM::ANON_PRO_ITALIC, 12));
            QFontMetrics fontMetrics(painter.font());
            const int    width  = fontMetrics.horizontalAdvance(cluster.name().c_str());
            const int    height = fontMetrics.height();
            painter.fillRect(namePosition.x() - 4, namePosition.y() + 2 - height, width + 8, height + 4, color::NAME_BACKGROUND);
            painter.drawText(namePosition.x(), namePosition.y(), cluster.name().c_str());
        }
#ifdef DEBUG
        const auto alignedRectangle = geom::AlignedRectangle::boundingAlignedRectangle(cluster);

        for (size_t i = 0; i != 4; ++i) {
            const auto wl = alignedRectangle.sideAt(i);

            const auto start = ScreenXy::fromWorldXy(wl.start(), viewPort);
            const auto end   = ScreenXy::fromWorldXy(wl.end(), viewPort);
            painter.save();
            painter.setPen(QPen{Qt::red, 3});
            painter.drawLine(start.x(), start.y(), end.x(), end.y());
            painter.restore();
        }
        const auto& sides = cluster.sides(0);
        for (const auto& side : sides) {
            const auto start = ScreenXy::fromWorldXy(side.start(), viewPort);
            const auto end   = ScreenXy::fromWorldXy(side.end(), viewPort);
            painter.save();
            painter.setPen(QPen{Qt::yellow, 3});
            painter.drawLine(start.x(), start.y(), end.x(), end.y());
            painter.restore();
        }
#endif
    }

    void MainViewPainter::drawConnected(
        const GridXyVector& blocks, const QColor& color, QPainter& painter, double angle, const PhaseTransformation& f, const ViewPort& viewPort) {
        painter.save();

        const auto topLeft = ScreenXy::fromWorldXy(f(WorldXy(GridXy{geom::minX(blocks), geom::minY(blocks)})), viewPort);
        painter.translate(topLeft.x(), topLeft.y());
        painter.rotate(angle);
        painter.drawPixmap(0, 0, connectedPixmap(blocks, color, viewPort));

        painter.restore();
    }
} // namespace view