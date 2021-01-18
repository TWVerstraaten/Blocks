//
// Created by pc on 28-12-20.
//

#ifndef BLOCKS_COLOR_H
#define BLOCKS_COLOR_H

#include <QColor>

namespace view::color {
    static const QColor DARK_GREY_COLOR                    = {120, 120, 124, 255};
    static const QColor WHITE_COLOR                        = {255, 255, 255, 255};
    static const QColor RED_COLOR                          = {255, 0, 0, 255};
    static const QColor BLACK_COLOR                        = {0, 0, 0, 255};
    static const QColor TEXT_ERROR_COLOR                   = {200, 50, 50, 255};
    static const QColor BACKGROUND_COLOR                   = {8, 33, 73, 255};
    static const QColor BACKGROUND_PLAYABLE_COLOR          = {35, 123, 193, 255};
    static const QColor BACKGROUND_START_COLOR             = {119, 181, 230, 255};
    static const QColor BACKGROUND_SPLICE_COLOR            = {230, 181, 119, 255};
    static const QColor BACKGROUND_CONWAY_COLOR            = {21, 231, 32, 255};
    static const QColor EDIT_BOX_BACKGROUND_INACTIVE_COLOR = {120, 120, 120, 255};
    static const QColor EDIT_BOX_HIGHLIGHT_DEAD_COLOR      = {150, 150, 150, 255};
    static const QColor COMMAND_EDIT_BACKGROUND            = {210, 210, 255, 255};
    static const QColor EDIT_BOX_HIGHLIGHT_HARD_COLOR      = {255, 200, 250, 255};
    static const QColor EDIT_BOX_HIGHLIGHT_SOFT_COLOR      = {220, 200, 250, 255};
    static const QColor SCROLL_AREA_BACKGROUND_COLOR       = {140, 140, 180, 255};
    static const QColor CLUSTER_COLOR                      = {250, 235, 65, 255};
    static const QColor CLUSTER_DEAD_COLOR                 = {255, 0, 0, 255};

} // namespace view::color

#endif // BLOCKS_COLOR_H
