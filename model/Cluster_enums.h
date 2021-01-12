//
// Created by pc on 30-12-20.
//

#ifndef BLOCKS_CLUSTER_ENUMS_H
#define BLOCKS_CLUSTER_ENUMS_H

namespace model {
    enum class CLUSTER_STATE { ALIVE, STOPPED, DEAD };
    enum class PHASE { NONE, TRANSLATING, ROTATING };
    enum class PENDING_DYNAMIC_MOVES { ZERO, ONE, TOO_MANY };

} // namespace model

#endif // BLOCKS_CLUSTER_ENUMS_H
