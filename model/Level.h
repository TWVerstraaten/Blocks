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
        Level() = default;

        enum class DYNAMIC_BLOCK_TYPE { NONE, ROTATE_CW, ROTATE_CCW };
        enum class INSTANT_BLOCK_TYPE { NONE, KILL };

        DYNAMIC_BLOCK_TYPE                             dynamicBlockAt(const IndexPair& indexPair);
        const std::map<IndexPair, DYNAMIC_BLOCK_TYPE>& dynamicBlocks() const;
        INSTANT_BLOCK_TYPE                             instantBlockAt(const IndexPair& indexPair);
        const std::map<IndexPair, INSTANT_BLOCK_TYPE>& instantBlocks() const;

        void addBlock(const IndexPair& indexPair, DYNAMIC_BLOCK_TYPE blockType);
        void addBlock(const IndexPair& indexPair, INSTANT_BLOCK_TYPE blockType);

      private:
        std::map<IndexPair, DYNAMIC_BLOCK_TYPE> m_dynamicBLocks;
        std::map<IndexPair, INSTANT_BLOCK_TYPE> m_instantBLocks;
    };
} // namespace model
#endif // BLOCKS_LEVEL_H
