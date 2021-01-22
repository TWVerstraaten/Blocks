//
// Created by teunv on 1/21/2021.
//

#include "PixmapManager.h"

#include "toPixmap.h"

namespace view {

    const QPixmap& PixmapManager::pixmap(view::BlockType type, const QSize& size) {
        if (m_pixmaps.find({type, size.width(), size.height()}) == m_pixmaps.end()) {
            m_pixmaps[{type, size.width(), size.height()}] = toPixmapVariant(type, size);
        }
        return m_pixmaps.at({type, size.width(), size.height()});
    }
} // namespace view