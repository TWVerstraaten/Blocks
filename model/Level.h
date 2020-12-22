//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_LEVEL_H
#define BLOCKS_LEVEL_H

#include "GridCoordinates.h"

#include <map>
#include <set>
#include <tuple>
#include <vector>

namespace model {

    class Level {

      public:
        Level() = default;

        enum class DYNAMIC_BLOCK_TYPE { NONE, ROTATE_CW, ROTATE_CCW };
        enum class INSTANT_BLOCK_TYPE { NONE, KILL };

        DYNAMIC_BLOCK_TYPE                                   dynamicBlockAt(const GridCoordinates& indexPair);
        const std::map<GridCoordinates, DYNAMIC_BLOCK_TYPE>& dynamicBlocks() const;
        INSTANT_BLOCK_TYPE                                   instantBlockAt(const GridCoordinates& indexPair);
        const std::map<GridCoordinates, INSTANT_BLOCK_TYPE>& instantBlocks() const;

        const std::set<GridCoordinates>& levelBlocks() const;

        void addBlock(const GridCoordinates& gridCoordinates, DYNAMIC_BLOCK_TYPE blockType);
        void addBlock(const GridCoordinates& gridCoordinates, INSTANT_BLOCK_TYPE blockType);
        void addLevelBlock(const GridCoordinates& gridCoordinates);
        void setLevelBlocks(std::set<GridCoordinates>&& levelBlocks);
        void clear();
        bool isInLevel(const model::WorldCoordinates& worldCoordinates);

      private:
        std::map<GridCoordinates, DYNAMIC_BLOCK_TYPE> m_dynamicBLocks;
        std::map<GridCoordinates, INSTANT_BLOCK_TYPE> m_instantBLocks;
        std::set<GridCoordinates>                     m_levelBlocks;
    };
} // namespace model
#endif // BLOCKS_LEVEL_H
