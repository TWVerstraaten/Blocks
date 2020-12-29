//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_LEVEL_H
#define BLOCKS_LEVEL_H

#include "GridXY.h"
#include "Level_enums.h"
#include "Line.h"

#include <map>
#include <set>
#include <variant>

namespace model {

    class Level {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        Level()                   = default;
        Level(const Level& other) = default;
        Level(Level&& other)      = default;
        Level& operator=(const Level& other) = default;

        /****** CONST GETTERS  ******/
        const std::set<GridXY>&                     levelBlocks() const;
        const std::set<GridXY>&                     startBlocks() const;
        const std::set<Line<WorldXY>>&              boundaries() const;
        const std::map<GridXY, DYNAMIC_BLOCK_TYPE>& dynamicBlocks() const;
        const std::map<GridXY, INSTANT_BLOCK_TYPE>& instantBlocks() const;

        /****** CONST FUNCTIONS  ******/
        bool               isInLevel(const model::WorldXY& worldXY) const;
        bool               isFreeStartBlock(const GridXY& gridXY) const;
        DYNAMIC_BLOCK_TYPE dynamicBlockAt(const GridXY& gridXY) const;
        INSTANT_BLOCK_TYPE instantBlockAt(const GridXY& gridXY) const;

        /****** NON CONST FUNCTIONS  ******/
        void addBlock(const GridXY& gridXY, const std::variant<DYNAMIC_BLOCK_TYPE, INSTANT_BLOCK_TYPE>& type);
        void addLevelBlock(const GridXY& gridXY);
        void addStartBlock(const GridXY& gridXY);
        void clear();
        void createBoundaries();
        void addBlock(const GridXY& gridXY, DYNAMIC_BLOCK_TYPE blockType);
        void addBlock(const GridXY& gridXY, INSTANT_BLOCK_TYPE blockType);

      private:
        /****** PRIVATE CONST FUNCTIONS  ******/
        bool contains(const GridXY& gridXY) const;

        /****** DATA MEMBERS  ******/
        std::map<GridXY, DYNAMIC_BLOCK_TYPE> m_dynamicBLocks;
        std::map<GridXY, INSTANT_BLOCK_TYPE> m_instantBLocks;
        std::set<GridXY>                     m_levelBlocks;
        std::set<GridXY>                     m_startBlocks;
        std::set<Line<WorldXY>>              m_boundaries;
    };
} // namespace model
#endif // BLOCKS_LEVEL_H
