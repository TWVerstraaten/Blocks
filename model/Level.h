//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_LEVEL_H
#define BLOCKS_LEVEL_H

#include "../action/Action.h"
#include "Cluster.h"
#include "GridXY.h"
#include "Level_enums.h"
#include "Line.h"

#include <list>
#include <map>
#include <memory>
#include <set>

namespace model {

    class Level {

        /****** PRIVATE ENUMS / TYPEDEFS  ******/
        typedef std::unique_ptr<action::Action> Action_u_ptr;

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        Level()                   = default;
        Level(const Level& other) = default;
        Level(Level&& other)      = default;
        Level& operator=(const Level& other) = default;

        /****** CONST GETTERS  ******/
        [[nodiscard]] bool                                        isFreeStartBlock(const GridXY& gridXY) const;
        [[nodiscard]] DYNAMIC_BLOCK_TYPE                          dynamicBlockAt(const GridXY& gridXY) const;
        [[nodiscard]] const std::set<GridXY>&                     levelBlocks() const;
        [[nodiscard]] const std::set<GridXY>&                     startBlocks() const;
        [[nodiscard]] const std::set<Line<WorldXY>>&              boundaries() const;
        [[nodiscard]] const std::map<GridXY, DYNAMIC_BLOCK_TYPE>& dynamicBlocks() const;
        [[nodiscard]] const std::map<GridXY, INSTANT_BLOCK_TYPE>& instantBlocks() const;
        [[nodiscard]] const std::list<Cluster>&                   stoppedClusters() const;

        /****** NON CONST FUNCTIONS  ******/
        void                clear();
        void                createBoundaries();
        Action_u_ptr        addStartBlock(const GridXY& gridXY);
        Action_u_ptr        addLevelBlock(const GridXY& gridXY);
        Action_u_ptr        addBlock(const GridXY& gridXY, DYNAMIC_BLOCK_TYPE blockType);
        Action_u_ptr        addBlock(const GridXY& gridXY, INSTANT_BLOCK_TYPE blockType);
        Action_u_ptr        addBlock(const GridXY& gridXY, FLOOR_BLOCK_TYPE blockType);
        Action_u_ptr        removeBlock(const GridXY& gridXY, DYNAMIC_BLOCK_TYPE blockType);
        Action_u_ptr        removeBlock(const GridXY& gridXY, INSTANT_BLOCK_TYPE blockType);
        Action_u_ptr        removeBlock(const GridXY& gridXY, FLOOR_BLOCK_TYPE blockType);
        Action_u_ptr        removeBlock(const GridXY& gridXY);
        std::list<Cluster>& stoppedClusters();

      private:
        /****** PRIVATE CONST FUNCTIONS  ******/
        [[nodiscard]] bool contains(const GridXY& gridXY) const;

        /****** DATA MEMBERS  ******/
        std::map<GridXY, DYNAMIC_BLOCK_TYPE> m_dynamicBLocks;
        std::map<GridXY, INSTANT_BLOCK_TYPE> m_instantBLocks;
        std::set<GridXY>                     m_levelBlocks;
        std::set<GridXY>                     m_startBlocks;
        std::set<Line<WorldXY>>              m_boundaries;
        std::list<Cluster>                   m_stoppedClusters;
    };
} // namespace model

#endif // BLOCKS_LEVEL_H
