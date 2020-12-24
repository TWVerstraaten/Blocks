//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_LEVEL_H
#define BLOCKS_LEVEL_H

#include "GridXY.h"

#include <map>
#include <set>

namespace model {

    class Level {

      public:
        enum class DYNAMIC_BLOCK_TYPE { NONE, ROTATE_CW, ROTATE_CCW };
        enum class INSTANT_BLOCK_TYPE { NONE, KILL };

        Level()                   = default;
        Level(const Level& other) = default;
        Level(Level&& other)      = default;
        Level& operator=(const Level& other) = default;

        void                                        addBlock(const GridXY& gridXY, DYNAMIC_BLOCK_TYPE blockType);
        void                                        addBlock(const GridXY& gridXY, INSTANT_BLOCK_TYPE blockType);
        void                                        addLevelBlock(const GridXY& gridXY);
        void                                        addStartBlock(const GridXY& gridXY);
        void                                        clear();
        bool                                        isInLevel(const model::WorldXY& worldXY);
        DYNAMIC_BLOCK_TYPE                          dynamicBlockAt(const GridXY& gridXY);
        INSTANT_BLOCK_TYPE                          instantBlockAt(const GridXY& gridXY);
        const std::map<GridXY, DYNAMIC_BLOCK_TYPE>& dynamicBlocks() const;
        const std::map<GridXY, INSTANT_BLOCK_TYPE>& instantBlocks() const;
        const std::set<GridXY>&                     levelBlocks() const;
        const std::set<GridXY>&                     startBlocks() const;
        bool                                        isFreeStartBlock(const GridXY gridXY) const;

      private:
        std::map<GridXY, DYNAMIC_BLOCK_TYPE> m_dynamicBLocks;
        std::map<GridXY, INSTANT_BLOCK_TYPE> m_instantBLocks;
        std::set<GridXY>                     m_levelBlocks;
        std::set<GridXY>                     m_startBlocks;
    };
} // namespace model
#endif // BLOCKS_LEVEL_H
