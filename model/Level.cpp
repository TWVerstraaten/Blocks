//
// Created by pc on 15-12-20.
//

#include "Level.h"

#include "../global/defines.h"
#include "../global/geom.h"

#include <cassert>

namespace model {
    const std::map<GridXY, DYNAMIC_BLOCK_TYPE>& Level::dynamicBlocks() const {
        return m_dynamicBLocks;
    }

    const std::map<GridXY, INSTANT_BLOCK_TYPE>& Level::instantBlocks() const {
        return m_instantBLocks;
    }

    void Level::addBlock(const GridXY& gridXY, DYNAMIC_BLOCK_TYPE blockType) {
        assert(not(m_dynamicBLocks.find(gridXY) != m_dynamicBLocks.end() && m_dynamicBLocks[gridXY] == blockType));
        assert(m_floorBlocks.find(gridXY) != m_floorBlocks.end());
        m_dynamicBLocks[gridXY] = blockType;
    }

    void Level::addBlock(const GridXY& gridXY, INSTANT_BLOCK_TYPE blockType) {
        assert(not(m_instantBLocks.find(gridXY) != m_instantBLocks.end() && m_instantBLocks[gridXY] == blockType));
        assert(m_floorBlocks.find(gridXY) != m_floorBlocks.end());
        m_instantBLocks[gridXY] = blockType;
    }

    void Level::clear() {
        m_floorBlocks.clear();
        m_instantBLocks.clear();
        m_dynamicBLocks.clear();
    }

    bool Level::isFreeStartBlock(const GridXY& gridXY) const {
        D_NOTE_ONCE("Handle starting positions")
        if (m_floorBlocks.find(gridXY) == m_floorBlocks.end()) {
            return false;
        }
        if (m_instantBLocks.find(gridXY) != m_instantBLocks.end()) {
            return false;
        }
        if (m_dynamicBLocks.find(gridXY) != m_dynamicBLocks.end()) {
            return false;
        }
        if (std::find_if(D_CIT(m_stoppedClusters), D_FUNC(cluster, cluster.contains(gridXY))) != m_stoppedClusters.end()) {
            return false;
        }
        return true;
    }

    const WorldLineSet& Level::boundaries() const {
        return m_boundaries;
    }

    bool Level::contains(const GridXY& gridXY) const {
        return m_floorBlocks.find(gridXY) != m_floorBlocks.end();
    }

    void Level::createBoundaries() {
        std::set<GridXY> blocks;
        for (const auto& [point, _] : m_floorBlocks) {
            blocks.emplace(point);
        }
        m_boundaries = geom::getSidesFromGridXY(blocks);
        for (const auto& cluster : m_stoppedClusters) {
            m_boundaries.merge(geom::getSidesFromGridXY(cluster.gridXY()));
        }
    }

    void Level::addBlock(const GridXY& gridXY, FLOOR_BLOCK_TYPE blockType) {
        m_floorBlocks[gridXY] = blockType;
    }

    void Level::removeBlock(const GridXY& gridXY, DYNAMIC_BLOCK_TYPE blockType) {
        assert(m_dynamicBLocks.find(gridXY) != m_dynamicBLocks.end() && m_dynamicBLocks[gridXY] == blockType);
        m_dynamicBLocks.erase(gridXY);
    }

    void Level::removeBlock(const GridXY& gridXY, INSTANT_BLOCK_TYPE blockType) {
        assert(m_instantBLocks.find(gridXY) != m_instantBLocks.end() && m_instantBLocks[gridXY] == blockType);
        m_instantBLocks.erase(gridXY);
    }

    void Level::removeBlock(const GridXY& gridXY, [[maybe_unused]] FLOOR_BLOCK_TYPE blockType) {
        assert(m_floorBlocks.find(gridXY) != m_floorBlocks.end());
        m_floorBlocks.erase(gridXY);
    }

    std::list<Cluster>& Level::stoppedClusters() {
        return m_stoppedClusters;
    }

    const std::list<Cluster>& Level::stoppedClusters() const {
        return m_stoppedClusters;
    }

    const std::map<GridXY, FLOOR_BLOCK_TYPE>& Level::floorBlocks() const {
        return m_floorBlocks;
    }

    GridXYSet Level::blocks(FLOOR_BLOCK_TYPE blockType) const {
        std::set<GridXY> result;
        for (const auto& [point, type] : m_floorBlocks) {
            if (type == blockType) {
                result.emplace(point);
            }
        }
        return result;
    }

} // namespace model