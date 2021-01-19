//
// Created by teunv on 1/19/2021.
//

#include "toPixmap.h"

QPixmap view::toPixmap(model::FLOOR_BLOCK_TYPE type, const QSize& size) {
    QPixmap p(size);
    p.fill(toColor(type));
    p.scaled(size);
    return p;
}

QPixmap view::toPixmap(view::CLUSTER_BLOCK type, const QSize& size) {
    QPixmap p(size);
    p.fill(toColor(type));
    p.scaled(size);
    return p;
}

QPixmap view::toPixmap(model::INSTANT_BLOCK_TYPE type, const QSize& size) {
    QPixmap p(toFile(type));
    p.scaled(size);
    return p;
}

QPixmap view::toPixmap(model::DYNAMIC_BLOCK_TYPE type, const QSize& size) {
    QPixmap p(toFile(type));
    p.scaled(size);
    return p;
}

QPixmap view::toPixmapVariant(const view::BlockType& type, const QSize& size) {
    return std::visit([&](const auto& a) { return toPixmap(a, size); }, type);
}
