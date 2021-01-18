//
// Created by teunv on 1/18/2021.
//

#include "MainViewPainter.h"

#include "MainView.h"
#include "global/geom.h"
#include "toColor.h"

#include <QFontDatabase>

view::MainViewPainter::MainViewPainter(const view::MainView* mainView) : m_mainView(mainView) {
    const int     id     = QFontDatabase::addApplicationFont("assets/UbuntuMono-Italic.ttf");
    const QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    m_font               = QFont(family, 12);
}

void view::MainViewPainter::paint(QPainter& painter, QPaintEvent* event) {
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), view::color::BACKGROUND_COLOR);

    const auto blockShrinkInScreen = m_mainView->m_viewPort.worldToScreen(app::BLOCK_SHRINK_IN_WORLD);
    const auto shrinkInScreenXY    = view::ScreenXY{blockShrinkInScreen, blockShrinkInScreen};
    const auto shrunkBlockSize     = m_mainView->m_viewPort.blockSizeInScreen() - 2 * blockShrinkInScreen;

    for (const auto& [point, type] : m_mainView->m_model->level().floorBlocks()) {
        const auto position = view::ScreenXY::fromGridXY(point, m_mainView->m_viewPort) + shrinkInScreenXY;

        painter.setBrush({view::toColor(type)});
        painter.drawRect(QRect{position.x(), position.y(), shrunkBlockSize, shrunkBlockSize});
    }

    for (const auto& stoppedCluster : m_mainView->m_model->level().stoppedClusters()) {
        drawConnected(stoppedCluster.gridXY(), view::color::DARK_GREY_COLOR, painter);
    }
    for (const auto& cluster : m_mainView->m_model->clusters()) {
        drawCluster(cluster, painter);
    }
    for (const auto& [point, type] : m_mainView->m_model->level().dynamicBlocks()) {
        const auto position = view::ScreenXY::fromGridXY(point, m_mainView->m_viewPort) + shrinkInScreenXY;
        switch (type) {
            case model::DYNAMIC_BLOCK_TYPE::ROTATE_CW:
                painter.drawImage(QRect{position.x(), position.y(), shrunkBlockSize, shrunkBlockSize}, QImage("assets/arrow_cw.png"));
                break;
            case model::DYNAMIC_BLOCK_TYPE::ROTATE_CCW:
                painter.drawImage(QRect{position.x(), position.y(), shrunkBlockSize, shrunkBlockSize}, QImage("assets/arrow_ccw.png"));
                break;
        }
    }
    for (const auto& [point, type] : m_mainView->m_model->level().instantBlocks()) {
        const auto position = view::ScreenXY::fromGridXY(point, m_mainView->m_viewPort) + shrinkInScreenXY;
        switch (type) {
            case model::INSTANT_BLOCK_TYPE::KILL:
                painter.drawImage(QRect{position.x(), position.y(), shrunkBlockSize, shrunkBlockSize}, QImage("assets/kill.png"));
                break;
        }
    }
}

std::unique_ptr<QPixmap> view::MainViewPainter::connectedPixmap(const model::GridXYSet& blocks, const QColor& color) const {
    using namespace model;

    const auto shrunkSize  = m_mainView->m_viewPort.blockSizeInScreen() - 2 * m_mainView->m_viewPort.worldToScreen(app::BLOCK_SHRINK_IN_WORLD);
    const auto twiceShrink = 2 * m_mainView->m_viewPort.worldToScreen(app::BLOCK_SHRINK_IN_WORLD);
    const auto minX        = geom::minX(blocks);
    const auto minY        = geom::minY(blocks);
    const auto maxX        = geom::maxX(blocks);
    const auto maxY        = geom::maxY(blocks);
    const auto width       = (maxX - minX + 1) * m_mainView->m_viewPort.blockSizeInScreen();
    const auto height      = (maxY - minY + 1) * m_mainView->m_viewPort.blockSizeInScreen();

    assert(width > 0);
    assert(height > 0);

    auto* result = new QPixmap{width + 20, height + 20};
    result->fill(Qt::transparent);
    QPainter painter(result);
    painter.setBrush(QBrush{color});
    for (const auto block : blocks) {
        const auto positionInWorld = block - GridXY{minX, minY} + WorldXY{app::BLOCK_SHRINK_IN_WORLD, app::BLOCK_SHRINK_IN_WORLD};
        const auto position =
            view::ScreenXY{m_mainView->m_viewPort.worldToScreen(positionInWorld.x()), m_mainView->m_viewPort.worldToScreen(positionInWorld.y())};
        painter.fillRect(QRect{position.x(), position.y(), shrunkSize, shrunkSize}, QBrush{color});

        const bool leftOccupied = blocks.find(block.neighbor(GridXY::DIRECTION::LEFT)) != blocks.end();
        const bool upOccupied   = blocks.find(block.neighbor(GridXY::DIRECTION::UP)) != blocks.end();
        if (upOccupied && leftOccupied) {
            painter.fillRect(QRect{position.x(), position.y() - twiceShrink - 1, shrunkSize, twiceShrink + 2}, color);
            painter.fillRect(QRect{position.x() - twiceShrink - 1, position.y(), twiceShrink + 2, shrunkSize}, color);

            const bool upAndLeftOccupied = blocks.find(block.neighbor(GridXY::DIRECTION::UP).neighbor(GridXY::DIRECTION::LEFT)) != blocks.end();
            if (upAndLeftOccupied) {
                painter.fillRect(QRect{position.x() - twiceShrink - 1, position.y() - twiceShrink - 1, twiceShrink + 2, twiceShrink + 2}, color);
            }
        } else {
            if (upOccupied) {
                painter.fillRect(QRect{position.x(), position.y() - twiceShrink - 1, shrunkSize, twiceShrink + 2}, color);
            }
            if (leftOccupied) {
                painter.fillRect(QRect{position.x() - twiceShrink - 1, position.y(), twiceShrink + 2, shrunkSize}, color);
            }
        }
    }
    return std::unique_ptr<QPixmap>(result);
}

void view::MainViewPainter::drawConnected(const model::GridXYSet& blocks, const QColor& color, QPainter& painter) const {
    const auto pixmap  = connectedPixmap(blocks, color);
    const auto topLeft = view::ScreenXY::fromWorldXY(model::WorldXY(model::GridXY{geom::minX(blocks), geom::minY(blocks)}), m_mainView->m_viewPort);

    painter.drawPixmap(topLeft.x(), topLeft.y(), *pixmap);
}

void view::MainViewPainter::drawCluster(const model::Cluster& cluster, QPainter& painter) {
    drawConnected(cluster.gridXY(), view::color::CLUSTER_COLOR, painter);
    const auto namePosition = view::ScreenXY::fromWorldXY(
        model::WorldXY(*cluster.gridXY().begin()) + model::WorldXY{5, app::HALF_BLOCK_SIZE_IN_WORLD}, m_mainView->m_viewPort);

    painter.setFont(m_font);
    QFontMetrics fontMetrics(m_font);
    const int    width  = fontMetrics.horizontalAdvance(cluster.name().c_str());
    const int    height = fontMetrics.height();
    painter.fillRect(namePosition.x() - 4, namePosition.y() + 2 - height, width + 8, height + 4, view::color::WHITE_COLOR);
    painter.drawText(namePosition.x(), namePosition.y(), cluster.name().c_str());
}
