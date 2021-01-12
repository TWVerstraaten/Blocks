//
// Created by pc on 28-12-20.
//

#ifndef BLOCKS_LEVEL_ENUMS_H
#define BLOCKS_LEVEL_ENUMS_H

#include <vector>

namespace model {
    enum class DYNAMIC_BLOCK_TYPE { ROTATE_CW, ROTATE_CCW };
    enum class INSTANT_BLOCK_TYPE { KILL };
    enum class FLOOR_BLOCK_TYPE { LEVEL, START, SPLICE, CONWAY };

} // namespace model

#endif // BLOCKS_LEVEL_ENUMS_H
