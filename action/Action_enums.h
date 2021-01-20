//
// Created by pc on 28-12-20.
//

#ifndef BLOCKS_ACTION_ENUMS_H
#define BLOCKS_ACTION_ENUMS_H

namespace action {
    enum class ACTION_TYPE {
        ADD_CLUSTER,
        REMOVE_BLOCK_FROM_CLUSTER,
        REMOVE_CLUSTER,
        MERGE_CLUSTERS,
        CHANGE_FLOOR_BLOCK,
        ADD_FLOOR_BLOCK,
        REMOVE_FLOOR_BLOCK,
        TEXT_EDIT_ACTION,
        ADD_LEVEL_BLOCK,
        REMOVE_LEVEL_BLOCK,
        CHANGE_LEVEL_BLOCK
    };
}

#endif // BLOCKS_ACTION_ENUMS_H
