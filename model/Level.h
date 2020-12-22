//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_LEVEL_H
#define BLOCKS_LEVEL_H

#include "GridXY.h"

#include <map>
#include <set>
#include <tuple>

namespace model {

    class Level {

      public:
        Level() = default;

        Level& operator=(const Level& other) = default;

        enum class DYNAMIC_BLOCK_TYPE { NONE, ROTATE_CW, ROTATE_CCW };
        enum class INSTANT_BLOCK_TYPE { NONE, KILL };

        DYNAMIC_BLOCK_TYPE                                   dynamicBlockAt(const GridXY& gridCoordinates);
        const std::map<GridXY, DYNAMIC_BLOCK_TYPE>& dynamicBlocks() const;
        INSTANT_BLOCK_TYPE                                   instantBlockAt(const GridXY& gridCoordinates);
        const std::map<GridXY, INSTANT_BLOCK_TYPE>& instantBlocks() const;

        const std::set<GridXY>& levelBlocks() const;

        void addBlock(const GridXY& gridCoordinates, DYNAMIC_BLOCK_TYPE blockType);
        void addBlock(const GridXY& gridCoordinates, INSTANT_BLOCK_TYPE blockType);
        void addLevelBlock(const GridXY& gridCoordinates);
        void setLevelBlocks(std::set<GridXY>&& levelBlocks);
        void clear();
        bool isInLevel(const model::WorldXY& worldCoordinates);

      private:
        std::map<GridXY, DYNAMIC_BLOCK_TYPE> m_dynamicBLocks;
        std::map<GridXY, INSTANT_BLOCK_TYPE> m_instantBLocks;
        std::set<GridXY>                     m_levelBlocks;
    };
} // namespace model
#endif // BLOCKS_LEVEL_H
