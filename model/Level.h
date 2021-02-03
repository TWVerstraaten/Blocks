//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_LEVEL_H
#define BLOCKS_LEVEL_H

#include "Cluster.h"
#include "GridXy.h"
#include "Level_enums.h"
#include "WorldLine.h"

#include <map>
#include <memory>
#include <set>
#include <vector>

namespace io {
    std::istream& operator>>(std::istream& in, model::Level& level);
}

namespace model {

    class Level {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        Level() = default;

        /****** CONST GETTERS  ******/
        [[nodiscard]] bool                                        isFreeStartBlock(const GridXy& gridXy) const;
        [[nodiscard]] const WorldLineVector&                      sides() const;
        [[nodiscard]] const std::map<GridXy, DYNAMIC_BLOCK_TYPE>& dynamicBlocks() const;
        [[nodiscard]] const std::map<GridXy, INSTANT_BLOCK_TYPE>& instantBlocks() const;
        [[nodiscard]] const std::vector<Cluster>&                 stoppedClusters() const;
        [[nodiscard]] const std::map<GridXy, FLOOR_BLOCK_TYPE>&   floorBlocks() const;

        /****** CONST FUNCTIONS  ******/
        [[nodiscard]] GridXyVector blocks(FLOOR_BLOCK_TYPE blockType) const;

        /****** NON CONST FUNCTIONS  ******/
        void                  clear();
        void                  buildSides();
        void                  addBlock(const GridXy& gridXy, DYNAMIC_BLOCK_TYPE blockType);
        void                  addBlock(const GridXy& gridXy, INSTANT_BLOCK_TYPE blockType);
        void                  addBlock(const GridXy& gridXy, FLOOR_BLOCK_TYPE blockType);
        void                  removeBlock(const GridXy& gridXy, DYNAMIC_BLOCK_TYPE blockType);
        void                  removeBlock(const GridXy& gridXy, INSTANT_BLOCK_TYPE blockType);
        void                  removeBlock(const GridXy& gridXy, FLOOR_BLOCK_TYPE blockType);
        std::vector<Cluster>& stoppedClusters();

      private:
        friend std::istream& io::operator>>(std::istream& in, model::Level& level);

        /****** PRIVATE CONST FUNCTIONS  ******/
        [[nodiscard]] bool contains(const GridXy& gridXy) const;

        /****** DATA MEMBERS  ******/
        std::map<GridXy, DYNAMIC_BLOCK_TYPE> m_dynamicBLocks;
        std::map<GridXy, INSTANT_BLOCK_TYPE> m_instantBLocks;
        std::map<GridXy, FLOOR_BLOCK_TYPE>   m_floorBlocks;
        WorldLineVector                      m_sides;
        std::vector<Cluster>                 m_stoppedClusters;
    };
} // namespace model

#endif // BLOCKS_LEVEL_H
