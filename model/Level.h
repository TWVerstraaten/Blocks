//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_LEVEL_H
#define BLOCKS_LEVEL_H

#include "IndexPair.h"

#include <tuple>
#include <vector>

namespace model {

    class Level {

      public:
        Level();

        enum class BLOCK_TYPE { NONE, ROTATE_CW, ROTATE_CCW, KILL };
        typedef std::pair<IndexPair, BLOCK_TYPE> Block;

        BLOCK_TYPE   blockAt(int row, int column);
        BLOCK_TYPE   blockAt(const IndexPair& indexPair);
        size_t       blockCount() const;
        const Block& blockAt(size_t index) const;

      private:
        std::vector<Block> m_blocks;
    };
} // namespace model
#endif // BLOCKS_LEVEL_H
