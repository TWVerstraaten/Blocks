//
// Created by teunv on 1/21/2021.
//

#ifndef BLOCKS_PIXMAPMANAGER_H
#define BLOCKS_PIXMAPMANAGER_H

#include "BlockType.h"

#include <QPixmap>
#include <QSize>
#include <map>
#include <tuple>

namespace view {

    class PixmapManager {

      public:
        static const QPixmap& pixmap(BlockType type, const QSize& size);

      private:
        static std::map<std::tuple<BlockType, size_t, size_t>, QPixmap> m_pixmaps;
    };

} // namespace view

#endif // BLOCKS_PIXMAPMANAGER_H
