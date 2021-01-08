//
// Created by pc on 29-12-20.
//

#ifndef BLOCKS_BLOCKTYPE_TYPEDEF_H
#define BLOCKS_BLOCKTYPE_TYPEDEF_H

#include "Level_enums.h"

#include <variant>

namespace model {
    enum class CLUSTER_TYPE { CLUSTER };
    typedef std::variant<DYNAMIC_BLOCK_TYPE, INSTANT_BLOCK_TYPE, FLOOR_BLOCK_TYPE, CLUSTER_TYPE> BlockType;
    static const std::vector<BlockType>                                                          s_allTypes{CLUSTER_TYPE::CLUSTER,
                                                   INSTANT_BLOCK_TYPE::KILL,
                                                   DYNAMIC_BLOCK_TYPE::ROTATE_CCW,
                                                   DYNAMIC_BLOCK_TYPE::ROTATE_CW,
                                                   FLOOR_BLOCK_TYPE::LEVEL,
                                                   FLOOR_BLOCK_TYPE::START,
                                                   FLOOR_BLOCK_TYPE::SPLICE};
} // namespace model
#endif // BLOCKS_BLOCKTYPE_TYPEDEF_H
