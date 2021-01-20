//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_TOPIXMAP_H
#define BLOCKS_TOPIXMAP_H

#include "BlockType.h"
#include "toColor.h"
#include "toFile.h"

#include <QPixmap>

namespace view {

    QPixmap toPixmap(model::FLOOR_BLOCK_TYPE type, const QSize& size);

    QPixmap toPixmap(view::CLUSTER_BLOCK type, const QSize& size);

    QPixmap toPixmap(model::INSTANT_BLOCK_TYPE type, const QSize& size);

    QPixmap toPixmap(model::DYNAMIC_BLOCK_TYPE type, const QSize& size);

    QPixmap toPixmapVariant(const BlockType& type, const QSize& size);

} // namespace view

#endif // BLOCKS_TOPIXMAP_H
