//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_LEVEL_H
#define BLOCKS_LEVEL_H

#include "IndexPair.h"

#include <map>
#include <tuple>
#include <vector>

namespace model {

    class Level {

      public:
        Level();

        enum class BLOCK_TYPE { NONE, ROTATE_CW, ROTATE_CCW, KILL };

        BLOCK_TYPE                             blockAt(const IndexPair& indexPair);
        const std::map<IndexPair, BLOCK_TYPE>& blocks() const;

      private:
        std::map<IndexPair, BLOCK_TYPE> m_blocks;
    };
} // namespace model
#endif // BLOCKS_LEVEL_H
