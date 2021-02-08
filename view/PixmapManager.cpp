//
// Created by teunv on 1/21/2021.
//

#include "PixmapManager.h"

#include "toPixmap.h"

#include <QDebug>

namespace view {

    const QPixmap& PixmapManager::pixmap(view::BlockType type, const QSize& size) {
        if (s_pixmaps.find({type, size.width(), size.height()}) == s_pixmaps.end()) {
            s_pixmaps[{type, size.width(), size.height()}] = toPixmapVariant(type, size);
        }
        return s_pixmaps.at({type, size.width(), size.height()});
    }

    void PixmapManager::clear() {
        s_pixmaps.clear();
    }
} // namespace view