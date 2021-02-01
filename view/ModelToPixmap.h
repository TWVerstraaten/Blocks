//
// Created by teunv on 2/1/2021.
//

#ifndef BLOCKS_MODELTOPIXMAP_H
#define BLOCKS_MODELTOPIXMAP_H

#include <QPixmap>

namespace view {

    QPixmap modelToPixmap(const std::string& path, const QSize&size);

} // namespace view

#endif // BLOCKS_MODELTOPIXMAP_H
