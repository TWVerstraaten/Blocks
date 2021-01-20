//
// Created by teunv on 1/18/2021.
//

#include "MainViewPainter.h"

#include "../model/Cluster.h"
#include "MainView.h"
#include "global/geom.h"
#include "toColor.h"
#include "toPixmap.h"

#include <QDebug>
#include <QFontDatabase>

namespace view {

    using namespace model;

    MainViewPainter::MainViewPainter(const MainView* mainView) : m_mainView(mainView), m_viewPort(&m_mainView->m_viewPort) {
        const int id = QFontDatabase::addApplicationFont(":/assets/UbuntuMono-Italic.ttf");
        assert(id >= 0);
        const QString family = QFontDatabase::applicationFontFamilies(id).at(0);
        m_font               = QFont(family, 12);
    }

    void MainViewPainter::paint(QPainter& painter, QPaintEvent* event) {
        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(event->rect(), color::BACKGROUND);

        const auto blockShrinkInScreen = m_viewPort->worldToScreen(app::BLOCK_SHRINK_IN_WORLD);
        const auto shrinkInScreenXy    = ScreenXy{blockShrinkInScreen, blockShrinkInScreen};
        const auto shrunkBlockSize     = m_viewPort->blockSizeInScreen() - 2 * blockShrinkInScreen;

        for (const auto& [point, type] : m_mainView->m_model->level().floorBlocks()) {
            const auto position = ScreenXy::fromGridXy(point, *m_viewPort) + shrinkInScreenXy;
            painter.setBrush({toColor(type)});
            painter.drawRect(QRect{position.x(), position.y(), shrunkBlockSize, shrunkBlockSize});
        }
        for (const auto& stoppedCluster : m_mainView->m_model->level().stoppedClusters()) {
            drawConnected(stoppedCluster.gridXy(), color::CLUSTER_STOPPED, painter);
        }
        for (const auto& cluster : m_mainView->m_model->clusters()) {
            drawCluster(cluster, painter);
        }
        for (const auto& [point, type] : m_mainView->m_model->level().dynamicBlocks()) {
            const auto position = ScreenXy::fromGridXy(point, *m_viewPort) + shrinkInScreenXy;
            painter.drawPixmap(QRect{position.x(), position.y(), shrunkBlockSize, shrunkBlockSize},
                               toPixmap(type, QSize{shrunkBlockSize, shrunkBlockSize}));
        }
        for (const auto& [point, type] : m_mainView->m_model->level().instantBlocks()) {
            const auto position = ScreenXy::fromGridXy(point, *m_viewPort) + shrinkInScreenXy;
            painter.drawPixmap(QRect{position.x(), position.y(), shrunkBlockSize, shrunkBlockSize},
                               toPixmap(type, QSize{shrunkBlockSize, shrunkBlockSize}));
        }
    }

    QPixmap MainViewPainter::connectedPixmap(const GridXySet& blocks, const QColor& color) const {
        using namespace model;

        const auto shrunkSize  = m_viewPort->blockSizeInScreen() - 2 * m_viewPort->worldToScreen(app::BLOCK_SHRINK_IN_WORLD);
        const auto twiceShrink = 2 * m_viewPort->worldToScreen(app::BLOCK_SHRINK_IN_WORLD);
        const auto minX        = geom::minX(blocks);
        const auto minY        = geom::minY(blocks);
        const auto maxX        = geom::maxX(blocks);
        const auto maxY        = geom::maxY(blocks);
        const auto width       = (maxX - minX + 1) * m_viewPort->blockSizeInScreen();
        const auto height      = (maxY - minY + 1) * m_viewPort->blockSizeInScreen();

        assert(width > 0);
        assert(height > 0);

        QPixmap result{width + 20, height + 20};
        result.fill(Qt::transparent);
        QPainter painter(&result);
        painter.setBrush(color);
        for (const auto block : blocks) {
            const auto positionInWorld = block - GridXy{minX, minY} + WorldXy{app::BLOCK_SHRINK_IN_WORLD, app::BLOCK_SHRINK_IN_WORLD};
            const auto position        = ScreenXy{m_viewPort->worldToScreen(positionInWorld.x()), m_viewPort->worldToScreen(positionInWorld.y())};
            painter.fillRect(QRect{position.x(), position.y(), shrunkSize, shrunkSize}, QBrush{color});

            const bool leftOccupied = blocks.find(block.neighbor(GridXy::DIRECTION::LEFT)) != blocks.end();
            const bool upOccupied   = blocks.find(block.neighbor(GridXy::DIRECTION::UP)) != blocks.end();
            if (upOccupied && leftOccupied) {
                painter.fillRect(QRect{position.x(), position.y() - twiceShrink - 1, shrunkSize, twiceShrink + 2}, color);
                painter.fillRect(QRect{position.x() - twiceShrink - 1, position.y(), twiceShrink + 2, shrunkSize}, color);

                const bool upAndLeftOccupied = blocks.find(block.neighbor(GridXy::DIRECTION::UP).neighbor(GridXy::DIRECTION::LEFT)) != blocks.end();
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

    void MainViewPainter::drawConnected(const GridXySet& blocks, const QColor& color, QPainter& painter) const {
        const auto topLeft = ScreenXy::fromWorldXy(WorldXy(GridXy{geom::minX(blocks), geom::minY(blocks)}), *m_viewPort);
        const auto pixmap  = connectedPixmap(blocks, color);
        painter.drawPixmap(topLeft.x(), topLeft.y(), pixmap);
    }

    void MainViewPainter::drawCluster(const Cluster& cluster, QPainter& painter) {
        const auto f = cluster.phaseTransformation();
        if (cluster.phaseTransformation()) {
            drawConnected(cluster.gridXy(), cluster.isAlive() ? color::CLUSTER : color::CLUSTER_DEAD_COLOR, painter, -cluster.angle(), f);
        }
        const auto namePosition =
            ScreenXy::fromWorldXy(f(WorldXy(*cluster.gridXy().begin()) + WorldXy{5, app::HALF_BLOCK_SIZE_IN_WORLD}), *m_viewPort);

#ifdef DEBUG
        const auto sides = cluster.sides(0);
        for (const auto& side : sides) {
            const auto start = ScreenXy::fromWorldXy(side.start(), m_mainView->viewPort());
            const auto end   = ScreenXy::fromWorldXy(side.end(), m_mainView->viewPort());
            painter.save();
            painter.setPen(QPen{Qt::yellow, 3});
            painter.drawLine(start.x(), start.y(), end.x(), end.y());
            painter.restore();
        }
#endif

//        painter.setFont(m_font);
//        QFontMetrics fontMetrics(m_font);
//        const int    width  = fontMetrics.horizontalAdvance(cluster.name().c_str());
//        const int    height = fontMetrics.height();
//        painter.fillRect(namePosition.x() - 4, namePosition.y() + 2 - height, width + 8, height + 4, color::NAME_BACKGROUND);
//        painter.drawText(namePosition.x(), namePosition.y(), cluster.name().c_str());
    }

    void MainViewPainter::drawConnected(
        const GridXySet& blocks, const QColor& color, QPainter& painter, double angle, const PhaseTransformation& f) const {
        painter.save();

        const auto topLeft = ScreenXy::fromWorldXy(f(WorldXy(GridXy{geom::minX(blocks), geom::minY(blocks)})), *m_viewPort);
        painter.translate(topLeft.x(), topLeft.y());
        painter.rotate(angle);
        const auto pixmap = connectedPixmap(blocks, color);
        painter.drawPixmap(0, 0, pixmap);

        painter.restore();
    }
} // namespace view