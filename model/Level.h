//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_LEVEL_H
#define BLOCKS_LEVEL_H

#include "Cluster.h"
#include "GridXy.h"
#include "Level_enums.h"
#include "WorldLine.h"

#include <list>
#include <map>
#include <memory>
#include <set>

namespace model {

    class Level {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        Level() = default;

        /****** CONST GETTERS  ******/
        [[nodiscard]] bool                                        isFreeStartBlock(const GridXy& gridXy) const;
        [[nodiscard]] const WorldLineSet&                         sides() const;
        [[nodiscard]] const std::map<GridXy, DYNAMIC_BLOCK_TYPE>& dynamicBlocks() const;
        [[nodiscard]] const std::map<GridXy, INSTANT_BLOCK_TYPE>& instantBlocks() const;
        [[nodiscard]] const std::list<Cluster>&                   stoppedClusters() const;
        [[nodiscard]] const std::map<GridXy, FLOOR_BLOCK_TYPE>&   floorBlocks() const;

        /****** CONST FUNCTIONS  ******/
        [[nodiscard]] GridXySet blocks(FLOOR_BLOCK_TYPE blockType) const;

        /****** NON CONST FUNCTIONS  ******/
        void                clear();
        void                buildSides();
        void                addBlock(const GridXy& gridXy, DYNAMIC_BLOCK_TYPE blockType);
        void                addBlock(const GridXy& gridXy, INSTANT_BLOCK_TYPE blockType);
        void                addBlock(const GridXy& gridXy, FLOOR_BLOCK_TYPE blockType);
        void                removeBlock(const GridXy& gridXy, DYNAMIC_BLOCK_TYPE blockType);
        void                removeBlock(const GridXy& gridXy, INSTANT_BLOCK_TYPE blockType);
        void                removeBlock(const GridXy& gridXy, FLOOR_BLOCK_TYPE blockType);
        std::list<Cluster>& stoppedClusters();

      private:
        /****** PRIVATE CONST FUNCTIONS  ******/
        [[nodiscard]] bool contains(const GridXy& gridXy) const;

        /****** DATA MEMBERS  ******/
        std::map<GridXy, DYNAMIC_BLOCK_TYPE> m_dynamicBLocks;
        std::map<GridXy, INSTANT_BLOCK_TYPE> m_instantBLocks;
        std::map<GridXy, FLOOR_BLOCK_TYPE>   m_floorBlocks;
        WorldLineSet                         m_sides;
        std::list<Cluster>                   m_stoppedClusters;
    };
} // namespace model

#endif // BLOCKS_LEVEL_H
