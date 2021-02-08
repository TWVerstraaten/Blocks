//
// Created by teunv on 2/1/2021.
//

#include "ModelToPixmap.h"

#include "../misc/geom.h"
#include "../model/Model.h"
#include "ViewPort.h"
#include "widget/MainViewPainter.h"

namespace view {

    QPixmap modelToPixmap(const std::string& path, const QSize& size) {
        model::Model model;
        model.init(path);
        const auto& blocks = model.level().blocks(model::FLOOR_BLOCK_TYPE::LEVEL);
        const auto  minX   = geom::minX(blocks);
        const auto  minY   = geom::minY(blocks);
        const int   width  = 1 + geom::maxX(blocks) - minX;
        const int   height = 1 + geom::maxY(blocks) - minY;

        assert(width > 0);
        assert(height > 0);

        const double scale =
            std::min(static_cast<double>(size.width()) / (width), static_cast<double>(size.height()) / (height)) / app::BLOCK_SIZE_IN_WORLD;

        view::ViewPort viewPort;
        viewPort.setScale(scale);
        viewPort.setOffset(-viewPort.worldToScreen(minX * app::BLOCK_SIZE_IN_WORLD), -viewPort.worldToScreen(minY * app::BLOCK_SIZE_IN_WORLD));

        QPixmap     pixmap(size);
        const QRect rect{0, 0, size.width(), size.height()};
        QPainter    painter(&pixmap);
        model.buildClusterSides();
        widget::MainViewPainter::paint(painter, rect, model, viewPort, false);
        return pixmap;
    }
} // namespace view