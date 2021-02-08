//
// Created by pc on 15-12-20.
//

#include "Level.h"

#include "../misc/defines.h"
#include "../misc/geom.h"

#include <cassert>

namespace model {
    const std::map<GridXy, DYNAMIC_BLOCK_TYPE>& Level::dynamicBlocks() const {
        return m_dynamicBLocks;
    }

    const std::map<GridXy, INSTANT_BLOCK_TYPE>& Level::instantBlocks() const {
        return m_instantBLocks;
    }

    void Level::addBlock(const GridXy& gridXy, DYNAMIC_BLOCK_TYPE blockType) {
        assert(not(m_dynamicBLocks.find(gridXy) != m_dynamicBLocks.end() && m_dynamicBLocks[gridXy] == blockType));
        assert(m_floorBlocks.find(gridXy) != m_floorBlocks.end());
        m_dynamicBLocks[gridXy] = blockType;
    }

    void Level::addBlock(const GridXy& gridXy, INSTANT_BLOCK_TYPE blockType) {
        assert(not(m_instantBLocks.find(gridXy) != m_instantBLocks.end() && m_instantBLocks[gridXy] == blockType));
        assert(m_floorBlocks.find(gridXy) != m_floorBlocks.end());
        m_instantBLocks[gridXy] = blockType;
    }

    void Level::clear() {
        m_floorBlocks.clear();
        m_instantBLocks.clear();
        m_dynamicBLocks.clear();
        m_sides.clear();
        m_stoppedClusters.clear();
    }

    bool Level::isFreeStartBlock(const GridXy& gridXy) const {
        D_NOTE_ONCE("Handle starting positions")
        if (m_floorBlocks.find(gridXy) == m_floorBlocks.end()) {
            return false;
        }
        if (m_instantBLocks.find(gridXy) != m_instantBLocks.end()) {
            return false;
        }
        if (m_dynamicBLocks.find(gridXy) != m_dynamicBLocks.end()) {
            return false;
        }
        if (std::find_if(D_CIT(m_stoppedClusters), D_FUNC(cluster, cluster.contains(gridXy))) != m_stoppedClusters.end()) {
            return false;
        }
        return true;
    }

    const WorldLineVector& Level::sides() const {
        return m_sides;
    }

    bool Level::contains(const GridXy& gridXy) const {
        return m_floorBlocks.find(gridXy) != m_floorBlocks.end();
    }

    void Level::buildSides() {
        GridXyVector blocks;
        for (const auto& [point, _] : m_floorBlocks) {
            blocks.emplace_back(point);
        }
        m_sides = geom::getSidesFromGridXy(blocks);
        for (auto& cluster : m_stoppedClusters) {
            cluster.buildSides();
            auto clusterSides = geom::getSidesFromGridXy(cluster.gridXyVector());
            std::copy(D_CIT(clusterSides), std::back_inserter(clusterSides));
        }
    }

    void Level::addBlock(const GridXy& gridXy, FLOOR_BLOCK_TYPE blockType) {
        m_floorBlocks[gridXy] = blockType;
    }

    void Level::removeBlock(const GridXy& gridXy, DYNAMIC_BLOCK_TYPE blockType) {
        assert(m_dynamicBLocks.find(gridXy) != m_dynamicBLocks.end() && m_dynamicBLocks[gridXy] == blockType);
        m_dynamicBLocks.erase(gridXy);
    }

    void Level::removeBlock(const GridXy& gridXy, INSTANT_BLOCK_TYPE blockType) {
        assert(m_instantBLocks.find(gridXy) != m_instantBLocks.end() && m_instantBLocks[gridXy] == blockType);
        m_instantBLocks.erase(gridXy);
    }

    void Level::removeBlock(const GridXy& gridXy, [[maybe_unused]] FLOOR_BLOCK_TYPE blockType) {
        assert(m_floorBlocks.find(gridXy) != m_floorBlocks.end());
        m_floorBlocks.erase(gridXy);
    }

    std::vector<GridXyContainer>& Level::stoppedClusters() {
        return m_stoppedClusters;
    }

    const std::vector<GridXyContainer>& Level::stoppedClusters() const {
        return m_stoppedClusters;
    }

    const std::map<GridXy, FLOOR_BLOCK_TYPE>& Level::floorBlocks() const {
        return m_floorBlocks;
    }

    GridXyVector Level::blocks(FLOOR_BLOCK_TYPE blockType) const {
        GridXyVector result;
        for (const auto& [point, type] : m_floorBlocks) {
            if (type == blockType) {
                result.emplace_back(point);
            }
        }
        return result;
    }

} // namespace model