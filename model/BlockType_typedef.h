//
// Created by pc on 29-12-20.
//

#ifndef BLOCKS_BLOCKTYPE_TYPEDEF_H
#define BLOCKS_BLOCKTYPE_TYPEDEF_H

#include "Level_enums.h"

#include <variant>

namespace model {
    enum class CLUSTER { CLUSTER };
    typedef std::variant<DYNAMIC_BLOCK_TYPE, INSTANT_BLOCK_TYPE, FLOOR_BLOCK> BlockType;
    static const std::vector<BlockType>                                       s_allTypes{INSTANT_BLOCK_TYPE::KILL,
                                                   DYNAMIC_BLOCK_TYPE::ROTATE_CCW,
                                                   DYNAMIC_BLOCK_TYPE::ROTATE_CW,
                                                   FLOOR_BLOCK::CLUSTER,
                                                   FLOOR_BLOCK::LEVEL,
                                                   FLOOR_BLOCK::START};
} // namespace model
#endif // BLOCKS_BLOCKTYPE_TYPEDEF_H
