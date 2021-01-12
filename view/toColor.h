//
// Created by teunv on 1/12/2021.
//

#ifndef BLOCKS_ENUM_H
#define BLOCKS_ENUM_H

#include "../model/Level_enums.h"

namespace view {

    constexpr const SDL_Color& toColor(model::FLOOR_BLOCK_TYPE type) {
        switch (type) {
            case model::FLOOR_BLOCK_TYPE::LEVEL:
                return color::BACKGROUND_PLAYABLE;
            case model::FLOOR_BLOCK_TYPE::START:
                return color::BACKGROUND_START;
            case model::FLOOR_BLOCK_TYPE::SPLICE:
                return color::BACKGROUND_SPLICE;
            case model::FLOOR_BLOCK_TYPE::CONWAY:
                return color::BACKGROUND_CONWAY;
        }
        return color::WHITE;
    }
} // namespace view

#endif // BLOCKS_ENUM_H
