//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_LEVEL_H
#define BLOCKS_LEVEL_H

#include "Cluster.h"
#include "GridXY.h"
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
        [[nodiscard]] bool                                        isFreeStartBlock(const GridXY& gridXY) const;
        [[nodiscard]] const WorldLineSet&                         boundaries() const;
        [[nodiscard]] const std::map<GridXY, DYNAMIC_BLOCK_TYPE>& dynamicBlocks() const;
        [[nodiscard]] const std::map<GridXY, INSTANT_BLOCK_TYPE>& instantBlocks() const;
        [[nodiscard]] const std::list<Cluster>&                   stoppedClusters() const;
        [[nodiscard]] const std::map<GridXY, FLOOR_BLOCK_TYPE>&   floorBlocks() const;

        /****** CONST FUNCTIONS  ******/
        [[nodiscard]] GridXYSet blocks(FLOOR_BLOCK_TYPE blockType) const;

        /****** NON CONST FUNCTIONS  ******/
        void                clear();
        void                createBoundaries();
        void                addBlock(const GridXY& gridXY, DYNAMIC_BLOCK_TYPE blockType);
        void                addBlock(const GridXY& gridXY, INSTANT_BLOCK_TYPE blockType);
        void                addBlock(const GridXY& gridXY, FLOOR_BLOCK_TYPE blockType);
        void                removeBlock(const GridXY& gridXY, DYNAMIC_BLOCK_TYPE blockType);
        void                removeBlock(const GridXY& gridXY, INSTANT_BLOCK_TYPE blockType);
        void                removeBlock(const GridXY& gridXY, FLOOR_BLOCK_TYPE blockType);
        std::list<Cluster>& stoppedClusters();

      private:
        /****** PRIVATE CONST FUNCTIONS  ******/
        [[nodiscard]] bool contains(const GridXY& gridXY) const;

        /****** DATA MEMBERS  ******/
        std::map<GridXY, DYNAMIC_BLOCK_TYPE> m_dynamicBLocks;
        std::map<GridXY, INSTANT_BLOCK_TYPE> m_instantBLocks;
        std::map<GridXY, FLOOR_BLOCK_TYPE>   m_floorBlocks;
        WorldLineSet                         m_boundaries;
        std::list<Cluster>                   m_stoppedClusters;
    };
} // namespace model

#endif // BLOCKS_LEVEL_H
