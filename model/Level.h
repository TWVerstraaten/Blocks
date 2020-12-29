//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_LEVEL_H
#define BLOCKS_LEVEL_H

#include "../action/Action.h"
#include "GridXY.h"
#include "Level_enums.h"
#include "Line.h"

#include <map>
#include <memory>
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
        [[nodiscard]] const std::set<GridXY>&                     levelBlocks() const;
        [[nodiscard]] const std::set<GridXY>&                     startBlocks() const;
        [[nodiscard]] const std::set<Line<WorldXY>>&              boundaries() const;
        [[nodiscard]] const std::map<GridXY, DYNAMIC_BLOCK_TYPE>& dynamicBlocks() const;
        [[nodiscard]] const std::map<GridXY, INSTANT_BLOCK_TYPE>& instantBlocks() const;

        /****** CONST FUNCTIONS  ******/
        [[nodiscard]] bool               isInLevel(const model::WorldXY& worldXY) const;
        [[nodiscard]] bool               isFreeStartBlock(const GridXY& gridXY) const;
        [[nodiscard]] DYNAMIC_BLOCK_TYPE dynamicBlockAt(const GridXY& gridXY) const;
        [[nodiscard]] INSTANT_BLOCK_TYPE instantBlockAt(const GridXY& gridXY) const;

        void                            addLevelBlock(const GridXY& gridXY);
        void                            addStartBlock(const GridXY& gridXY);
        void                            clear();
        void                            createBoundaries();
        void                            removeBlock(const GridXY& gridXY, DYNAMIC_BLOCK_TYPE blockType);
        void                            removeBlock(const GridXY& gridXY, INSTANT_BLOCK_TYPE blockType);
        void                            removeBlock(const GridXY& gridXY, FLOOR_BLOCK_TYPE blockType);
        std::unique_ptr<action::Action> addBlock(const GridXY& gridXY, DYNAMIC_BLOCK_TYPE blockType);
        std::unique_ptr<action::Action> addBlock(const GridXY& gridXY, INSTANT_BLOCK_TYPE blockType);
        std::unique_ptr<action::Action> addBlock(const GridXY& gridXY, FLOOR_BLOCK_TYPE blockType);
        std::unique_ptr<action::Action> removeBlock(const GridXY& gridXY);

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
