//
// Created by pc on 28-12-20.
//

#ifndef BLOCKS_ACTION_ENUMS_H
#define BLOCKS_ACTION_ENUMS_H

namespace action {
    enum class ACTION_TYPE {
        ADD_BLOCK_TO_CLUSTER,
        ADD_CLUSTER,
        GENERIC_COMMAND_EDIT_BOX,
        GENERIC_MODEL,
        REMOVE_BLOCK_FROM_CLUSTER,
        REMOVE_CLUSTER,
        MERGE_CLUSTERS,
        ADD_LEVEL_BLOCK_ACTION,
        REMOVE_LEVEL_BLOCK_ACTION,
        GENERIC_TEXT_EDIT_ACTION,
        GLOB
    };
}

#endif // BLOCKS_ACTION_ENUMS_H
