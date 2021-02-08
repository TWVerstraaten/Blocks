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

    class PixmapManager : public QObject {
        Q_OBJECT

      public:
        static const QPixmap& pixmap(BlockType type, const QSize& size);

      public slots:
        static void clear();

      private:
        inline static std::map<std::tuple<BlockType, size_t, size_t>, QPixmap> s_pixmaps;
    };

} // namespace view

#endif // BLOCKS_PIXMAPMANAGER_H
