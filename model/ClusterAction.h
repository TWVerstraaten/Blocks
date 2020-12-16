//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_CLUSTERACTION_H
#define BLOCKS_CLUSTERACTION_H

namespace model {

    struct ClusterAction {

        enum class ACTION { MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };
        enum class MODIFIER { NONE, IGNORE, SKIP };

        ACTION   m_action;
        MODIFIER m_modifier;
    };
} // namespace model

#endif // BLOCKS_CLUSTERACTION_H
