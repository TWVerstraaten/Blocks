//
// Created by teunv on 1/12/2021.
//

#ifndef BLOCKS_ENUM_H
#define BLOCKS_ENUM_H

#include "../model/Level_enums.h"
#include "BlockType.h"
#include "color.h"

namespace view {

    constexpr const QColor& toColor(model::FLOOR_BLOCK_TYPE type) {
        switch (type) {
            case model::FLOOR_BLOCK_TYPE::LEVEL:
                return color::BACKGROUND_PLAYABLE_COLOR;
            case model::FLOOR_BLOCK_TYPE::START:
                return color::BACKGROUND_START_COLOR;
            case model::FLOOR_BLOCK_TYPE::SPLICE:
                return color::BACKGROUND_SPLICE_COLOR;
            case model::FLOOR_BLOCK_TYPE::CONWAY:
                return color::BACKGROUND_CONWAY_COLOR;
        }
        return color::BACKGROUND_PLAYABLE_COLOR;
    }

    constexpr const QColor& toColor(view::CLUSTER_BLOCK type) {
        switch (type) {
            case CLUSTER_BLOCK::CLUSTER:
                return color::CLUSTER_COLOR;
        }
        return color::BACKGROUND_PLAYABLE_COLOR;
    }

} // namespace view

#endif // BLOCKS_ENUM_H
