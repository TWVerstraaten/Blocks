//
// Created by teunv on 1/19/2021.
//

#include "toPixmap.h"

namespace view {

    QPixmap toPixmap(model::FLOOR_BLOCK_TYPE type, const QSize& size) {
        QPixmap p(size);
        p.fill(toColor(type));
        p.scaled(size);
        return p;
    }

    QPixmap toPixmap(CLUSTER_BLOCK type, const QSize& size) {
        QPixmap p(size);
        p.fill(toColor(type));
        p.scaled(size);
        return p;
    }

    QPixmap toPixmap(model::INSTANT_BLOCK_TYPE type, const QSize& size) {
        QPixmap p(toFile(type));
        p.scaled(size);
        return p;
    }

    QPixmap toPixmap(model::DYNAMIC_BLOCK_TYPE type, const QSize& size) {
        QPixmap p(toFile(type));
        p.scaled(size);
        return p;
    }

    QPixmap toPixmapVariant(const BlockType& type, const QSize& size) {
        return std::visit([&](const auto& a) { return toPixmap(a, size); }, type);
    }
} // namespace view