//
// Created by teunv on 1/12/2021.
//

#ifndef BLOCKS_TOCOLOR_H
#define BLOCKS_TOCOLOR_H

#include "../model/Level_enums.h"
#include "BlockType.h"
#include "color.h"

namespace view {

    constexpr const QColor& toColor(model::FLOOR_BLOCK_TYPE type) {
        switch (type) {
            case model::FLOOR_BLOCK_TYPE::LEVEL:
                return color::PLAYABLE;
            case model::FLOOR_BLOCK_TYPE::START:
                return color::START;
            case model::FLOOR_BLOCK_TYPE::SPLICE:
                return color::SPLICE;
            case model::FLOOR_BLOCK_TYPE::CONWAY:
                return color::CONWAY;
        }
        return color::PLAYABLE;
    }

    constexpr const QColor& toColor(view::CLUSTER_BLOCK type) {
        switch (type) {
            case CLUSTER_BLOCK::CLUSTER:
                return color::CLUSTER;
        }
        return color::PLAYABLE;
    }

} // namespace view

#endif // BLOCKS_TOCOLOR_H
