//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_BLOCKTYPE_H
#define BLOCKS_BLOCKTYPE_H

#include "../model/Level_enums.h"

#include <variant>

namespace view {

    enum class CLUSTER_BLOCK { CLUSTER };

    typedef std::variant<model::FLOOR_BLOCK_TYPE, model::DYNAMIC_BLOCK_TYPE, model::INSTANT_BLOCK_TYPE, CLUSTER_BLOCK> BlockType;

} // namespace view

#endif // BLOCKS_BLOCKTYPE_H
